//                                               -*- C++ -*-
/**
 *  @file  NormalCopula.cxx
 *  @brief A class that implements an independent copula
 *
 *  Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author schueller
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include <cmath>
#include "NormalCopula.hxx"
#include "Uniform.hxx"
#include "IdentityMatrix.hxx"
#include "NatafEllipticalCopulaEvaluation.hxx"
#include "NatafEllipticalCopulaGradient.hxx"
#include "NatafEllipticalCopulaHessian.hxx"
#include "InverseNatafEllipticalCopulaEvaluation.hxx"
#include "InverseNatafEllipticalCopulaGradient.hxx"
#include "InverseNatafEllipticalCopulaHessian.hxx"
#include "Indices.hxx"
#include "RandomGenerator.hxx"
#include "Exception.hxx"
#include "Description.hxx"
#include "DistFunc.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(NormalCopula);

static Factory<NormalCopula> RegisteredFactory("NormalCopula");

/* Default constructor */
NormalCopula::NormalCopula(const UnsignedLong dim)
  : CopulaImplementation("NormalCopula")
  , correlation_(dim)
  , normal_(dim)
{
  // The range is generic for all the copulas
  setDimension(dim);
}

/* Default constructor */
NormalCopula::NormalCopula(const CorrelationMatrix & correlation)
  : CopulaImplementation("NormalCopula")
  , correlation_(correlation)
  , normal_(NumericalPoint(correlation.getNbRows(), 0.0), NumericalPoint(correlation.getNbRows(), 1.0), correlation)
{
  // The range is generic for all the copulas
  setDimension(correlation.getNbRows());
}

/* Comparison operator */
Bool NormalCopula::operator ==(const NormalCopula & other) const
{
  if (this == &other) return true;
  return correlation_ == other.correlation_;
}

/* String converter */
String NormalCopula::__repr__() const
{
  OSS oss;
  oss << "class=" << NormalCopula::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " correlation=" << correlation_;
  return oss;
}

String NormalCopula::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(R = " << correlation_.__str__(offset) << ")";
  return oss;
}

/* Virtual constructor */
NormalCopula * NormalCopula::clone() const
{
  return new NormalCopula(*this);
}

/* Get one realization of the distribution */
NumericalPoint NormalCopula::getRealization() const
{
  UnsignedLong dimension(getDimension());
  if (hasIndependentCopula()) return RandomGenerator::Generate(dimension);
  else
    {
      NumericalPoint realization(normal_.getRealization());
      for (UnsignedLong i = 0; i < dimension; ++i) realization[i] = DistFunc::pNormal(realization[i]);
      return realization;
    }
}

/* Get the DDF of the distribution */
NumericalPoint NormalCopula::computeDDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point has a dimension not compatible with the distribution dimension";
  // Be careful to evaluate the copula only in the interior of its support
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      // If outside of the support return 0.0
      if ((point[i] <= 0.0) || (point[i] >= 1.0)) return NumericalPoint(dimension, 0.0);
    }
  NumericalPoint x(dimension);
  NumericalPoint marginalPDF(dimension);
  NumericalScalar marginalPDFProduct(1.0);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar xi(DistFunc::qNormal(point[i]));
      x[i] = xi;
      // .398942280401432677939946059934 = 1 / sqrt(2.pi)
      const NumericalScalar pdfI(0.398942280401432677939946059934 * exp(-0.5 * xi * xi));
      marginalPDF[i] = pdfI;
      marginalPDFProduct *= pdfI;
    }
  const NumericalPoint ddfNorm(normal_.computeDDF(x));
  const NumericalScalar pdfNorm(normal_.computePDF(x));
  NumericalPoint ddf(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i) ddf[i] = (ddfNorm[i] + x[i] * pdfNorm) / (marginalPDFProduct * marginalPDF[i]);
  return ddf;
}

/* Get the PDF of the distribution */
NumericalScalar NormalCopula::computePDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point has a dimension not compatible with the distribution dimension";
  // Be careful to evaluate the copula only in the interior of its support
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      // If outside of the support return 0.0
      if ((point[i] <= 0.0) || (point[i] >= 1.0)) return 0.0;
    }
  // Compute the normal point such that a normal distribution with this copula
  // and standard 1D normal marginals has the same CDF at this normal point
  // than the copula at the given point.
  // Compute the multiplicative factor between the copula PDF
  // and the PDF of the associated generic normal using the specific form of
  // the standard normal PDF
  NumericalPoint normalPoint(dimension);
  NumericalScalar value(0.0);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar yi(DistFunc::qNormal(point[i]));
      normalPoint[i] = yi;
      value += yi * yi;
    }
  // .398942280401432677939946059934 = 1 / sqrt(2.pi)
  value = pow(.398942280401432677939946059934, dimension) * exp(-0.5 * value);
  return normal_.computePDF(normalPoint) / value;
}

/* Get the CDF of the distribution */
NumericalScalar NormalCopula::computeCDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point has a dimension not compatible with the distribution dimension";
  // Compute the normal point such that a normal distribution with this copula
  // and standard 1D normal marginals has the same CDF at this normal point
  // than the copula at the given point.
  // Be careful to evaluate the copula only in the interior of its support
  Indices indices;
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      // If outside of the support, in the lower part, return 0.0
      if (point[i] <= 0.0) return 0.0;
      // If the current component is in the interior of the support, its index
      // is taken into account
      if (point[i] < 1.0) indices.add(i);
    }
  const UnsignedLong activeDimension(indices.getSize());
  // Quick return if all the components are >= 1
  if (activeDimension == 0) return 1.0;
  NumericalPoint normalPoint(activeDimension);
  for (UnsignedLong i = 0; i < activeDimension; ++i) normalPoint[i] = DistFunc::qNormal(point[indices[i]]);
  // In the usual case when the given point is in the interior of the support
  // use the associated normal distribution
  if (dimension == activeDimension) return normal_.computeCDF(normalPoint);
  // In the other case, we must use the appropriate marginal distribution
  else return normal_.getMarginal(indices)->computeCDF(normalPoint);
} // computeCDF

/* Get the CDF of the distribution */
NumericalScalar NormalCopula::computeComplementaryCDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point has a dimension not compatible with the distribution dimension";
  // Compute the normal point such that a normal distribution with this copula
  // and standard 1D normal marginals has the same CDF at this normal point
  // than the copula at the given point.
  // Be careful to evaluate the copula only in the interior of its support
  Indices indices;
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      // If outside of the support, in the lower part, return 0.0
      if (point[i] <= 0.0) return 0.0;
      // If the current component is in the interior of the support, its index
      // is taken into account
      if (point[i] < 1.0) indices.add(i);
    }
  const UnsignedLong activeDimension(indices.getSize());
  // Quick return if all the components are >= 1
  if (activeDimension == 0) return 1.0;
  NumericalPoint normalPoint(activeDimension);
  for (UnsignedLong i = 0; i < activeDimension; ++i) normalPoint[i] = DistFunc::qNormal(point[indices[i]], true);
  // In the usual case when the given point is in the interior of the support
  // use the associated normal distribution
  if (dimension == activeDimension) return normal_.computeComplementaryCDF(normalPoint);
  // In the other case, we must use the appropriate marginal distribution
  else return Distribution(normal_.getMarginal(indices)).computeComplementaryCDF(normalPoint);
} // computeCDF

/* Compute the probability content of an interval */
NumericalScalar NormalCopula::computeProbability(const Interval & interval) const
{
  const UnsignedLong dimension(getDimension());
  if (interval.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given interval has a dimension not compatible with the distribution dimension";
  // Reduce the given interval to the support of the distribution, which is the nD unit cube
  const Interval intersect(interval.intersect(Interval(dimension)));
  // If the intersection is empty
  if (intersect.isNumericallyEmpty()) return 0.0;
  const NumericalPoint lowerBoundIntersect(intersect.getLowerBound());
  const NumericalPoint upperBoundIntersect(intersect.getUpperBound());
  NumericalPoint lowerBound(dimension);
  NumericalPoint upperBound(dimension);
  Interval::BoolCollection finiteLowerBound(dimension);
  Interval::BoolCollection finiteUpperBound(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      if (lowerBoundIntersect[i] == 0.0)
        {
          finiteLowerBound[i] = false;
          lowerBound[i] = -1.0;
        }
      else
        {
          finiteLowerBound[i] = true;
          lowerBound[i] = DistFunc::qNormal(lowerBoundIntersect[i]);
        }
      if (upperBoundIntersect[i] == 1.0)
        {
          finiteUpperBound[i] = false;
          upperBound[i] = 1.0;
        }
      else
        {
          finiteUpperBound[i] = true;
          upperBound[i] = DistFunc::qNormal(upperBoundIntersect[i]);
        }
    }
  return normal_.computeProbability(Interval(lowerBound, upperBound, finiteLowerBound, finiteUpperBound));
}

/* Get the PDF gradient of the distribution */
NumericalPoint NormalCopula::computePDFGradient(const NumericalPoint & point) const
{
  return NumericalPoint(0, 0.0);
}

/* Get the CDF gradient of the distribution */
NumericalPoint NormalCopula::computeCDFGradient(const NumericalPoint & point) const
{
  return NumericalPoint(0, 0.0);
}

/* Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1)
   For Normal distribution, the conditional distribution is also Normal, with mean and covariance
   such as:
   mean_cond = mean(x) + cov(x, y).cov(y, y)^(-1)(y - mean(y))
   cov_cond = cov(x, x) - cov(x, y).cov(y, y)^(-1)cov(x, y)
   This expression simplifies if we use the inverse of the Cholesky factor of the covariance matrix.
   See [Lebrun, Dutfoy, "Rosenblatt and Nataf transformation"]
*/
NumericalScalar NormalCopula::computeConditionalPDF(const NumericalScalar x, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional PDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return 1.0;
  // General case
  NumericalPoint u(conditioningDimension);
  for (UnsignedLong i = 0; i < conditioningDimension; ++i) u[i] = DistFunc::qNormal(y[i]);
  return normal_.computeConditionalPDF(DistFunc::qNormal(x), u);
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar NormalCopula::computeConditionalCDF(const NumericalScalar x, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional CDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return x;
  // General case
  NumericalPoint u(conditioningDimension);
  for (UnsignedLong i = 0; i < conditioningDimension; ++i) u[i] = DistFunc::qNormal(y[i]);
  return normal_.computeConditionalCDF(DistFunc::qNormal(x), u);
}

/* Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
NumericalScalar NormalCopula::computeConditionalQuantile(const NumericalScalar q, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile with a conditioning point of dimension greater or equal to the distribution dimension.";
  if ((q < 0.0) || (q > 1.0)) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile for a probability level outside of [0, 1]";
  if (q == 0.0) return 0.0;
  if (q == 1.0) return 1.0;
  // Special case when no contitioning or independent copula
  if ((conditioningDimension == 0) || hasIndependentCopula()) return q;
  // General case
  NumericalPoint u(conditioningDimension);
  for (UnsignedLong i = 0; i < conditioningDimension; ++i) u[i] = DistFunc::qNormal(y[i]);
  return DistFunc::pNormal(normal_.computeConditionalQuantile(q, u));
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
NormalCopula::Implementation NormalCopula::getMarginal(const Indices & indices) const
{
  const UnsignedLong dimension(getDimension());
  if (!indices.check(dimension - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal distribution must be in the range [0, dim-1] and  must be different";
  // Special case for dimension 1
  if (dimension == 1) return clone();
  // General case
  const UnsignedLong outputDimension(indices.getSize());
  CorrelationMatrix R(outputDimension);
  // Extract the correlation matrix, the marginal standard deviations and means
  for (UnsignedLong i = 0; i < outputDimension; ++i)
    {
      const UnsignedLong index_i(indices[i]);
      for (UnsignedLong j = 0; j <= i; ++j) R(i, j) = correlation_(index_i, indices[j]);
    }
  return new NormalCopula(R);
}

/* Get the isoprobabilist transformation */
NormalCopula::IsoProbabilisticTransformation NormalCopula::getIsoProbabilisticTransformation() const
{
  IsoProbabilisticTransformation transformation;
  transformation.setEvaluationImplementation(new NatafEllipticalCopulaEvaluation(getStandardDistribution(), normal_.getInverseCholesky()));
  transformation.setGradientImplementation(new NatafEllipticalCopulaGradient(getStandardDistribution(), normal_.getInverseCholesky()));
  transformation.setHessianImplementation(new NatafEllipticalCopulaHessian(getStandardDistribution(), normal_.getInverseCholesky()));

  return transformation;
}

/* Get the inverse isoprobabilist transformation */
NormalCopula::InverseIsoProbabilisticTransformation NormalCopula::getInverseIsoProbabilisticTransformation() const
{
  InverseIsoProbabilisticTransformation transformation;
  transformation.setEvaluationImplementation(new InverseNatafEllipticalCopulaEvaluation(getStandardDistribution(), normal_.getCholesky()));
  transformation.setGradientImplementation(new InverseNatafEllipticalCopulaGradient(getStandardDistribution(), normal_.getCholesky()));
  transformation.setHessianImplementation(new InverseNatafEllipticalCopulaHessian(getStandardDistribution(), normal_.getCholesky()));

  return transformation;
}

/* Tell if the distribution has independent copula */
Bool NormalCopula::hasIndependentCopula() const
{
  return normal_.hasIndependentCopula();
}

/* Parameters value and description accessor */
NormalCopula::NumericalPointWithDescriptionCollection NormalCopula::getParametersCollection() const
{
  const UnsignedLong dimension(getDimension());
  NumericalPointWithDescriptionCollection parameters(0);
  if (dimension > 1)
    {
      // Put the dependence parameters
      const UnsignedLong parametersDimension(dimension * (dimension - 1) / 2);
      NumericalPointWithDescription point(parametersDimension);
      Description description(parametersDimension);
      point.setName(getName());
      UnsignedLong dependenceIndex(0);
      for (UnsignedLong i = 0; i < dimension; ++i)
        {
          for (UnsignedLong j = 0; j < i; ++j)
            {
              point[dependenceIndex] = correlation_(i, j);
              OSS oss;
              oss << "R_" << i + 1 << "_" << j + 1;
              description[dependenceIndex] = oss;
              ++dependenceIndex;
            }
        }
      point.setDescription(description);
      parameters.add(point);
    } // dimension > 1
  return parameters;
} // getParametersCollection

void NormalCopula::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  // Check if the given parameters are ok
  if (parametersCollection.getSize() != 1) throw InvalidArgumentException(HERE) << "Error: the given collection has a size=" << parametersCollection.getSize() << " but should be of size=1";
  const NumericalPoint parameters(parametersCollection[0]);
  const UnsignedLong dimension(getDimension());
  if (parameters.getDimension() != dimension * (dimension - 1) / 2) throw InvalidArgumentException(HERE) << "Error: got " << parameters.getDimension() << " parameters instead of " << dimension * (dimension - 1) / 2;
  if (dimension == 1) return;
  UnsignedLong dependenceIndex(0);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      for (UnsignedLong j = 0; j < i; ++j)
        {
          correlation_(i, j) = parameters[dependenceIndex];
          ++dependenceIndex;
        }
    }
}

/* Compute the correlation matrix of a Normal Copula from its Spearman correlation matrix */
CorrelationMatrix NormalCopula::GetCorrelationFromSpearmanCorrelation(const CorrelationMatrix & matrix)
{
  const UnsignedLong dimension(matrix.getNbRows());
  CorrelationMatrix result(dimension);
  for (UnsignedLong i = 1; i < dimension; ++i)
    for (UnsignedLong j = 0; j < i; ++j) result(i, j) = 2.0 * sin(M_PI * matrix(i, j) / 6.0);
  if (!result.isPositiveDefinite()) throw NotSymmetricDefinitePositiveException(HERE) << "Error: the normal copula correlation matrix built from the given Spearman correlation matrix is not definite positive";
  return result;
}

/* Compute the correlation matrix of a Normal Copula from its Kendal correlation matrix */
CorrelationMatrix NormalCopula::GetCorrelationFromKendallCorrelation(const CorrelationMatrix & matrix)
{
  const UnsignedLong dimension(matrix.getNbRows());
  CorrelationMatrix result(dimension);
  for (UnsignedLong i = 1; i < dimension; ++i)
    for (UnsignedLong j = 0; j < i; ++j) result(i, j) = sin(M_PI_2 * matrix(i, j));
  if (!result.isPositiveDefinite()) throw NotSymmetricDefinitePositiveException(HERE) << "Error: the normal copula correlation matrix built from the given Kendall correlation matrix is not definite positive";
  return result;
}

/* Method save() stores the object through the StorageManager */
void NormalCopula::save(Advocate & adv) const
{
  CopulaImplementation::save(adv);
  adv.saveAttribute( "correlation_", correlation_ );
  adv.saveAttribute( "covariance_duplicate", covariance_ );
  adv.saveAttribute( "normal_", normal_ );
  adv.saveAttribute( "integrationNodesNumber_duplicate", integrationNodesNumber_ );
  adv.saveAttribute( "isAlreadyComputedCovariance_duplicate", isAlreadyComputedCovariance_ );
}

/* Method load() reloads the object from the StorageManager */
void NormalCopula::load(Advocate & adv)
{
  // The range is generic for all the copulas
  CopulaImplementation::load(adv);
  adv.loadAttribute( "correlation_", correlation_ );
  adv.loadAttribute( "covariance_duplicate", covariance_ );
  adv.loadAttribute( "normal_", normal_ );
  adv.loadAttribute( "integrationNodesNumber_duplicate", integrationNodesNumber_ );
  adv.loadAttribute( "isAlreadyComputedCovariance_duplicate", isAlreadyComputedCovariance_ );
}

END_NAMESPACE_OPENTURNS
