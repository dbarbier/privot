//                                               -*- C++ -*-
/**
 *  @file  SklarCopula.cxx
 *  @brief The SklarCopula distribution
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
 *  @author lebrun
 *  @date   2008-09-09 23:26:29 +0200 (mar, 09 sep 2008)
 */
#include "SklarCopula.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"
#include "MarginalTransformationEvaluation.hxx"
#include "MarginalTransformationGradient.hxx"
#include "MarginalTransformationHessian.hxx"
#include "RosenblattEvaluation.hxx"
#include "InverseRosenblattEvaluation.hxx"
#include "NumericalMathFunctionImplementation.hxx"
#include "RandomGenerator.hxx"
#include "Uniform.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS


typedef NumericalMathFunctionImplementation::EvaluationImplementation EvaluationImplementation;
typedef NumericalMathFunctionImplementation::GradientImplementation   GradientImplementation;
typedef NumericalMathFunctionImplementation::HessianImplementation    HessianImplementation;


CLASSNAMEINIT(SklarCopula);

static Factory<SklarCopula> RegisteredFactory("SklarCopula");

/* Default constructor */
SklarCopula::SklarCopula()
  : CopulaImplementation("SklarCopula"),
    distribution_(),
    marginalCollection_()
{
  setDimension( 0 );
}

/* Parameters constructor */
SklarCopula::SklarCopula(const Distribution distribution)
  : CopulaImplementation("SklarCopula"),
    distribution_(distribution),
    marginalCollection_(distribution.getDimension())
{
  // We set the dimension of the SklarCopula distribution
  const UnsignedLong dimension(distribution.getDimension());
  setDimension( dimension );
  // Extract all the 1D marginal distributions
  for (UnsignedLong i = 0; i < dimension; ++i) marginalCollection_[i] = distribution.getMarginal(i);
}

/* Comparison operator */
Bool SklarCopula::operator ==(const SklarCopula & other) const
{
  if (this == &other) return true;
  return distribution_ == other.distribution_;
}

/* String converter */
String SklarCopula::__repr__() const
{
  OSS oss;
  oss << "class=" << SklarCopula::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " distribution=" << distribution_;
  return oss;
}

/* Virtual constructor */
SklarCopula * SklarCopula::clone() const
{
  return new SklarCopula(*this);
}

/* Get one realization of the distribution
   F(x_1,\dots,x_n) = C(F_1(x_1),\dots,F_n(x_n))
   so a realization of C is a realization of F marginaly composed with F_i */
NumericalPoint SklarCopula::getRealization() const
{
  const UnsignedLong dimension(getDimension());
  // Special case for independent copula, for improved performance
  if (hasIndependentCopula()) return RandomGenerator::Generate(dimension);
  NumericalPoint realization(distribution_.getRealization());
  for (UnsignedLong i = 0; i < dimension; ++i) realization[i] = marginalCollection_[i].computeCDF(realization[i]);
  return realization;
}

/* Get the DDF of the distribution
   F(x_1,\dots,x_n) = C(u_1,\dots,u_n)
   where u_i = F_i(x_i)
   so p(x_1,\dots,x_n) = c(u_1,\dots,u_n)\prod_{i=1}^n p_i(x_i)
   and dp/dx_k(x_1,\dots,x_n) = [dc/du_k(u_1,\dots,u_n)p_k(x_k) +
   p'_k(x_k)/p_k(x_k)c(u_1,\dots,u_n)]\prod_{i=1}^n p_i(x_i)
   but c(u_1,\dots,u_n) = p(x_1,\dots,u_n)/\prod_{i=1}^n p_i(x_i)
   so dp/dx_k(x_1,\dots,x_n) = dc/du_k(u_1,\dots,u_n)p_k(x_k)\prod_{i=1}^n p_i(x_i) +
   p'_k(x_k)/p_k(x_k)p(x_1,\dots,x_n)
*/
NumericalPoint SklarCopula::computeDDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  // Early exit for the 1D case (Uniform(0,1) distribution)
  if (dimension == 1) return NumericalPoint(1, 0.0);
  NumericalPoint x(point);
  NumericalPoint pdfX(dimension);
  NumericalPoint ddfX(dimension);
  NumericalScalar factor(1.0);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar ui(point[i]);
      if ((ui <= 0.0) || ui >= 1.0) return NumericalPoint(dimension, 0.0);
      const NumericalPoint xi(marginalCollection_[i].computeQuantile(ui));
      x[i] = xi[0];
      pdfX[i] = marginalCollection_[i].computePDF(xi);
      ddfX[i] = marginalCollection_[i].computeDDF(xi)[0];
      factor *= pdfX[i];
      if (factor == 0.0) return NumericalPoint(dimension, 0.0);
    }
  const NumericalScalar pdfDistribution(distribution_.computePDF(x));
  NumericalPoint result(distribution_.computeDDF(x));
  for (UnsignedLong i = 0; i < dimension; ++i) result[i] -= ddfX[i] * pdfDistribution / pdfX[i];
  return result * (1.0 / factor);
}

/* Get the PDF of the distribution
   F(x_1,\dots,x_n) = C(F_1(x_1),\dots,F_n(x_n))
   so p(x_1,\dots,x_n) = c(F_1(x_1),\dots,F_n(x_n))\prod_{i=1}^n p_i(x_i) */
NumericalScalar SklarCopula::computePDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  // Early exit for the 1D case (Uniform(0,1) distribution)
  if (dimension == 1)
    {
      if ((point[0] > 0.0) && (point[0] <= 1.0)) return 1.0;
      return 0.0;
    }
  NumericalPoint x(dimension);
  NumericalScalar factor(1.0);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar ui(point[i]);
      if ((ui <= 0.0) || ui > 1.0) return 0.0;
      const NumericalPoint xi(marginalCollection_[i].computeQuantile(ui));
      x[i] = xi[0];
      factor *= marginalCollection_[i].computePDF(xi);
      if (factor == 0.0) return 0.0;
    }
  return distribution_.computePDF(x) / factor;
}

/* Get the CDF of the distribution
   F(x_1,\dots,x_n) = C(F_1(x_1),\dots,F_n(x_n)) */
NumericalScalar SklarCopula::computeCDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  // Early exit for the 1D case (Uniform(0,1) distribution)
  if (dimension == 1) return Uniform(0.0, 1.0).computeCDF(point);
  NumericalPoint x(point);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar ui(std::min(point[i], 1.0));
      if (ui <= 0.0) return 0.0;
      x[i] = marginalCollection_[i].computeQuantile(ui)[0];
    }
  return distribution_.computeCDF(x);
}

/* Compute the probability content of an interval */
NumericalScalar SklarCopula::computeProbability(const Interval & interval) const
{
  const UnsignedLong dimension(getDimension());
  if (interval.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given interval has a dimension not compatible with the distribution dimension";
  // Reduce the given interval to the support of the distribution, which is the nD unit cube
  const Interval intersect(interval.intersect(Interval(dimension)));
  // If the intersection is empty
  if (intersect.isNumericallyEmpty()) return 0.0;
  const NumericalPoint lowerBoundIntersect(intersect.getLowerBound());
  const NumericalPoint upperBoundIntersect(intersect.getUpperBound());
  // Early exit for the 1D case (Uniform(0,1) distribution)
  if (dimension == 1) return upperBoundIntersect[0] - lowerBoundIntersect[0];
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
          lowerBound[i] = marginalCollection_[i].computeQuantile(lowerBoundIntersect[i])[0];
        }
      if (upperBoundIntersect[i] == 1.0)
        {
          finiteUpperBound[i] = false;
          upperBound[i] = 1.0;
        }
      else
        {
          finiteUpperBound[i] = true;
          upperBound[i] = marginalCollection_[i].computeQuantile(upperBoundIntersect[i])[0];
        }
    }
  return distribution_.computeProbability(Interval(lowerBound, upperBound, finiteLowerBound, finiteUpperBound));
}

/* Get the PDFGradient of the distribution */
NumericalPoint SklarCopula::computePDFGradient(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}

/* Get the CDFGradient of the distribution */
NumericalPoint SklarCopula::computeCDFGradient(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}

/* Get the quantile of the distribution
   F(x_1,\dots,x_n) = C(F_1(x_1),\dots,F_n(x_n)) */
NumericalPoint SklarCopula::computeQuantile(const NumericalScalar prob,
                                            const Bool tail) const
{
  const UnsignedLong dimension(getDimension());
  const NumericalScalar epsilon(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon"));
  if (prob < -epsilon || prob > 1.0 + epsilon) throw InvalidArgumentException(HERE) << "Error: cannot compute a quantile for a probability level outside of [0, 1]";
  if (dimension == 1) return NumericalPoint(1, (tail ? 1.0 - prob : prob));
  NumericalPoint uq(distribution_.computeQuantile(prob));
  for (UnsignedLong i = 0; i < dimension; ++i) uq[i] = tail ? marginalCollection_[i].computeComplementaryCDF(uq[i]) : marginalCollection_[i].computeCDF(uq[i]);
  return uq;
}

/* Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar SklarCopula::computeConditionalPDF(const NumericalScalar x, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional PDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return Distribution(getMarginal(conditioningDimension)).computePDF(x);
  // General case
  NumericalPoint u(conditioningDimension);
  for (UnsignedLong i = 0; i < conditioningDimension; ++i) u[i] = marginalCollection_[i].computeQuantile(y[i])[0];
  const NumericalScalar ux(marginalCollection_[conditioningDimension].computeQuantile(x)[0]);
  const NumericalScalar pdf(marginalCollection_[conditioningDimension].computePDF(ux));
  if (pdf == 0.0) return 0.0;
  return distribution_.computeConditionalPDF(ux, u) / pdf;
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar SklarCopula::computeConditionalCDF(const NumericalScalar x, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional CDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return marginalCollection_[conditioningDimension].computeQuantile(x)[0];
  // General case
  NumericalPoint u(conditioningDimension);
  for (UnsignedLong i = 0; i < conditioningDimension; ++i) u[i] = marginalCollection_[i].computeQuantile(y[i])[0];
  return distribution_.computeConditionalCDF(marginalCollection_[conditioningDimension].computeQuantile(x)[0], u);
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
SklarCopula::Implementation SklarCopula::getMarginal(const Indices & indices) const
{
  // This call will check that indices are correct
  return new SklarCopula(distribution_.getMarginal(indices));
}

/* Get the isoprobabilist transformation */
SklarCopula::IsoProbabilisticTransformation SklarCopula::getIsoProbabilisticTransformation() const
{
  // Special case for distributions with elliptical copula
  if (distribution_.getImplementation()->hasEllipticalCopula())
    {
      // Get the IsoProbabilisticTransformation from the copula
      const IsoProbabilisticTransformation isoprobabilistic(distribution_.getIsoProbabilisticTransformation());
      // Get the right function implementations
      const MarginalTransformationEvaluation rightEvaluation(marginalCollection_, MarginalTransformationEvaluation::TO);
      const EvaluationImplementation p_rightFunction(rightEvaluation.clone());
      // Get the right gradient implementations
      const GradientImplementation p_rightGradient = new MarginalTransformationGradient(rightEvaluation);
      // Get the right hessian implementations
      const HessianImplementation p_rightHessian = new MarginalTransformationHessian(rightEvaluation);
      const IsoProbabilisticTransformation right(p_rightFunction, p_rightGradient, p_rightHessian);
      const IsoProbabilisticTransformation transformation(isoprobabilistic, right);
      return transformation;
    }
  // Else simply use the Rosenblatt transformation
  else return NumericalMathFunctionImplementation(new RosenblattEvaluation(*this));
}

/* Get the inverse isoprobabilist transformation */
SklarCopula::InverseIsoProbabilisticTransformation SklarCopula::getInverseIsoProbabilisticTransformation() const
{
  // Special case for the elliptical copula
  if (distribution_.getImplementation()->hasEllipticalCopula())
    {
      // Get the inverse IsoProbabilisticTransformation from the distribution
      const InverseIsoProbabilisticTransformation inverseIsoprobabilistic(distribution_.getInverseIsoProbabilisticTransformation());
      // Get the left and right function implementations
      const MarginalTransformationEvaluation leftEvaluation(marginalCollection_);
      const EvaluationImplementation p_leftFunction(leftEvaluation.clone());
      // Get the left and right gradient implementations
      const GradientImplementation p_leftGradient = new MarginalTransformationGradient(leftEvaluation);
      // Get the left and right hessian implementations
      const HessianImplementation p_leftHessian = new MarginalTransformationHessian(leftEvaluation);
      const InverseIsoProbabilisticTransformation left(p_leftFunction, p_leftGradient, p_leftHessian);
      const InverseIsoProbabilisticTransformation transformation(left, inverseIsoprobabilistic);
      return transformation;
    }
  // Else simply use the inverse  Rosenblatt transformation
  else return NumericalMathFunctionImplementation(new InverseRosenblattEvaluation(clone()));
}

/* Get the standard distribution */
SklarCopula::Implementation SklarCopula::getStandardDistribution() const
{
  return distribution_.getStandardDistribution().getImplementation();
}

/* Parameters value and description accessor */
SklarCopula::NumericalPointWithDescriptionCollection SklarCopula::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(0);
  const NumericalPointWithDescriptionCollection distributionParameters(distribution_.getParametersCollection());
  const UnsignedLong dimension(distribution_.getDimension());
  // If the underlying distribution has dependence parameters
  if (distributionParameters.getSize() == dimension + 1)
    {
      parameters.add(distributionParameters[dimension]);
    }
  return parameters;
}

/* Tell if the distribution has independent copula */
Bool SklarCopula::hasIndependentCopula() const
{
  return distribution_.getImplementation()->hasIndependentCopula();
}

/* Tell if the distribution has elliptical copula */
Bool SklarCopula::hasEllipticalCopula() const
{
  return distribution_.getImplementation()->hasEllipticalCopula();
}

/* Distribution accessor */
void SklarCopula::setDistribution(const Distribution distribution)
{
  distribution_ = distribution;
}

/* Distribution accessor */
Distribution SklarCopula::getDistribution() const
{
  return distribution_;
}

/* Method save() stores the object through the StorageManager */
void SklarCopula::save(Advocate & adv) const
{
  CopulaImplementation::save(adv);
  adv.saveAttribute( "distribution_", distribution_ );
  adv.saveAttribute( "marginalCollection_", marginalCollection_ );
}

/* Method load() reloads the object from the StorageManager */
void SklarCopula::load(Advocate & adv)
{
  CopulaImplementation::load(adv);
  adv.loadAttribute( "distribution_", distribution_ );
  adv.loadAttribute( "marginalCollection_", marginalCollection_ );
}

END_NAMESPACE_OPENTURNS
