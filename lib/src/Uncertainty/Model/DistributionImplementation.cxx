//                                               -*- C++ -*-
/**
 *  @file  DistributionImplementation.cxx
 *  @brief Abstract top-level class for all distributions
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include <cmath>
#include <cstdlib>

#include "PersistentObjectFactory.hxx"
#include "DistributionImplementation.hxx"
#include "Distribution.hxx"
#include "Exception.hxx"
#include "Log.hxx"
#include "Lapack.hxx"
#include "IdentityMatrix.hxx"
#include "Collection.hxx"
#include "RandomGenerator.hxx"
#include "Normal.hxx"
#include "IndependentCopula.hxx"
#include "MarginalTransformationEvaluation.hxx"
#include "MarginalTransformationGradient.hxx"
#include "MarginalTransformationHessian.hxx"
#include "NatafIndependentCopulaEvaluation.hxx"
#include "NatafIndependentCopulaGradient.hxx"
#include "NatafIndependentCopulaHessian.hxx"
#include "InverseNatafIndependentCopulaEvaluation.hxx"
#include "InverseNatafIndependentCopulaGradient.hxx"
#include "InverseNatafIndependentCopulaHessian.hxx"
#include "RosenblattEvaluation.hxx"
#include "InverseRosenblattEvaluation.hxx"
#include "NumericalMathFunction.hxx"
#include "SklarCopula.hxx"
#include "SpecFunc.hxx"
#include "PlatformInfo.hxx"
#include "MethodBoundNumericalMathEvaluationImplementation.hxx"
#include "Curve.hxx"
#include "Staircase.hxx"
#include "Drawable.hxx"
#include "Graph.hxx"
#include "Brent.hxx"
#include "Bisection.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(DistributionImplementation);

typedef NumericalMathFunctionImplementation::EvaluationImplementation EvaluationImplementation;
typedef NumericalMathFunctionImplementation::GradientImplementation   GradientImplementation;
typedef NumericalMathFunctionImplementation::HessianImplementation    HessianImplementation;
typedef Collection<Distribution>                                      DistributionCollection;

static Factory<DistributionImplementation> RegisteredFactory("DistributionImplementation");

/* Default constructor */
DistributionImplementation::DistributionImplementation(const String & name)
  : PersistentObject(name),
    mean_(NumericalPoint(0)),
    covariance_(CovarianceMatrix(0)),
    gaussNodesAndWeights_(),
    integrationNodesNumber_(ResourceMap::GetAsUnsignedLong( "DistributionImplementation-DefaultIntegrationNodesNumber" )),
    isAlreadyComputedMean_(false),
    isAlreadyComputedCovariance_(false),
    isAlreadyComputedGaussNodesAndWeights_(false),
    pdfEpsilon_(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-DefaultPDFEpsilon" )),
    cdfEpsilon_(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-DefaultCDFEpsilon" )),
    quantileEpsilon_(),
    isAlreadyComputedStandardDistribution_(false),
    p_standardDistribution_(),
    isAlreadyInitializedQuantileCache_(false),
    scalarQuantileCache_(0, 2),
    isAlreadyCreatedGeneratingFunction_(false),
    generatingFunction_(0),
    dimension_(1),
    weight_(1.0),
    range_(),
    description_(1)
{
  description_[0] = "marginal 1";
}

/* Virtual constructor */
DistributionImplementation * DistributionImplementation::clone() const
{
  return new DistributionImplementation(*this);
}

/* Comparison operator */
Bool DistributionImplementation::operator ==(const DistributionImplementation & other) const
{
  if (this == &other) return true;
  return (dimension_ == other.dimension_) && (weight_ == other.weight_) && (range_ == other.range_);
}

/* String converter */
String DistributionImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << DistributionImplementation::GetClassName()
      << " description=" << description_;
  return oss;
}

/* String converter */
String DistributionImplementation::__str__(const String & offset) const
{
  return __repr__();
}


/* Weight accessor */
void DistributionImplementation::setWeight(const NumericalScalar w)
{
  weight_ = w;
}

/* Weight accessor */
NumericalScalar DistributionImplementation::getWeight() const
{
  return weight_;
}


/* Dimension accessor */
UnsignedLong DistributionImplementation::getDimension() const
{
  return dimension_;
}

/* Get the roughness, i.e. the L2-norm of the PDF */
NumericalScalar DistributionImplementation::getRoughness() const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::getRoughness() const";
}

/* Dimension accessor */
void DistributionImplementation::setDimension(const UnsignedLong dim)
{
  if (dim == 0) throw InvalidArgumentException(HERE) << "Dimension argument must be an integer >= 1, here dim = " << dim;
  if (dim != dimension_)
    {
      dimension_ = dim;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      isAlreadyComputedGaussNodesAndWeights_ = false;
      // Check if the current description is compatible with the new dimension
      if (description_.getSize() != dim)
        {
          description_ = Description(dim);
          for (UnsignedLong i = 0; i < dim; ++i) description_[i] = OSS() << "marginal " << i + 1;
        }
    }
}

/* Get one realization of the distributionImplementation */
NumericalPoint DistributionImplementation::getRealization() const
{
  // Use CDF inversion in the 1D case
  if (dimension_ == 1) return computeQuantile(RandomGenerator::Generate());
  NumericalPoint point(0);
  for (UnsignedLong i = 0; i < dimension_; ++i) point.add(computeConditionalQuantile(RandomGenerator::Generate(), point));
  return point;
}

/* Get a numerical sample whose elements follow the distributionImplementation */
NumericalSample DistributionImplementation::getSample(const UnsignedLong size) const
{
  NumericalSample returnSample(size, dimension_);
#pragma omp parallel for
  for (UnsignedLong i = 0; i < size; ++i)
    returnSample[i] = getRealization();
  returnSample.setName(getName());
  returnSample.setDescription(getDescription());
  return returnSample;
}

NumericalSample DistributionImplementation::getNumericalSample(const UnsignedLong size) const
{
  LOGUSER(OSS() << "In class Distribution, method getNumericalSample is deprecated in favor of getSample.");
  return getSample(size);
}

/* Get the DDF of the distributionImplementation */
NumericalPoint DistributionImplementation::computeDDF(const NumericalPoint & point) const
{
  NumericalPoint ddf(dimension_);
  const NumericalScalar cdfPoint(computeCDF(point));
  const NumericalScalar h(pow(cdfEpsilon_, 0.25));
  const NumericalScalar idenom(1.0 / sqrt(cdfEpsilon_));
  for (UnsignedLong i = 0; i < dimension_; ++i)
    {
      NumericalPoint epsilon(dimension_, 0.0);
      epsilon[i] = h;
      ddf[i] = (computeCDF(point + epsilon) - 2.0 * cdfPoint + computeCDF(point - epsilon)) * idenom;
    }
  return ddf;
}

/* Get the PDF of the distributionImplementation */
NumericalScalar DistributionImplementation::computePDF(const NumericalPoint & point) const
{
  const NumericalPoint epsilon(dimension_, pow(cdfEpsilon_, 1.0 / 3.0));
  pdfEpsilon_ = epsilon[0];
  // Centered finite differences of CDF
  return (computeCDF(point + epsilon) - computeCDF(point - epsilon)) / (2.0 * pdfEpsilon_);
}

NumericalScalar DistributionImplementation::computeLogPDF(const NumericalPoint & point) const
{
  NumericalScalar pdf = computePDF(point);
  NumericalScalar logPdf = -SpecFunc::MaxNumericalScalar;
  if ( pdf > 0.0 )
    {
      logPdf = log(pdf);
    }
  return logPdf;
}

/* Get the CDF of the distributionImplementation */
NumericalScalar DistributionImplementation::computeCDF(const NumericalPoint & point,
                                                       const Bool tail) const
{
  LOGUSER(OSS() << "In class Distribution, method computeCDF(x, tail) is deprecated.");
  return tail ? computeComplementaryCDF(point) : computeCDF(point);
}

NumericalScalar DistributionImplementation::computeCDF(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}

NumericalScalar DistributionImplementation::computeComplementaryCDF(const NumericalPoint & point) const
{
  return 0.5 + (0.5 - computeCDF(point));
}

/* Compute the probability content of an interval */
NumericalScalar DistributionImplementation::computeProbability(const Interval & interval) const
{
  if (dimension_ == 1)
    {
      // Empty interval
      if (interval.isNumericallyEmpty()) return 0.0;
      NumericalScalar upperCDF(1.0);
      NumericalScalar lowerCDF(0.0);
      if (interval.getFiniteLowerBound()[0]) lowerCDF = computeCDF(interval.getLowerBound());
      // Use the tail probability if the left endpoint of the interval is greater than the median, for an improved accuracy
      const Bool tail(lowerCDF > 0.5);
      if (tail) lowerCDF = computeComplementaryCDF(interval.getLowerBound());
      if (interval.getFiniteUpperBound()[0]) upperCDF = tail ? computeComplementaryCDF(interval.getUpperBound()) : computeCDF(interval.getUpperBound());
      // If we used the tail probability
      if (tail) return lowerCDF - upperCDF;
      // Classical case
      return upperCDF - lowerCDF;
    }
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::computeProbability()";
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex DistributionImplementation::computeCharacteristicFunction(const NumericalScalar x, const Bool logScale) const
{
  LOGUSER(OSS() << "In distribution, computeCharacteristicFunction(x, logScale) is deprecated.");
  return logScale ? computeLogCharacteristicFunction(x) : computeCharacteristicFunction(x);
}

NumericalComplex DistributionImplementation::computeCharacteristicFunction(const NumericalScalar x) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "Error:  cannot use the computeCharacteristicFunction method with distributions of dimension > 1";
  NumericalComplex value(0.0);
  // In the continuous case, we use simple gauss integration with a fixed number of integration points. We divide the interval in order to have a sufficient number of integration points by interval. It is neither accurate nor efficient. We should use some specific integration methods such as Filon's or Levin's methods.
  if (isContinuous())
    {
      // The circular function will have x(b-a)/2\pi arches over [a, b], so we need a number of points of this order, we decide to take 8 points per arch
      const NumericalSample legendreNodesAndWeights(getGaussNodesAndWeights());
      // How many sub-intervals?
      // nPts = 8*x(b-a)/2\pi => (b-a)/2 = nPts * \pi / (8*x)
      const NumericalScalar xMin(range_.getLowerBound()[0]);
      const NumericalScalar xMax(range_.getUpperBound()[0]);
      const NumericalScalar delta(xMax - xMin);
      const UnsignedLong intervalsNumber(std::max(1, static_cast<int>(round(2 * x * delta / integrationNodesNumber_))));
      // Here, we should adopt a specific method for the case of highly oscilating integrand, i.e. large x
      const NumericalScalar halfLength(0.5 * delta / intervalsNumber);
      for (UnsignedLong n = 0; n < intervalsNumber; ++n)
        {
          const NumericalScalar a(xMin + 2.0 * n * halfLength);
          for (UnsignedLong i = 0; i < integrationNodesNumber_; ++i)
            {
              const NumericalScalar xi(a + (1.0 + legendreNodesAndWeights[0][i]) * halfLength);
              value += legendreNodesAndWeights[1][i] * computePDF(xi) * exp(NumericalComplex(0.0, x * xi));
            }
        }
      // We factor out the scaling as all the sub intervals have the same length
      value *= halfLength;
    }
  else
    {
      // In the discrete case, we have a reasonably efficient algorithm both in term of speed and precision.
      if (isDiscrete())
        {
          const NumericalSample support(getSupport());
          const UnsignedLong size(support.getSize());
          for (UnsignedLong i = 0; i < size; ++i)
            {
              const NumericalScalar pt(support[i][0]);
              value += computePDF(pt) * exp(NumericalComplex(0.0, x * pt));
            }
        }
      // In the composite case, no default algorithm
      else
        {
          throw NotYetImplementedException(HERE) << "Error: no default algorithm to compute the characteristic function in the composite case.";
        }
    }
  return value;
}

NumericalComplex DistributionImplementation::computeLogCharacteristicFunction(const NumericalScalar x) const
{
  NumericalComplex value = computeCharacteristicFunction(x);
  NumericalComplex result = log(value);
  return result;
}

NumericalComplex DistributionImplementation::computeCharacteristicFunction(const UnsignedLong index,
                                                                           const NumericalScalar step,
                                                                           const Bool logScale) const
{
  return computeCharacteristicFunction(index * step);
}

/* Get the generating function of the distribution, i.e. psi(z) = E(z^X) */
NumericalScalar DistributionImplementation::computeGeneratingFunction(const NumericalScalar z,
                                                                      const Bool logScale) const
{
  LOGUSER(OSS() << "In distribution, computeGeneratingFunction(z, logScale) is deprecated.");
  return logScale ? computeLogGeneratingFunction(z) : computeGeneratingFunction(z);
}

NumericalScalar DistributionImplementation::computeGeneratingFunction(const NumericalScalar z) const
{
  return computeGeneratingFunction(NumericalComplex(z, 0.0)).real();
}

NumericalScalar DistributionImplementation::computeLogGeneratingFunction(const NumericalScalar z) const
{
  return computeLogGeneratingFunction(NumericalComplex(z, 0.0)).real();
}

NumericalComplex DistributionImplementation::computeGeneratingFunction(const NumericalComplex & z, const Bool logScale) const
{
  LOGUSER(OSS() << "In distribution, computeGeneratingFunction(z, logScale) is deprecated.");
  return logScale ? computeLogGeneratingFunction(z) : computeGeneratingFunction(z);
}

NumericalComplex DistributionImplementation::computeGeneratingFunction(const NumericalComplex & z) const
{
  if (getDimension() != 1) throw InvalidDimensionException(HERE) << "Error:  cannot use the computeCharacteristicFunction method with distributions of dimension > 1";
  if (!isDiscrete()) throw NotDefinedException(HERE) << "Error: cannot compute the generating function for non discrete distributions.";
  const NumericalSample support(getSupport());
  const UnsignedLong size(support.getSize());
  NumericalComplex value(0.0);
  // If the distribution is integral, the generating function is either a polynomial if the support is finite, or can be well approximated by such a polynomial
  if (isAlreadyCreatedGeneratingFunction_) value = generatingFunction_(z);
  else
    // If isIntegral, then we have to create the generating function as a polynomial
    if (isIntegral())
      {
        NumericalPoint coefficients(size);
        for (UnsignedLong i = 0; i < size; ++i) coefficients[i] = computePDF(support[i]);
        generatingFunction_ = UniVariatePolynomial(coefficients);
        isAlreadyCreatedGeneratingFunction_ = true;
        value = generatingFunction_(z);
      }
  // The distribution is discrete but not integral
    else
      {
        for (UnsignedLong i = 0; i < size; ++i)
          {
            const NumericalScalar pt(support[i][0]);
            value += computePDF(pt) * pow(z, pt);
          }
      }
  return value;
}

NumericalComplex DistributionImplementation::computeLogGeneratingFunction(const NumericalComplex & z) const
{
  NumericalComplex value = computeGeneratingFunction(z);
  return log(value);
}

/* Get the DDF of the distributionImplementation */
NumericalSample DistributionImplementation::computeDDF(const NumericalSample & inSample) const
{
  const UnsignedLong size(inSample.getSize());
  NumericalSample outSample(size, 1);
  for (UnsignedLong i = 0; i < size; ++i) outSample[i] = computeDDF(inSample[i]);
  return outSample;
}

/* Get the PDF of the distributionImplementation */
NumericalSample DistributionImplementation::computePDF(const NumericalSample & inSample) const
{
  const UnsignedLong size(inSample.getSize());
  NumericalSample outSample(size, 1);
  for (UnsignedLong i = 0; i < size; ++i) outSample[i] = NumericalPoint(1, computePDF(inSample[i]));
  return outSample;
}

NumericalSample DistributionImplementation::computeLogPDF(const NumericalSample & inSample) const
{
  const UnsignedLong size(inSample.getSize());
  NumericalSample outSample(size, 1);
  for (UnsignedLong i = 0; i < size; ++i) outSample[i] = NumericalPoint(1, computeLogPDF(inSample[i]));
  return outSample;
}

/* Get the CDF of the distributionImplementation */
NumericalSample DistributionImplementation::computeCDF(const NumericalSample & inSample,
                                                       const Bool tail) const
{
  LOGUSER(OSS() << "In class Distribution, method computeCDF(x, tail) is deprecated.");
  return tail ? computeComplementaryCDF(inSample) : computeCDF(inSample);
}

NumericalSample DistributionImplementation::computeCDF(const NumericalSample & inSample) const
{
  const UnsignedLong size(inSample.getSize());
  NumericalSample outSample(size, 1);
  for (UnsignedLong i = 0; i < size; ++i) outSample[i] = NumericalPoint(1, computeCDF(inSample[i]));
  return outSample;
}

NumericalSample DistributionImplementation::computeComplementaryCDF(const NumericalSample & inSample) const
{
  const UnsignedLong size(inSample.getSize());
  NumericalSample outSample(size, 1);
  for (UnsignedLong i = 0; i < size; ++i) outSample[i] = NumericalPoint(1, computeComplementaryCDF(inSample[i]));
  return outSample;
}

/* Get the DDF of the distributionImplementation */
NumericalScalar DistributionImplementation::computeDDF(const NumericalScalar scalar) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "ERROR: cannot use the simplified interface of computeDDF with distributions of dimension > 1";
  return computeDDF(NumericalPoint(1, scalar))[0];
}

/* Get the PDF of the distributionImplementation */
NumericalScalar DistributionImplementation::computePDF(const NumericalScalar scalar) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "ERROR: cannot use the simplified interface of computePDF with distributions of dimension > 1";
  return computePDF(NumericalPoint(1, scalar));
}

NumericalScalar DistributionImplementation::computeLogPDF(const NumericalScalar scalar) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "ERROR: cannot use the simplified interface of computeLogPDF with distributions of dimension > 1";
  return computeLogPDF(NumericalPoint(1, scalar));
}

/* Get the CDF of the distributionImplementation */
NumericalScalar DistributionImplementation::computeCDF(const NumericalScalar scalar,
                                                       const Bool tail) const
{
  LOGUSER(OSS() << "In class Distribution, method computeCDF(x, tail) is deprecated.");
  return tail ? computeComplementaryCDF(scalar) : computeCDF(scalar);
}

NumericalScalar DistributionImplementation::computeCDF(const NumericalScalar scalar) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "ERROR: cannot use the simplified interface of computeCDF with distributions of dimension > 1";
  return computeCDF(NumericalPoint(1, scalar));
}

NumericalScalar DistributionImplementation::computeComplementaryCDF(const NumericalScalar scalar) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "ERROR: cannot use the simplified interface of computeComplementaryCDF with distributions of dimension > 1";
  return computeComplementaryCDF(NumericalPoint(1, scalar));
}

/*  Compute the PDF of 1D distributions over a regular grid */
NumericalSample DistributionImplementation::computePDF(const NumericalScalar xMin,
                                                       const NumericalScalar xMax,
                                                       const UnsignedLong pointNumber,
                                                       const NumericalScalar precision) const
{
  if (dimension_ != 1) throw InvalidArgumentException(HERE) << "Error: cannot compute the PDF over a regular 1D grid if the dimension is > 1";
  NumericalSample result(pointNumber, 2);
  NumericalScalar x(xMin);
  const NumericalScalar step((xMax - xMin) / NumericalScalar(pointNumber - 1.0));
  for (UnsignedLong i = 0; i < pointNumber; ++i)
    {
      result[i][0] = x;
      result[i][1] = computePDF(x);
      x += step;
    }
  return result;
}

/*  Compute the CDF of 1D distributions over a regular grid */
NumericalSample DistributionImplementation::computeCDF(const NumericalScalar xMin,
                                                       const NumericalScalar xMax,
                                                       const UnsignedLong pointNumber,
                                                       const NumericalScalar precision) const
{
  if (dimension_ != 1) throw InvalidArgumentException(HERE) << "Error: cannot compute the CDF over a regular 1D grid if the dimension is > 1";
  NumericalSample result(pointNumber, 2);
  NumericalScalar x(xMin);
  NumericalScalar step((xMax - xMin) / NumericalScalar(pointNumber - 1.0));
  for (UnsignedLong i = 0; i < pointNumber; ++i)
    {
      result[i][0] = x;
      result[i][1] = computeCDF(x);
      x += step;
    }
  return result;
}

NumericalSample DistributionImplementation::computeComplementaryCDF(const NumericalScalar xMin,
                                                                    const NumericalScalar xMax,
                                                                    const UnsignedLong pointNumber,
                                                                    const NumericalScalar precision) const
{
  if (dimension_ != 1) throw InvalidArgumentException(HERE) << "Error: cannot compute the CDF over a regular 1D grid if the dimension is > 1";
  NumericalSample result(pointNumber, 2);
  NumericalScalar x(xMin);
  NumericalScalar step((xMax - xMin) / NumericalScalar(pointNumber - 1.0));
  for (UnsignedLong i = 0; i < pointNumber; ++i)
    {
      result[i][0] = x;
      result[i][1] = computeComplementaryCDF(x);
      x += step;
    }
  return result;
}
/*  Compute the quantile over a regular grid */
NumericalSample DistributionImplementation::computeQuantile(const NumericalScalar qMin,
                                                            const NumericalScalar qMax,
                                                            const UnsignedLong pointNumber,
                                                            const NumericalScalar precision,
                                                            const Bool tail) const
{
  if (getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot compute the quantile over a regular 1D grid if the dimension is > 1";
  NumericalSample result(pointNumber, 2);
  NumericalScalar q(qMin);
  const NumericalScalar step((qMax - qMin) / NumericalScalar(pointNumber - 1.0));
  for (UnsignedLong i = 0; i < pointNumber; ++i)
    {
      result[i][0] = q;
      result[i][1] = computeScalarQuantile(q, tail, precision);
      q += step;
    }
  return result;
}

/* Compute the quantile over a provided grid */
NumericalSample DistributionImplementation::computeQuantile(const NumericalPoint & prob,
                                                            const Bool tail) const
{
  const UnsignedLong size(prob.getSize());
  NumericalSample result(size, getDimension());
  for ( UnsignedLong i = 0; i < size; ++ i ) result[i] = computeQuantile(prob[i], tail);
  return result;
}

/* Get the PDF gradient of the distribution */
NumericalPoint DistributionImplementation::computePDFGradient(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::computePDFGradient()";
}

/* Get the CDF gradient of the distribution */
NumericalPoint DistributionImplementation::computeCDFGradient(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::computeCDFGradient()";
}


/* Compute the DDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar DistributionImplementation::computeConditionalDDF(const NumericalScalar x,
                                                                  const NumericalPoint & y) const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::computeConditionalDDF()";
}

/* Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar DistributionImplementation::computeConditionalPDF(const NumericalScalar x,
                                                                  const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional PDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return getMarginal(conditioningDimension)->computePDF(x);
  // General case
  Indices conditioning(conditioningDimension);
  for (UnsignedLong i = 0; i < conditioningDimension; ++i) conditioning[i] = i;
  Indices conditioned(conditioning);
  conditioned.add(conditioningDimension);
  const Implementation conditioningDistribution(getMarginal(conditioning));
  const NumericalScalar pdfConditioning(conditioningDistribution->computePDF(y));
  if (pdfConditioning <= 0.0) return 0.0;
  NumericalPoint z(y);
  z.add(x);
  const Implementation conditionedDistribution(getMarginal(conditioned));
  const NumericalScalar pdfConditioned(conditionedDistribution->computePDF(z));
  pdfEpsilon_ = conditionedDistribution->getPDFEpsilon() + conditioningDistribution->getPDFEpsilon();
  return pdfConditioned / pdfConditioning;
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar DistributionImplementation::computeConditionalCDF(const NumericalScalar x,
                                                                  const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional CDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return getMarginal(conditioningDimension)->computeCDF(x);
  // General case
  Indices conditioning(conditioningDimension);
  for (UnsignedLong i = 0; i < conditioningDimension; ++i) conditioning[i] = i;
  Indices conditioned(conditioning);
  conditioned.add(conditioningDimension);
  const Implementation conditioningDistribution(getMarginal(conditioning));
  const NumericalScalar pdfConditioning(conditioningDistribution->computePDF(y));
  if (pdfConditioning <= 0.0) return 0.0;
  const Implementation conditionedDistribution(getMarginal(conditioned));
  const NumericalScalar xMin(conditionedDistribution->getRange().getLowerBound()[conditioningDimension]);
  if (x <= xMin) return 0.0;
  const NumericalScalar xMax(conditionedDistribution->getRange().getUpperBound()[conditioningDimension]);
  if (x >= xMax) return 1.0;
  // Numerical integration with respect to x
  NumericalPoint z(y);
  z.add(x);
  pdfEpsilon_ = conditionedDistribution->getPDFEpsilon() + conditioningDistribution->getPDFEpsilon();
  const NumericalSample legendreNodesAndWeights(getGaussNodesAndWeights());
  const NumericalScalar halfLength(0.5 * (x - xMin));
  cdfEpsilon_ = conditioningDistribution->getPDFEpsilon();
  NumericalScalar value(0.0);
  for (UnsignedLong i = 0; i < integrationNodesNumber_; ++i)
    {
      const NumericalScalar xi(xMin + (1.0 + legendreNodesAndWeights[0][i]) * halfLength);
      z[conditioningDimension] = xi;
      value += legendreNodesAndWeights[1][i] * conditionedDistribution->computePDF(z);
      cdfEpsilon_ += legendreNodesAndWeights[1][i] * conditionedDistribution->getPDFEpsilon();
    }
  value *= (halfLength / pdfConditioning);
  return value;
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) with reuse of expansive data */
NumericalScalar DistributionImplementation::computeConditionalCDFForQuantile(const NumericalScalar x,
                                                                             const NumericalPoint & y,
                                                                             const Implementation & conditioningDistribution,
                                                                             const Implementation & conditionedDistribution,
                                                                             const NumericalScalar xMin) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  const NumericalScalar pdfConditioning(conditioningDistribution->computePDF(y));
  if (pdfConditioning <= 0.0) return 0.0;
  // Numerical integration with respect to x
  NumericalPoint z(y);
  z.add(x);
  pdfEpsilon_ = conditionedDistribution->getPDFEpsilon() + conditioningDistribution->getPDFEpsilon();
  const NumericalSample legendreNodesAndWeights(getGaussNodesAndWeights());
  const NumericalScalar halfLength(0.5 * (x - xMin));
  cdfEpsilon_ = conditioningDistribution->getPDFEpsilon();
  NumericalScalar value(0.0);
  for (UnsignedLong i = 0; i < integrationNodesNumber_; ++i)
    {
      const NumericalScalar xi(xMin + (1.0 + legendreNodesAndWeights[0][i]) * halfLength);
      z[conditioningDimension] = xi;
      value += legendreNodesAndWeights[1][i] * conditionedDistribution->computePDF(z);
      cdfEpsilon_ += legendreNodesAndWeights[1][i] * conditionedDistribution->getPDFEpsilon();
    }
  value *= (halfLength / pdfConditioning);
  return value;
}

/* Compute the PDF and CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar DistributionImplementation::computeConditionalPDFAndCDF(const NumericalScalar x,
                                                                        const NumericalPoint & y,
                                                                        NumericalScalar & cdf,
                                                                        const Implementation & conditioningDistribution,
                                                                        const Implementation & conditionedDistribution,
                                                                        const NumericalScalar xMin) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  const NumericalScalar pdfConditioning(conditioningDistribution->computePDF(y));
  if (pdfConditioning <= 0.0)
    {
      cdf = 0.0;
      return 0.0;
    }
  // Numerical integration with respect to x
  NumericalPoint z(y);
  z.add(x);
  const NumericalScalar pdfConditioned(conditionedDistribution->computePDF(z));
  cdf = 0.0;
  const NumericalSample legendreNodesAndWeights(getGaussNodesAndWeights());
  const NumericalScalar halfLength(0.5 * (x - xMin));
  cdfEpsilon_ = conditioningDistribution->getPDFEpsilon();
  for (UnsignedLong i = 0; i < integrationNodesNumber_; ++i)
    {
      const NumericalScalar xi(xMin + (1.0 + legendreNodesAndWeights[0][i]) * halfLength);
      z[conditioningDimension] = xi;
      cdf += legendreNodesAndWeights[1][i] * conditionedDistribution->computePDF(z);
      cdfEpsilon_ += legendreNodesAndWeights[1][i] * conditionedDistribution->getPDFEpsilon();
    }
  cdf *= (halfLength / pdfConditioning);
  return pdfConditioned / pdfConditioning;
}

/* Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
NumericalScalar DistributionImplementation::computeConditionalQuantile(const NumericalScalar q,
                                                                       const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= dimension_) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile with a conditioning point of dimension greater or equal to the distribution dimension.";
  if ((q < 0.0) || (q > 1.0)) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile for a probability level outside of [0, 1]";
  // Initialize the conditional quantile with the quantile of the i-th marginal distribution
  const Implementation marginalDistribution(getMarginal(conditioningDimension));
  NumericalScalar quantile(marginalDistribution->computeQuantile(q)[0]);
  // Special case for bording values
  if ((q == 0.0) || (q == 1.0)) return quantile;
  // Special case when no contitioning or independent copula
  if ((conditioningDimension == 0) || hasIndependentCopula()) return quantile;
  //NumericalScalar step(marginalDistribution->getDispersionIndicator());
  Indices conditioning(conditioningDimension);
  for (UnsignedLong i = 0; i < conditioningDimension; ++i) conditioning[i] = i;
  Indices conditioned(conditioning);
  conditioned.add(conditioningDimension);
  const Implementation conditioningDistribution(getMarginal(conditioning));
  const Implementation conditionedDistribution(getMarginal(conditioned));
  const NumericalScalar xMin(conditionedDistribution->getRange().getLowerBound()[conditioningDimension]);
  const NumericalScalar xMax(conditionedDistribution->getRange().getUpperBound()[conditioningDimension]);
  // Start with the largest bracketing interval
  NumericalScalar a(xMin);
  NumericalScalar b(xMax);
  Bool convergence(false);
  NumericalScalar residual(0.0);
  UnsignedLong iteration(0);
  UnsignedLong maximumIteration(ResourceMap::GetAsUnsignedLong("DistributionImplementation-DefaultQuantileIteration"));
  NumericalScalar quantileEpsilon(ResourceMap::GetAsUnsignedLong("DistributionImplementation-DefaultQuantileEpsilon"));
  while (!convergence && (iteration < maximumIteration))
    {
      NumericalScalar cdf(0.0);
      const NumericalScalar pdf(computeConditionalPDFAndCDF(quantile, y, cdf, conditioningDistribution, conditionedDistribution, xMin));
      // Do we have to perform a bisection step?
      if ((pdf == 0.0) || (quantile > b) || (quantile < a))
        {
          quantile = 0.5 * (a + b);
          cdf = computeConditionalCDFForQuantile(quantile, y, conditioningDistribution, conditionedDistribution, xMin);
          if (cdf > q) b = quantile;
          else a = quantile;
        }
      else
        {
          // No, so do a Newton step
          residual = (q - cdf) / pdf;
          quantile += residual;
        }
      convergence = fabs(residual) < quantileEpsilon * (1.0 + fabs(quantile)) || (fabs(cdf - q) < 2.0 * cdfEpsilon_) || (b - a < quantileEpsilon * (1.0 + fabs(quantile)));
      ++iteration;
    }
  return quantile;
}

/* Scalar quantile cache initialization */
void DistributionImplementation::initializeQuantileCache() const
{
  const UnsignedLong size(ResourceMap::GetAsUnsignedLong("DistributionImplementation-DefaultQuantileCacheSize"));
  // Fill the cache only if its size is at least 2
  if (size > 1)
    {
      scalarQuantileCache_ = NumericalSample(size, 2);
      // Compute the lower bound by sequential search then by bisection
      NumericalScalar xMin(0.0);
      NumericalScalar step(1.0);
      // Go backward until the CDF becomes small
      NumericalScalar cdf(computeCDF(xMin));
      while (cdf > cdfEpsilon_)
        {
          LOGDEBUG(OSS() << "lower bound, xMin=" << xMin << ", step=" << step << ", cdf=" << cdf);
          xMin -= step;
          step *= 2;
	  cdf = computeCDF(xMin);
        }
      // If no progress, go forward
      if (xMin == 0.0)
        {
          // Go forward until the CDF becomes significant
          xMin += step;
          step *= 2;
	  cdf = computeCDF(xMin);
          while (cdf < cdfEpsilon_)
            {
              LOGDEBUG(OSS() << "lower bound, xMin=" << xMin << ", step=" << step << ", cdf=" << cdf);
              xMin += step;
              step *= 2;
	      cdf = computeCDF(xMin);
            }
        }
      // Else we know that the previous step was before the lower bound
      else xMin += 0.5 * step;
      // Numerical fixed point iteration
      while (step + xMin != xMin)
        {
          LOGDEBUG(OSS() << "lower bound, xMin=" << xMin << ", step=" << step << ", cdf=" << cdf);
          step *= 0.5;
          xMin -= step;
	  cdf = computeCDF(xMin);
          if (cdf < cdfEpsilon_) xMin += step;
        }
      scalarQuantileCache_[0][0] = 0.0;
      scalarQuantileCache_[0][1] = xMin;
      // Compute the upper bound by sequential search then by bisection
      NumericalScalar xMax(0.0);
      step = 1.0;
      // Go forward until the tail CDF becomes close to 0
      NumericalScalar ccdf(computeComplementaryCDF(xMax));
      while (ccdf >= cdfEpsilon_)
        {
          LOGDEBUG(OSS() << "upper bound, xMin=" << xMin << ", step=" << step << ", ccdf=" << ccdf);
          xMax += step;
          step *= 2;
	  ccdf = computeComplementaryCDF(xMax);
        }
      // If no progress, go backward
      if (xMax == 0.0)
        {
          // Go backward until the tail CDF becomes significantly greater than 0
          xMax -= step;
          step *= 2;
	  ccdf = computeComplementaryCDF(xMax);
          while (ccdf < cdfEpsilon_)
            {
              LOGDEBUG(OSS() << "upper bound, xMin=" << xMin << ", step=" << step << ", ccdf=" << ccdf);
              xMax -= step;
              step *= 2;
	      ccdf = computeComplementaryCDF(xMax);
            }
        }
      // Else we know that the previous step was after the upper bound
      else xMax -= 0.5 * step;
      // Numerical fixed point iteration
      while (step + xMax != xMax)
        {
          LOGDEBUG(OSS() << "upper bound, xMin=" << xMin << ", step=" << step << ", ccdf=" << ccdf);
          step *= 0.5;
          xMax += step;
	  ccdf = computeComplementaryCDF(xMax);
          if (ccdf < cdfEpsilon_) xMax -= step;
        }
      scalarQuantileCache_[size - 1][0] = 1.0;
      scalarQuantileCache_[size - 1][1] = xMax;
      for (UnsignedLong i = 1; i < size - 1; ++i)
        {
          const NumericalScalar x(xMin + i * (xMax - xMin) / (size - 1));
          scalarQuantileCache_[i][0] = computeCDF(x);
          scalarQuantileCache_[i][1] = x;
        }
      LOGDEBUG(OSS() << "quantile cache=" << scalarQuantileCache_);
      isAlreadyInitializedQuantileCache_ = true;
    } // size > 1
}

/* Quantile computation for dimension=1 */
NumericalScalar DistributionImplementation::computeScalarQuantile(const NumericalScalar prob,
                                                                  const Bool tail,
                                                                  const NumericalScalar precision) const
{
  // Cache initialization by bisection
  if (!isAlreadyInitializedQuantileCache_) initializeQuantileCache();
  NumericalScalar quantileEpsilon(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon"));
  LOGDEBUG(OSS() << "DistributionImplementation::computeScalarQuantile: prob=" << prob << " tail=" << (tail ? "true" : "false") << " precision=" << precision);
  if (getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: the method computeScalarQuantile is only defined for 1D distributions";
  if ((prob < -quantileEpsilon) || (prob > 1.0 + quantileEpsilon)) throw InvalidArgumentException(HERE) << "Error: cannot compute a quantile for a probability level outside of [0, 1]";
  // Use the cache to obtain a bracketing interval
  // As only a coarse value is looked for, we can use the complementary to 1 as the needed probability value
  NumericalScalar p(tail ? 1.0 - prob : prob);
  UnsignedLong currentIndex(static_cast<UnsignedLong>(floor(p * scalarQuantileCache_.getSize())));
  NumericalScalar currentCDF(scalarQuantileCache_[currentIndex][0]);
  LOGDEBUG(OSS() << "DistributionImplementation::computeScalarQuantile:p=" << p << ", currentIndex=" << currentIndex << ", currentCDF=" << currentCDF);
  // Search forward for an upper bound. The loop must end with currentIndex < size of scalarQuantileCache_
  // because the right endpoint of the cache is the upper bound of the range, i.e. with CDF == 1
  while (p > currentCDF)
    {
      ++currentIndex;
      currentCDF = scalarQuantileCache_[currentIndex][0];
      LOGDEBUG(OSS() << "DistributionImplementation::computeScalarQuantile: p=" << p << ", currentIndex=" << currentIndex << ", currentCDF=" << currentCDF);
    }
  // Here, the current index is > 0 and is such that prob <= currentCDF
  // Check that it is the tightest upper bound by looking backward for a lower bound. The loop must end
  // with currentIndex >= 0 because the right endpoint of the cache is the upper bound of the range,
  // i.e. with CDF == 1
  while (currentCDF >= p)
    {
      --currentIndex;
      currentCDF = scalarQuantileCache_[currentIndex][0];
    }
  // Here, we know that currentIndex is such that the needed quantile is bracketed by the
  // values at currentIndex and currentIndex+1
  NumericalScalar a(scalarQuantileCache_[currentIndex][1]);
  NumericalScalar aCDF(currentCDF);
  // If we have found a very good initial guess
  if (fabs(aCDF - prob) < 2.0 * cdfEpsilon_) return a;
  // Go to the upper bound
  ++currentIndex;
  NumericalScalar b(scalarQuantileCache_[currentIndex][1]);
  NumericalScalar bCDF(scalarQuantileCache_[currentIndex][0]);
  LOGDEBUG(OSS() << "DistributionImplementation::computeScalarQuantile: a=" << a << " b=" << b);
  // First, we try the Newton method with an initial guess obtained using a linear approximation
  NumericalScalar quantile(a + (prob - aCDF) * (b - a) / (bCDF - aCDF));
  Bool convergence(false);
  NumericalScalar residual(0.0);
  UnsignedLong iteration(0);
  Bool isNewtonAccepted(true);
  NumericalScalar cdf(0.0);
  UnsignedLong maximumIteration(ResourceMap::GetAsUnsignedLong( "DistributionImplementation-DefaultQuantileIteration" ));
  while (!convergence)
    {
      ++iteration;
      const NumericalScalar oldQuantile(quantile);
      // If we have to perform a Newton step, either because the last iteration was a successful Newton step or because it was a Bisection step
      if (isNewtonAccepted)
        {
          LOGDEBUG(OSS() << "Newton step");
          const NumericalScalar pdf(tail ? -computePDF(quantile) : computePDF(quantile));
          // f pdf is zero, stop Newton iteration
          if (pdf == 0.0)
            {
              convergence = false;
              isNewtonAccepted = false;
            }
          else
            {
              cdf = tail ? computeComplementaryCDF(quantile) : computeCDF(quantile);
              residual = (prob - cdf) / pdf;
              quantile += residual;
              LOGDEBUG(OSS() << "DistributionImplementation::computeScalarQuantile: cdf=" << cdf << ", residual=" << residual << ", quantile=" << quantile);
              // Safety stop if the iteration seems to diverge
              if ((quantile > b) || (quantile < a))
                {
                  isNewtonAccepted = false;
                  quantile -= residual;
                  convergence = false;
                }
              else
                {
                  // Adapt the bracketing interval
                  if (cdf > prob) b = oldQuantile;
                  else a = oldQuantile;
                  convergence = fabs(residual) < precision * (1.0 + fabs(quantile)) || (fabs(cdf - prob) < 2.0 * precision);
                  isNewtonAccepted = iteration < maximumIteration;
                }
            }
        }
      else
        {
          LOGDEBUG(OSS() << "DistributionImplementation::computeScalarQuantile: Bisection step");
          quantile = 0.5 * (a + b);
          cdf = tail ? computeComplementaryCDF(quantile) : computeCDF(quantile);
          if (cdf > prob) b = quantile;
          else a = quantile;
          convergence = (b - a < precision * (1.0 + fabs(quantile))) || (fabs(cdf - prob) < 2.0 * precision);
          isNewtonAccepted = iteration < maximumIteration;
        }
      LOGDEBUG(OSS() << "DistributionImplementation::computeScalarQuantile: quantile=" << quantile);
    }
  return quantile;
} // computeScalarQuantile

/* Generic implementation of the quantile computation */
NumericalPoint DistributionImplementation::computeQuantile(const NumericalScalar prob,
                                                           const Bool tail) const
{
  LOGDEBUG(OSS() << "DistributionImplementation::computeQuantile: prob=" << prob << ", tail=" << (tail ? "true" : "false"));
  NumericalScalar quantileEpsilon(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon"));
  if (prob < -quantileEpsilon || prob > 1.0 + quantileEpsilon) throw InvalidArgumentException(HERE) << "Error: cannot compute a quantile for a probability level outside of [0, 1]";
  // Special case for bording values
  const NumericalScalar q(tail ? 1.0 - prob : prob);
  if (q <= 0.0) return range_.getLowerBound();
  if (q >= 1.0) return range_.getUpperBound();
  // Special case for dimension 1
  if (dimension_ == 1)
    {
      const NumericalScalar xQ(computeScalarQuantile(prob, tail));
      LOGDEBUG(OSS() << "DistributionImplementation::computeQuantile: dimension=1, q=" << q << ", xQ=" << xQ);
      return NumericalPoint(1, xQ);
    }
  // Extract the marginal distributions
  Collection<Implementation> marginals(dimension_);
  for (UnsignedLong i = 0; i < dimension_; i++) marginals[i] = getMarginal(i);
  // The n-D quantile is defined as X(\tau) = (F_1^{-1}(\tau), ..., F_n^{-1}(\tau)),
  // with tau such as F(X(\tau)) = q.
  // As F(x) = C(F_1(x_1),...,F_n(x_n)), the constraint F(X(\tau)) = q reads:
  // C(\tau,...,\tau) = q
  // Bracketing of \tau using the Frechet Hoeffding bounds:
  // max(n\tau - n + 1, 0) <= C(\tau,...,\tau) <= \tau
  // from which we deduce that q <= \tau and \tau <= 1 - (1 - q) / n
  // Lower bound of the bracketing interval
  QuantileWrapper wrapper(marginals, this);
  const NumericalMathFunction f(bindMethod<QuantileWrapper, NumericalPoint, NumericalPoint>(wrapper, &QuantileWrapper::computeDiagonal, 1, 1));
  NumericalScalar leftTau(q);
  NumericalScalar leftCDF(f(NumericalPoint(1, leftTau))[0]);
  // Due to numerical precision issues, the theoretical bound can be slightly violated
  if (leftCDF > prob)
    {
      leftTau = 0.0;
      leftCDF = f(NumericalPoint(1, leftTau))[0];
    }
  // Upper bound of the bracketing interval
  NumericalScalar rightTau(1.0 - (1.0 - q) / dimension_);
  NumericalScalar rightCDF(f(NumericalPoint(1, rightTau))[0]);
  // Due to numerical precision issues, the theoretical bound can be slightly violated
  if (rightCDF < prob)
    {
      rightTau = 1.0;
      rightCDF = f(NumericalPoint(1, rightTau))[0];
    }
  LOGDEBUG(OSS() << "DistributionImplementation::computeQuantile: dimension=" << dimension_ << ", q=" << q << ", leftTau=" << leftTau << ", leftCDF=" << leftCDF << ", rightTau=" << rightTau << ", rightCDF=" << rightCDF);
  // Use Brent's method to compute the quantile efficiently for continuous distributions
  if (isContinuous())
    {
      LOGINFO("Brent solver");
      Brent solver(cdfEpsilon_, cdfEpsilon_, cdfEpsilon_, ResourceMap::GetAsUnsignedLong( "DistributionImplementation-DefaultQuantileIteration" ));
      const NumericalScalar tau(solver.solve(f, q, leftTau, rightTau, leftCDF, rightCDF));
      LOGINFO(OSS(false) << "tau=" << tau);
      return wrapper.diagonalToSpace(tau);
    }
  else
    {
      LOGINFO("Bisection solver");
      Bisection solver(cdfEpsilon_, cdfEpsilon_, cdfEpsilon_, ResourceMap::GetAsUnsignedLong( "DistributionImplementation-DefaultQuantileIteration" ));
      const NumericalScalar tau(solver.solve(f, q, leftTau, rightTau, leftCDF, rightCDF));
      LOGINFO(OSS(false) << "tau=" << tau);
      // Continuity correction
      return wrapper.diagonalToSpace(tau * (1.0 + 2.0 * cdfEpsilon_) + 2.0 * cdfEpsilon_);
    }
}

/* Get the mathematical and numerical range of the distribution.
   Its mathematical range is the smallest closed interval outside
   of which the PDF is zero, and the numerical range is the interval
   outside of which the PDF is rounded to zero in double precision */
Interval DistributionImplementation::getRange() const
{
  return range_;
}

void DistributionImplementation::setRange(const Interval & range)
{
  if (range.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given range has a dimension incompatible with the dimension of the distribution.";
  range_ = range;
}

/* Compute the numerical range of the distribution given the parameters values */
void DistributionImplementation::computeRange()
{
  const UnsignedLong dimension(getDimension());
  const Interval::BoolCollection finiteLowerBound(dimension, false);
  const Interval::BoolCollection finiteUpperBound(dimension, false);
  setRange(Interval(computeLowerBound(), computeUpperBound(), finiteLowerBound, finiteUpperBound));
}

/* Compute the lower bound of the range */
NumericalPoint DistributionImplementation::computeLowerBound() const
{
  const UnsignedLong dimension(getDimension());
  // For a multivariate distribution, the range is the axes aligned box that fits to the marginal ranges
  NumericalPoint lowerBound(dimension);
  // Here, we must separate the 1D case from the nD case as the getMarginal() method is generic for 1D case and
  // would involve a circular call to computeRange()
  if (dimension == 1) lowerBound[0] = computeScalarQuantile(cdfEpsilon_);
  else for (UnsignedLong i = 0; i < dimension; ++i) lowerBound[i] = getMarginal(i)->computeScalarQuantile(cdfEpsilon_);
  return lowerBound;
}

/* Compute the upper bound of the range */
NumericalPoint DistributionImplementation::computeUpperBound() const
{
  const UnsignedLong dimension(getDimension());
  // For a multivariate distribution, the range is the axes aligned box that fits to the marginal ranges
  NumericalPoint upperBound(dimension);
  if (dimension == 1) upperBound[0] = computeScalarQuantile(cdfEpsilon_, true);
  else for (UnsignedLong i = 0; i < dimension; ++i) upperBound[i] = getMarginal(i)->computeScalarQuantile(cdfEpsilon_, true);
  return upperBound;
}

/* Compute the mean of the distribution */
void DistributionImplementation::computeMean() const
{
  mean_ = computeShiftedMoment(1, NumericalPoint(getDimension(), 0.0));
  isAlreadyComputedMean_ = true;
}

/* Get the mean of the distribution */
NumericalPoint DistributionImplementation::getMean() const
{
  if (!isAlreadyComputedMean_) computeMean();
  return mean_;
}

/* Get the standard deviation of the distribution */
NumericalPoint DistributionImplementation::getStandardDeviation() const
{
  const UnsignedLong dimension(getDimension());
  const NumericalPoint variance(getCenteredMoment(2));
  NumericalPoint result(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i) result[i] = sqrt(variance[i]);
  return result;
}

/* Get the skewness of the distribution */
NumericalPoint DistributionImplementation::getSkewness() const
{
  const UnsignedLong dimension(getDimension());
  const NumericalPoint variance(getCenteredMoment(2));
  const NumericalPoint thirdMoment(getCenteredMoment(3));
  NumericalPoint result(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i) result[i] = thirdMoment[i] / pow(variance[i], 1.5);
  return result;
}

/* Get the kurtosis of the distribution */
NumericalPoint DistributionImplementation::getKurtosis() const
{
  const UnsignedLong dimension(getDimension());
  const NumericalPoint variance(getCenteredMoment(2));
  const NumericalPoint fourthMoment(getCenteredMoment(4));
  NumericalPoint result(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i) result[i] = fourthMoment[i] / pow(variance[i], 2.0);
  return result;
}

/* Get the moments of the distribution */
NumericalPoint DistributionImplementation::getMoment(const UnsignedLong n) const
{
  const UnsignedLong dimension(getDimension());
  if (n == 0) return NumericalPoint(dimension, 1.0);
  return computeShiftedMoment(n, NumericalPoint(dimension, 0.0));
}

/* Get the centered moments of the distribution */
NumericalPoint DistributionImplementation::getCenteredMoment(const UnsignedLong n) const
{
  const UnsignedLong dimension(getDimension());
  if (n == 0) throw InvalidArgumentException(HERE) << "Error: the centered moments of order 0 are undefined.";
  if (n == 1) return NumericalPoint(dimension, 0.0);
  return computeShiftedMoment(n, getMean());
}

/* Compute the covariance of the distribution */
void DistributionImplementation::computeCovariance() const
{
  const UnsignedLong dimension(getDimension());
  // We need this to initialize the covariance matrix in two cases:
  // + this is the first call to this routine (which could be checked by testing the dimension of the distribution and the dimension of the matrix
  // + the copula has changed from a non-independent one to the independent copula
  covariance_ = CovarianceMatrix(dimension);
  // First the diagonal terms, which are the marginal covariances
  // To ensure that the mean is up to date
  mean_ = getMean();
  // Get the standard deviation
  const NumericalPoint standardDeviation(getStandardDeviation());
  for(UnsignedLong component = 0; component < dimension; ++component) covariance_(component, component) = standardDeviation[component] * standardDeviation[component];
  // Off-diagonal terms if the copula is not the independent copula
  if (!hasIndependentCopula())
    {
      const NumericalScalar delta(2.0);
      Indices indices(2);
      const int N(8 * 2 * 2 * 2 * 2 * 2);
      const NumericalScalar h(0.5 / 2 / 2 / 2 / 2 / 2);
      for(UnsignedLong rowIndex = 0; rowIndex < dimension; ++rowIndex)
        {
          indices[0] = rowIndex;
          const Implementation marginalI(getMarginal(rowIndex));
          const NumericalScalar mi(marginalI->computeQuantile(0.5)[0]);
          const NumericalScalar di(marginalI->computeQuantile(0.75)[0] - marginalI->computeQuantile(0.25)[0]);
          for(UnsignedLong columnIndex = rowIndex + 1; columnIndex < dimension; ++columnIndex)
            {
              indices[1] = columnIndex;
              const Implementation marginalDistribution(getMarginal(indices));
              if (!marginalDistribution->hasIndependentCopula())
                {
                  const Implementation marginalJ(getMarginal(columnIndex));
                  const NumericalScalar mj(marginalJ->computeQuantile(0.5)[0]);
                  const NumericalScalar dj(marginalJ->computeQuantile(0.75)[0] - marginalJ->computeQuantile(0.25)[0]);
                  NumericalPoint xij(2);
                  xij[0] = mi;
                  xij[1] = mj;
                  NumericalScalar covarianceIJ(0.0);
                  // Then we loop over the integration points
                  for(int rowNodeIndex = -N; rowNodeIndex < N + 1; ++rowNodeIndex)
                    {
                      const NumericalScalar hi(h * rowNodeIndex);
                      const NumericalScalar expHi(exp(hi));
                      const NumericalScalar iexpHi(1.0 / expHi);
                      const NumericalScalar sinhHi(0.5 * (expHi - iexpHi));
                      const NumericalScalar expSinhHi(exp(sinhHi));
                      const NumericalScalar iexpSinhHi(1.0 / expSinhHi);
                      const NumericalScalar iTwoCoshSinhHi(1.0 / (expSinhHi + iexpSinhHi));
                      const NumericalScalar xip(mi + expSinhHi * iTwoCoshSinhHi * di * delta);
                      const NumericalScalar wi((expHi + iexpHi) * iTwoCoshSinhHi * iTwoCoshSinhHi);
                      const NumericalScalar cdfip(marginalI->computeCDF(xip));
                      for(int columnNodeIndex = -N; columnNodeIndex < N + 1; ++columnNodeIndex)
                        {
                          const NumericalScalar hj(h * columnNodeIndex);
                          const NumericalScalar expHj(exp(hj));
                          const NumericalScalar iexpHj(1.0 / expHj);
                          const NumericalScalar sinhHj(0.5 * (expHj - iexpHj));
                          const NumericalScalar expSinhHj(exp(sinhHj));
                          const NumericalScalar iexpSinhHj(1.0 / expSinhHj);
                          const NumericalScalar iTwoCoshSinhHj(1.0 / (expSinhHj + iexpSinhHj));
                          const NumericalScalar xjp(mj + expSinhHj * iTwoCoshSinhHj * dj * delta);
                          const NumericalScalar wj((expHj + iexpHj) * iTwoCoshSinhHj * iTwoCoshSinhHj);
                          const NumericalScalar cdfjp(marginalJ->computeCDF(xjp));
                          NumericalPoint inpp(2);
                          inpp[0] = xip;
                          inpp[1] = xjp;
                          covarianceIJ += delta * delta * di * dj * h * h * wi * wj * (marginalDistribution->computeCDF(inpp) - cdfip * cdfjp);
                        } // loop over J integration nodes
                    } // loop over I integration nodes
                  covariance_(rowIndex, columnIndex) = covarianceIJ;
                }
            } // loop over column indices
        } // loop over row indices
    } // if !hasIndependentCopula
  isAlreadyComputedCovariance_ = true;
} // computeCovariance

/* Get the covariance of the distribution */
CovarianceMatrix DistributionImplementation::getCovariance() const
{
  if (!isAlreadyComputedCovariance_) computeCovariance();
  return covariance_;
}

/* Correlation matrix accessor */
CorrelationMatrix DistributionImplementation::getCorrelation() const
{
  // To make sure the covariance is up to date
  covariance_ = getCovariance();
  CorrelationMatrix R(dimension_);
  NumericalPoint sigma(dimension_);
  for (UnsignedLong i = 0; i < dimension_; ++i)
    {
      const NumericalScalar sigmaI(sqrt(covariance_(i, i)));
      sigma[i] = sigmaI;
      for (UnsignedLong j = 0; j < i; ++j) R(i, j) = covariance_(i, j) / (sigmaI * sigma[j]);
    }
  return R;
}

/* Cholesky factor of the correlation matrix accessor */
SquareMatrix DistributionImplementation::getCholesky() const
{
  covariance_ = getCovariance();
  return covariance_.computeCholesky();
}

/* Inverse of the Cholesky factor of the correlation matrix accessor */
SquareMatrix DistributionImplementation::getInverseCholesky() const
{
  // Compute its Cholesky factor
  SquareMatrix cholesky(getCholesky());
  // Inversion of the Cholesky factor using the dtrsm blas level 3 routine
  // side tells if we solve M.X = alpha.B or X.M = alpha.B
  char side('L');
  int lside(1);
  // M must be triangular. uplo tells if it is upper or lower triangular
  char uplo('L');
  int luplo(1);
  // transa tells if M is transposed or not
  char transa('N');
  int ltransa(1);
  // diag tells if M is unit diagonal or not
  char diag('N');
  int ldiag(1);
  // the row dimension of M
  int m(dimension_);
  // the column dimension of M
  int n(dimension_);
  // we solve the case alpha=1
  double alpha(1.0);
  // leading dimension of M
  int lda(dimension_);
  // leading dimension of B
  int ldb(dimension_);
  // As we want to inverse M, we set B = Id
  SquareMatrix inverseCholesky = IdentityMatrix(dimension_);
  // B stores the result of the routine
  DTRSM_F77(&side, &uplo, &transa, &diag, &m, &n, &alpha, const_cast<double*>(&((*cholesky.getImplementation())[0])), &lda, const_cast<double*>(&((*inverseCholesky.getImplementation())[0])), &ldb, &lside, &luplo, &ltransa, &ldiag);
  return inverseCholesky;
}

/* Compute the nodes and weights for a 1D gauss quadrature over [-1, 1] with respect to the Lebesgue measure */
void DistributionImplementation::computeGaussNodesAndWeights() const
{
  int integrationNodesNumber(integrationNodesNumber_);
  gaussNodesAndWeights_ = NumericalSample(2, integrationNodesNumber);
  // First, build a symmetric tridiagonal matrix whose eigenvalues are the nodes of the
  // gauss integration rule
  char jobz('V');
  int ljobz(1);
  NumericalPoint d(integrationNodesNumber);
  NumericalPoint e(integrationNodesNumber);
  for (UnsignedLong i = 1; i < static_cast<UnsignedLong>(integrationNodesNumber); ++i) e[i - 1] = 0.5 / sqrt(1.0 - pow(2.0 * i, -2));
  int ldz(integrationNodesNumber);
  SquareMatrix z(integrationNodesNumber);
  NumericalPoint work(2 * integrationNodesNumber - 2);
  int info;
  DSTEV_F77(&jobz, &integrationNodesNumber, &d[0], &e[0], &z(0, 0), &ldz, &work[0], &info, &ljobz);
  if (info != 0) throw InternalException(HERE) << "Lapack DSTEV: error code=" << info;
  for (UnsignedLong i = 0; i < static_cast<UnsignedLong>(integrationNodesNumber); ++i)
    {
      // Nodes
      gaussNodesAndWeights_[0][i] = d[i];
      // Weights
      gaussNodesAndWeights_[1][i] = 2.0 * pow(z(0, i), 2);
    }
  isAlreadyComputedGaussNodesAndWeights_ = true;
}

/* integrationNodesNumber accessors */
UnsignedLong DistributionImplementation::getIntegrationNodesNumber() const
{
  return integrationNodesNumber_;
}

void DistributionImplementation::setIntegrationNodesNumber(const UnsignedLong integrationNodesNumber) const
{
  if (integrationNodesNumber != integrationNodesNumber_)
    {
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      isAlreadyComputedGaussNodesAndWeights_ = false;
      integrationNodesNumber_ = integrationNodesNumber;
    }
}

/* Gauss nodes and weights accessor */
NumericalSample DistributionImplementation::getGaussNodesAndWeights() const
{
  if (!isAlreadyComputedGaussNodesAndWeights_) computeGaussNodesAndWeights();
  return gaussNodesAndWeights_;
}

/* Gauss nodes and weights accessor */
NumericalPoint DistributionImplementation::getGaussNodesAndWeights(NumericalPoint & weights) const
{
  if (!isAlreadyComputedGaussNodesAndWeights_) computeGaussNodesAndWeights();
  weights = gaussNodesAndWeights_[1];
  return gaussNodesAndWeights_[0];
}


/* Get the moments of the standardized distribution */
NumericalPoint DistributionImplementation::getStandardMoment(const UnsignedLong n) const
{
  return getStandardRepresentative()->getMoment(n);
}


/* Compute the shifted moments of the distribution */
NumericalPoint DistributionImplementation::computeShiftedMoment(const UnsignedLong n,
                                                                const NumericalPoint & shift) const
{
  const UnsignedLong dimension(getDimension());
  if (n == 0) throw InvalidArgumentException(HERE) << "Error: the centered moments of order 0 are undefined.";
  if (shift.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the shift dimension must match the distribution dimension.";
  NumericalPoint moment(dimension);
  // Specific algorithm for discrete integral distributions
  if (isDiscrete())
    {
      NumericalSample support(getSupport());
      for (UnsignedLong i = 0; i < support.getSize(); ++i)
        {
          const NumericalScalar pdf(computePDF(support[i]));
          for (UnsignedLong j = 0; j < dimension; ++j)
            moment[j] += pow(support[i][j] - shift[j], n) * pdf;
        }
    }
  else
    {
      const NumericalScalar epsilon(sqrt(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")));
      const UnsignedLong MaximumLevel(ResourceMap::GetAsUnsignedLong( "DistributionImplementation-DefaultLevelNumber" ) + 3);
      // For each component
      for(UnsignedLong component = 0; component < dimension; ++component)
        {
          NumericalScalar h(0.5);
          UnsignedLong N(6);
          const Implementation marginalDistribution(getMarginal(component));
          const NumericalScalar shiftComponent(shift[component]);
          // Central term
          moment[component] = h * 0.5 * pow(marginalDistribution->computeQuantile(0.5)[0], n);
          // First block
          for (UnsignedLong j = 1; j <= N; ++j)
            {
              const NumericalScalar hj(h * j);
              const NumericalScalar expHj(exp(hj));
              const NumericalScalar iexpHj(1.0 / expHj);
              const NumericalScalar sinhHj(0.5 * (expHj - iexpHj));
              const NumericalScalar expSinhHj(exp(sinhHj));
              const NumericalScalar iexpSinhHj(1.0 / expSinhHj);
              const NumericalScalar iTwoCoshSinhHj(1.0 / (expSinhHj + iexpSinhHj));
              const NumericalScalar xjm(iexpSinhHj * iTwoCoshSinhHj);
              const NumericalScalar xjp(expSinhHj * iTwoCoshSinhHj);
              const NumericalScalar wj((expHj + iexpHj) * iTwoCoshSinhHj * iTwoCoshSinhHj);
              moment[component] += h * wj * (pow(marginalDistribution->computeQuantile(xjm)[0] - shiftComponent, n) + pow(marginalDistribution->computeQuantile(xjp)[0] - shiftComponent, n));
            } // End of first block
          //values[0] = moment[component];
          // Sequential addition of half-blocks
          NumericalScalar error(1.0);
          UnsignedLong level(0);
          while( (error > epsilon) && (level < MaximumLevel))
            {
              ++level;
              h *= 0.5;
              moment[component] *= 0.5;
              NumericalScalar delta(0.0);
              for (UnsignedLong j = 0; j <= N; ++j)
                {
                  const NumericalScalar hj(h * (2 * j + 1));
                  const NumericalScalar expHj(exp(hj));
                  const NumericalScalar iexpHj(1.0 / expHj);
                  const NumericalScalar sinhHj(0.5 * (expHj - iexpHj));
                  const NumericalScalar expSinhHj(exp(sinhHj));
                  const NumericalScalar iexpSinhHj(1.0 / expSinhHj);
                  const NumericalScalar iTwoCoshSinhHj(1.0 / (expSinhHj + iexpSinhHj));
                  const NumericalScalar xjm(iexpSinhHj * iTwoCoshSinhHj);
                  const NumericalScalar xjp(expSinhHj * iTwoCoshSinhHj);
                  NumericalScalar wj((expHj + iexpHj) * iTwoCoshSinhHj * iTwoCoshSinhHj);
                  delta += h * wj * (pow(marginalDistribution->computeQuantile(xjm)[0] - shiftComponent, n) + pow(marginalDistribution->computeQuantile(xjp)[0] - shiftComponent, n));
                }
              error = fabs((delta - moment[component]) / (1.0 + fabs(delta)));
              moment[component] += delta;
              N *= 2;
            } // End of half-block
        } // End of each component
    }
  return moment;
}

/* Check if the distribution is elliptical */
Bool DistributionImplementation::isElliptical() const
{
  return false;
}

/* Check if the distribution is continuous */
Bool DistributionImplementation::isContinuous() const
{
  return true;
}

/* Check if the distribution is discrete */
Bool DistributionImplementation::isDiscrete() const
{
  return false;
}

/* Tell if the distribution is integer valued */
Bool DistributionImplementation::isIntegral() const
{
  return false;
}

/* Tell if the distribution has elliptical copula */
Bool DistributionImplementation::hasEllipticalCopula() const
{
  return true;
}

/* Tell if the distribution has independent copula */
Bool DistributionImplementation::hasIndependentCopula() const
{
  return true;
}

/* Get the support of a distribution that intersect a given interval */
NumericalSample DistributionImplementation::getSupport(const Interval & interval) const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::getSupport()";
}

/* Get the support on the whole range */
NumericalSample DistributionImplementation::getSupport() const
{
  return getSupport(getRange());
}

/* Compute the density generator of the elliptical generator, i.e.
 *  the function phi such that the density of the distribution can
 *  be written as p(x) = phi(t(x-mu)R(x-mu))
 */
NumericalScalar DistributionImplementation::computeDensityGenerator(const NumericalScalar betaSquare) const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::computeDensityGenerator()";
}

/* Compute the derivative of the density generator */
NumericalScalar DistributionImplementation::computeDensityGeneratorDerivative(const NumericalScalar betaSquare) const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::computeDensityGeneratorDerivative()";
}

/* Compute the seconde derivative of the density generator */
NumericalScalar DistributionImplementation::computeDensityGeneratorSecondDerivative(const NumericalScalar betaSquare) const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::computeDensityGeneratorSecondDerivative()";
}

/* Get the i-th marginal distribution */
DistributionImplementation::Implementation DistributionImplementation::getMarginal(const UnsignedLong i) const
{
  if ((i != 0) || (dimension_ != 1)) throw NotYetImplementedException(HERE);
  return clone();
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
DistributionImplementation::Implementation DistributionImplementation::getMarginal(const Indices & indices) const
{
  if ((indices.getSize() != 1) || (indices[0] != 0) || (dimension_ != 1)) throw NotYetImplementedException(HERE);
  return clone();
}

/* Get the copula of a distribution */
DistributionImplementation::Implementation DistributionImplementation::getCopula() const
{
  if (dimension_ == 1) return new IndependentCopula(1);
  return new SklarCopula(*this);
}

/* Get the isoprobabilist transformation */
DistributionImplementation::IsoProbabilisticTransformation DistributionImplementation::getIsoProbabilisticTransformation() const
{
  // Special case for dimension 1
  if (dimension_ == 1)
    {
      DistributionCollection collection(1);
      collection[0] = *this;
      // Get the marginal transformation evaluation implementation
      MarginalTransformationEvaluation evaluation(collection, DistributionCollection(1, Normal()));
      // We have to correct the direction because the output collection corresponds to the standard space, so there is no parameter to take into account.
      evaluation.setDirection(MarginalTransformationEvaluation::FROM);
      const EvaluationImplementation p_evaluation(evaluation.clone());
      // Get the marginal transformation gradient implementation
      const GradientImplementation p_gradient = new MarginalTransformationGradient(evaluation);
      // Get the marginal transformation hessian implementation
      const HessianImplementation p_hessian = new MarginalTransformationHessian(evaluation);
      InverseIsoProbabilisticTransformation inverseTransformation(p_evaluation, p_gradient, p_hessian);
      NumericalPointWithDescription parameters(getParametersCollection()[0]);
      const UnsignedLong parametersDimension(parameters.getDimension());
      Description parametersDescription(parameters.getDescription());
      const String name(parameters.getName());
      for (UnsignedLong i = 0; i < parametersDimension; i++) parametersDescription[i] = OSS() << name << "_" << parametersDescription[i];
      parameters.setDescription(parametersDescription);
      inverseTransformation.setParameters(parameters);
      return inverseTransformation;
    }
  // General case, Rosenblatt transformation
  return NumericalMathFunctionImplementation(new RosenblattEvaluation(clone()));
}

/* Get the inverse isoprobabilist transformation */
DistributionImplementation::InverseIsoProbabilisticTransformation DistributionImplementation::getInverseIsoProbabilisticTransformation() const
{
  // Special case for dimension 1
  if (dimension_ == 1)
    {
      DistributionCollection collection(1);
      collection[0] = *this;
      // Get the marginal transformation evaluation implementation
      MarginalTransformationEvaluation evaluation(DistributionCollection(1, Normal()), collection);
      // We have to correct the direction because the input collection corresponds to the standard space, so there is no parameter to take into account.
      evaluation.setDirection(MarginalTransformationEvaluation::TO);
      const EvaluationImplementation p_evaluation(evaluation.clone());
      // Get the marginal transformation gradient implementation
      const GradientImplementation p_gradient = new MarginalTransformationGradient(evaluation);
      // Get the marginal transformation hessian implementation
      const HessianImplementation p_hessian = new MarginalTransformationHessian(evaluation);
      InverseIsoProbabilisticTransformation inverseTransformation(p_evaluation, p_gradient, p_hessian);
      NumericalPointWithDescription parameters(getParametersCollection()[0]);
      const UnsignedLong parametersDimension(parameters.getDimension());
      Description parametersDescription(parameters.getDescription());
      const String name(parameters.getName());
      for (UnsignedLong i = 0; i < parametersDimension; i++) parametersDescription[i] = OSS() << name << "_" << parametersDescription[i];
      parameters.setDescription(parametersDescription);
      inverseTransformation.setParameters(parameters);
      return inverseTransformation;
    }
  // General case, inverse Rosenblatt transformation
  return NumericalMathFunctionImplementation(new InverseRosenblattEvaluation(clone()));
}

/* Get the standard distribution */
void DistributionImplementation::computeStandardDistribution() const
{
  Normal standardDistribution(dimension_);
  standardDistribution.setDescription(getDescription());
  p_standardDistribution_ = standardDistribution.clone();
  isAlreadyComputedStandardDistribution_ = true;
}

/* Get the standard distribution */
DistributionImplementation::Implementation DistributionImplementation::getStandardDistribution() const
{
  if (!isAlreadyComputedStandardDistribution_) computeStandardDistribution();
  return p_standardDistribution_;
}

/* Get the standard representative in the parametric family, associated with the standard moments */
DistributionImplementation::Implementation DistributionImplementation::getStandardRepresentative() const
{
  return clone();
}

/* Compute the radial distribution CDF */
NumericalScalar DistributionImplementation::computeRadialDistributionCDF(const NumericalScalar radius,
                                                                         const Bool tail) const
{
  throw NotYetImplementedException(HERE);
}


/* Draw the PDF of a discrete distribution */
Graph DistributionImplementation::drawDiscretePDF(const NumericalScalar xMin,
                                                  const NumericalScalar xMax,
                                                  const UnsignedLong pointNumber) const
{
  if (getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot draw the PDF of a multidimensional discrete distribution this way.";
  if (xMax <= xMin) throw InvalidArgumentException(HERE) << "Error: cannot draw a PDF with xMax >= xMin, here xmin=" << xMin << " and xmax=" << xMax;
  const String title(OSS() << getName() << " PDF");
  const NumericalSample support(getSupport(Interval(xMin, xMax)));
  // First the vertical bars
  const String xName(getDescription()[0]);
  Graph graphPDF(title, xName, "PDF", true, "topright");
  NumericalPoint point(2);
  point[0] = xMin;
  NumericalSample data(0, 2);
  data.add(point);
  for (UnsignedLong i = 0; i < support.getSize(); ++i)
    {
      point[0] = support[i][0];
      data.add(point);
      point[1] = computePDF(point[0]);
      data.add(point);
      point[1] = 0.0;
      data.add(point);
    }
  point[0] = xMax;
  point[1] = 0.0;
  data.add(point);
  graphPDF.add(Curve(data, "red", "solid", 2, title));
  return graphPDF;
}

/* Draw the PDF of the distribution when its dimension is 1 */
Graph DistributionImplementation::drawPDF(const NumericalScalar xMin,
                                          const NumericalScalar xMax,
                                          const UnsignedLong pointNumber) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "Error: can draw a PDF only if dimension equals 1, here dimension=" << dimension_;
  if (xMax <= xMin) throw InvalidArgumentException(HERE) << "Error: cannot draw a PDF with xMax >= xMin, here xmin=" << xMin << " and xmax=" << xMax;
  if (pointNumber < 2) throw InvalidArgumentException(HERE) << "Error: cannot draw a PDF with a point number < 2";
  if (isDiscrete()) return drawDiscretePDF(xMin, xMax, pointNumber);
  // Discretization of the x axis
  const String title(OSS() << getName() << " PDF");
  const Curve curvePDF(computePDF(xMin, xMax, pointNumber), "red", "solid", 2, title);
  const String xName(getDescription()[0]);
  Graph graphPDF(title, xName, "PDF", true, "topright");
  graphPDF.add(curvePDF);
  NumericalPoint boundingBox(graphPDF.getBoundingBox());
  boundingBox[3] *= 1.1;
  graphPDF.setBoundingBox(boundingBox);
  return graphPDF;
}

/* Draw the PDF of the distribution when its dimension is 1 */
Graph DistributionImplementation::drawPDF(const UnsignedLong pointNumber) const
{
  if (getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: this method is available only for 1D distributions";
  // For discrete distributions, use the numerical range to define the drawing range
  if (isDiscrete()) return drawPDF(range_.getLowerBound()[0] - 1.0, range_.getUpperBound()[0] + 1.0, pointNumber);
  const NumericalScalar xMin(computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))[0]);
  const NumericalScalar xMax(computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ))[0]);
  const NumericalScalar delta(2.0 * (xMax - xMin) * (1.0 - 0.5 * (ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ) - ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))));
  return drawPDF(xMin - delta, xMax + delta, pointNumber);
}

/* Draw the PDF of a 1D marginal */
Graph DistributionImplementation::drawMarginal1DPDF(const UnsignedLong marginalIndex,
                                                    const NumericalScalar xMin,
                                                    const NumericalScalar xMax,
                                                    const UnsignedLong pointNumber) const
{
  Graph marginalGraph(getMarginal(marginalIndex)->drawPDF(xMin, xMax, pointNumber));
  marginalGraph.setTitle(OSS() << getName() << ", " << description_[marginalIndex] << " component PDF");
  return marginalGraph;
}

/* Draw the PDF of the distribution when its dimension is 2 */
Graph DistributionImplementation::drawPDF(const NumericalPoint & xMin,
                                          const NumericalPoint & xMax,
                                          const Indices & pointNumber) const
{
  PDFWrapper pdfWrapper(this);
  const NumericalMathFunction pdfFunction(bindMethod<PDFWrapper, NumericalPoint, NumericalPoint>(pdfWrapper, &PDFWrapper::computePDF, getDimension(), 1));
  Graph graph(pdfFunction.draw(xMin, xMax, pointNumber));
  const String title(OSS() << getName() << " iso-PDF");
  Drawable drawable(graph.getDrawable(0));
  drawable.setLegendName(title);
  graph.setDrawable(drawable, 0);
  graph.setTitle(title);
  graph.setXTitle(description_[0]);
  graph.setYTitle(description_[1]);
  graph.setAxes(true);
  graph.setLegendPosition("topright");
  return graph;
}

/* Draw the PDF of the distribution when its dimension is 2 */
Graph DistributionImplementation::drawPDF(const NumericalPoint & xMin,
                                          const NumericalPoint & xMax) const
{
  return drawPDF(xMin, xMax, Indices(2, ResourceMap::GetAsUnsignedLong( "DistributionImplementation-DefaultPointNumber" )));
}

/* Draw the PDF of the distribution when its dimension is 2 */
Graph DistributionImplementation::drawPDF(const Indices & pointNumber) const
{
  NumericalPoint xMin(2);
  xMin[0] = getMarginal(0)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))[0];
  xMin[1] = getMarginal(1)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))[0];
  NumericalPoint xMax(2);
  xMax[0] = getMarginal(0)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ))[0];
  xMax[1] = getMarginal(1)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ))[0];
  const NumericalPoint delta(2.0 * (xMax - xMin) * (1.0 - 0.5 * (ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ) - ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))));
  return drawPDF(xMin - delta, xMax + delta, pointNumber);
}

/* Draw the PDF of a 2D marginal */
Graph DistributionImplementation::drawMarginal2DPDF(const UnsignedLong firstMarginal,
                                                    const UnsignedLong secondMarginal,
                                                    const NumericalPoint & xMin,
                                                    const NumericalPoint & xMax,
                                                    const Indices & pointNumber) const
{
  Indices indices(2);
  indices[0] = firstMarginal;
  indices[1] = secondMarginal;
  Graph marginalGraph(getMarginal(indices)->drawPDF(xMin, xMax, pointNumber));
  marginalGraph.setTitle(OSS() << getName() << " (" << description_[firstMarginal] << ", " << description_[secondMarginal] << ") components iso-PDF");
  return marginalGraph;
}

/* Draw the PDF of the distribution when its dimension is 1 or 2 */
Graph DistributionImplementation::drawPDF() const
{
  UnsignedLong dimension(getDimension());
  // Generic interface for the 1D and 2D cases
  if (dimension == 1) return drawPDF(ResourceMap::GetAsUnsignedLong( "DistributionImplementation-DefaultPointNumber" ));
  if (dimension == 2) return drawPDF(Indices(2, ResourceMap::GetAsUnsignedLong( "DistributionImplementation-DefaultPointNumber" )));
  throw InvalidDimensionException(HERE) << "Error: can draw a PDF only if dimension equals 1 or 2, here dimension=" << dimension;
}


/* Draw the CDF of a discrete distribution */
Graph DistributionImplementation::drawDiscreteCDF(const NumericalScalar xMin,
                                                  const NumericalScalar xMax,
                                                  const UnsignedLong pointNumber) const
{
  // Value :    0    1/5  2/5  3/5    4/5    1
  // Data  : ------+-----+---+------+----+---------
  // Case 1: ------------------------------[----]--
  // Case 2: ------------------[---]---------------
  //         -[--]---------------------------------
  // Case 3: ----------[---]-----------------------
  //         ---[-----------------------------]----
  //         -------[-----------------]------------
  if (getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot draw the CDF of a multidimensional discrete distribution this way.";
  if (xMax <= xMin) throw InvalidArgumentException(HERE) << "Error: cannot draw a PDF with xMax >= xMin, here xmin=" << xMin << " and xmax=" << xMax;
  // Create the graph that will store the staircase representing the empirical CDF
  const String title(OSS() << getName() << " CDF");
  const NumericalSample support(getSupport(Interval(xMin, xMax)));
  const UnsignedLong size(support.getSize());
  const String xName(getDescription()[0]);
  Graph graphCDF(title, xName, "CDF", true, "topleft");
  NumericalSample data(size + 2, 2);
  data[0][0] = xMin;
  data[0][1] = computeCDF(xMin);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar x(support[i][0]);
      data[i + 1][0] = x;
      data[i + 1][1] = computeCDF(x);
    }
  data[size + 1][0] = xMax;
  data[size + 1][1] = computeCDF(xMax);
  graphCDF.add(Staircase(data, "red", "solid", 2, "s", title));
  return graphCDF;
}

/* Draw the CDF of the distribution when its dimension is 1 */
Graph DistributionImplementation::drawCDF(const NumericalScalar xMin,
                                          const NumericalScalar xMax,
                                          const UnsignedLong pointNumber) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "Error: can draw a CDF only if dimension equals 1, here dimension=" << dimension_;
  if (xMax <= xMin) throw InvalidArgumentException(HERE) << "Error: cannot draw a CDF with xMax >= xMin, here xmin=" << xMin << " and xmax=" << xMax;
  if (pointNumber < 2) throw InvalidArgumentException(HERE) << "Error: cannot draw a CDF with a point number < 2";
  if (isDiscrete()) return drawDiscreteCDF(xMin, xMax, pointNumber);
  const String title(OSS() << getName() << " CDF");
  const Curve curveCDF(computeCDF(xMin, xMax, pointNumber), "red", "solid", 2, title);
  const String xName(getDescription()[0]);
  Graph graphCDF(title, xName, "CDF", true, "topleft");
  graphCDF.add(curveCDF);
  return graphCDF;
}

/* Draw the CDF of the distribution when its dimension is 1 */
Graph DistributionImplementation::drawCDF(const UnsignedLong pointNumber) const
{
  if (getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: this method is available only for 1D distributions";
  // For discrete distributions, use the numerical range to define the drawing range
  if (isDiscrete()) return drawCDF(range_.getLowerBound()[0] - 1.0, range_.getUpperBound()[0] + 1.0, pointNumber);
  const NumericalScalar xMin(computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))[0]);
  const NumericalScalar xMax(computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ))[0]);
  const NumericalScalar delta(2.0 * (xMax - xMin) * (1.0 - 0.5 * (ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ) - ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))));
  return drawCDF(xMin - delta, xMax + delta, pointNumber);
}

/* Draw the CDF of a 1D marginal */
Graph DistributionImplementation::drawMarginal1DCDF(const UnsignedLong marginalIndex,
                                                    const NumericalScalar xMin,
                                                    const NumericalScalar xMax,
                                                    const UnsignedLong pointNumber) const
{
  Graph marginalGraph(getMarginal(marginalIndex)->drawCDF(xMin, xMax, pointNumber));
  marginalGraph.setTitle(OSS() << getName() << ", " << description_[marginalIndex] << " component CDF");
  return marginalGraph;
}

/* Draw the CDF of the distribution when its dimension is 2 */
Graph DistributionImplementation::drawCDF(const NumericalPoint & xMin,
                                          const NumericalPoint & xMax,
                                          const Indices & pointNumber) const
{
  CDFWrapper cdfWrapper(this);
  NumericalMathFunction cdfFunction(bindMethod<CDFWrapper, NumericalPoint, NumericalPoint>(cdfWrapper, &CDFWrapper::computeCDF, getDimension(), 1));
  Graph graph(cdfFunction.draw(xMin, xMax, pointNumber));
  const String title(OSS() << getName() << " iso-CDF");
  Drawable drawable(graph.getDrawable(0));
  drawable.setLegendName(title);
  graph.setDrawable(drawable, 0);
  graph.setTitle(title);
  graph.setXTitle(description_[0]);
  graph.setYTitle(description_[1]);
  graph.setAxes(true);
  graph.setLegendPosition("topright");
  return graph;
}

/* Draw the CDF of the distribution when its dimension is 2 */
Graph DistributionImplementation::drawCDF(const NumericalPoint & xMin,
                                          const NumericalPoint & xMax) const
{
  return drawCDF(xMin, xMax, Indices(2, ResourceMap::GetAsUnsignedLong( "DistributionImplementation-DefaultPointNumber" )));
}

/* Draw the CDF of the distribution when its dimension is 2 */
Graph DistributionImplementation::drawCDF(const Indices & pointNumber) const
{
  NumericalPoint xMin(2);
  xMin[0] = getMarginal(0)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))[0];
  xMin[1] = getMarginal(1)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))[0];
  NumericalPoint xMax(2);
  xMax[0] = getMarginal(0)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ))[0];
  xMax[1] = getMarginal(1)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ))[0];
  const NumericalPoint delta(2.0 * (xMax - xMin) * (1.0 - 0.5 * (ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ) - ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))));
  return drawCDF(xMin - delta, xMax + delta, pointNumber);
}

/* Draw the CDF of a 2D marginal */
Graph DistributionImplementation::drawMarginal2DCDF(const UnsignedLong firstMarginal,
                                                    const UnsignedLong secondMarginal,
                                                    const NumericalPoint & xMin,
                                                    const NumericalPoint & xMax,
                                                    const Indices & pointNumber) const
{
  Indices indices(2);
  indices[0] = firstMarginal;
  indices[1] = secondMarginal;
  Graph marginalGraph(getMarginal(indices)->drawCDF(xMin, xMax, pointNumber));
  marginalGraph.setTitle(OSS() << getName() << " (" << description_[firstMarginal] << ", " << description_[secondMarginal] << ") components iso-CDF");
  return marginalGraph;
}

/* Draw the CDF of the distribution when its dimension is 1 or 2 */
Graph DistributionImplementation::drawCDF() const
{
  const UnsignedLong dimension(getDimension());
  // Generic interface for the 1D and 2D cases
  if (dimension == 1) return drawCDF(ResourceMap::GetAsUnsignedLong( "DistributionImplementation-DefaultPointNumber" ));
  if (dimension == 2) return drawCDF(Indices(2, ResourceMap::GetAsUnsignedLong( "DistributionImplementation-DefaultPointNumber" )));
  throw InvalidDimensionException(HERE) << "Error: can draw a CDF only if dimension equals 1 or 2, here dimension=" << dimension;
}

/* Parameters value and description accessor */
DistributionImplementation::NumericalPointWithDescriptionCollection DistributionImplementation::getParametersCollection() const
{
  // By default, assume an empty set of parameters for a 1D distribution or a copula
  return NumericalPointWithDescriptionCollection(1, NumericalPointWithDescription(0));
}

void DistributionImplementation::setParametersCollection(const NumericalPointWithDescriptionCollection & parametersCollection)
{
  // Get the actual collection of parameters to check the description and the size
  const NumericalPointWithDescriptionCollection actualParameters(getParametersCollection());
  const UnsignedLong size(actualParameters.getSize());
  if (parametersCollection.getSize() != size) throw InvalidArgumentException(HERE) << "Error: the given parameters collection has an invalid size (" << parametersCollection.getSize() << "), it should be " << size;
  NumericalPointCollection coll(0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const UnsignedLong dimension(actualParameters[i].getDimension());
      if (parametersCollection[i].getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given parameters collection has an invalid dimension at index " << i;
      coll.add(parametersCollection[i]);
    }
  setParametersCollection(coll);
}

void DistributionImplementation::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  const UnsignedLong size(parametersCollection.getSize());
  NumericalPointWithDescriptionCollection newParameters(size);
  for (UnsignedLong i = 0; i < size; ++i) newParameters[i] = parametersCollection[i];
  setParametersCollection(newParameters);
}

void DistributionImplementation::setParametersCollection(const NumericalPoint & flattenCollection)
{
  const NumericalPointWithDescriptionCollection currentParameters(getParametersCollection());
  UnsignedLong size(currentParameters.getSize());
  NumericalPointCollection parametersCollection;
  UnsignedLong flattenSize = 0;
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const UnsignedLong subSize(currentParameters[i].getSize());
      NumericalPoint subCollection(subSize);
      // copy parameters into the collection if the flatten collection is large enough
      if ( (flattenSize + subSize) <= flattenCollection.getSize() )
        {
          for (UnsignedLong j = 0; j < subSize; ++j) subCollection[j] = flattenCollection[flattenSize + j];
        }
      parametersCollection.add(subCollection);
      flattenSize += subSize;
    }
  if (flattenCollection.getSize() != flattenSize) throw InvalidArgumentException(HERE) << "Error: the given parameters collection has an invalid size (" << flattenCollection.getSize() << "), it should be " << flattenSize;
  setParametersCollection(parametersCollection);
}

/* Parameters number */
UnsignedLong DistributionImplementation::getParametersNumber() const
{
  const NumericalPointWithDescriptionCollection parametersCollection(getParametersCollection());
  const UnsignedLong size(parametersCollection.getSize());
  UnsignedLong parametersNumber(0);
  for (UnsignedLong i = 0; i < size; ++i) parametersNumber += parametersCollection[i].getDimension();
  return parametersNumber;
}

/* Description accessor */
void DistributionImplementation::setDescription(const Description & description)
{
  const UnsignedLong size(description.getSize());
  if (size != getDimension()) throw InvalidArgumentException(HERE) << "Error: the description must have the same size than the distribution dimension, here size=" << size << " and dimension=" << getDimension();
  // Check if the description is valid
  // First, copy the description
  Description test(description);
  // Second, sort the copy
  std::sort(test.begin(), test.end());
  // Third, move the duplicates at the end
  Description::const_iterator it = std::unique(test.begin(), test.end());
  // Fourth, check if there was any duplicate
  if (it != test.end())
    {
      LOGINFO(OSS() << "Warning! The description of the distribution " << getName() << " is " << description << " and cannot identify uniquely the marginal distribution. Appended unique identifier to fix it:");
      Description newDescription(description);
      for (UnsignedLong i = 0; i < size; ++i) newDescription[i] = OSS() << "marginal_" << i + 1 << "_" << description[i];
      LOGINFO(OSS() << "the new description is " << newDescription);
      description_ = newDescription;
    }
  else description_ = description;
}

/* Description accessot */
Description DistributionImplementation::getDescription() const
{
  return description_;
}

/* Accessor to PDF computation precision */
NumericalScalar DistributionImplementation::getPDFEpsilon() const
{
  return pdfEpsilon_;
}

/* Accessor to CDF computation precision */
NumericalScalar DistributionImplementation::getCDFEpsilon() const
{
  return cdfEpsilon_;
}

/* Get a positon indicator for a 1D distribution */
NumericalScalar DistributionImplementation::getPositionIndicator() const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "Error: cannot get the position indicator of a distribution with dimension > 1";
  // Return the median of the distribution
  return computeQuantile(0.5)[0];
}

/* Get a dispersion indicator for a 1D distribution */
NumericalScalar DistributionImplementation::getDispersionIndicator() const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "Error: cannot get the dispersion indicator of a distribution with dimension > 1";
  // Return the interquartile of the distribution
  return computeQuantile(0.75)[0] - computeQuantile(0.25)[0];
}



/* Method save() stores the object through the StorageManager */
void DistributionImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "mean_", mean_ );
  adv.saveAttribute( "covariance_", covariance_ );
  adv.saveAttribute( "gaussNodesAndWeights_", gaussNodesAndWeights_ );
  adv.saveAttribute( "integrationNodesNumber_", integrationNodesNumber_ );
  adv.saveAttribute( "isAlreadyComputedMean_", isAlreadyComputedMean_ );
  adv.saveAttribute( "isAlreadyComputedCovariance_", isAlreadyComputedCovariance_ );
  adv.saveAttribute( "isAlreadyComputedGaussNodesAndWeights_", isAlreadyComputedGaussNodesAndWeights_ );
  adv.saveAttribute( "dimension_", dimension_ );
  adv.saveAttribute( "weight_", weight_ );
  adv.saveAttribute( "description_", description_ );
}

/* Method load() reloads the object from the StorageManager */
void DistributionImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "mean_", mean_ );
  adv.loadAttribute( "covariance_", covariance_ );
  adv.loadAttribute( "gaussNodesAndWeights_", gaussNodesAndWeights_ );
  adv.loadAttribute( "integrationNodesNumber_", integrationNodesNumber_ );
  adv.loadAttribute( "isAlreadyComputedMean_", isAlreadyComputedMean_ );
  adv.loadAttribute( "isAlreadyComputedCovariance_", isAlreadyComputedCovariance_ );
  adv.loadAttribute( "isAlreadyComputedGaussNodesAndWeights_", isAlreadyComputedGaussNodesAndWeights_ );
  adv.loadAttribute( "dimension_", dimension_ );
  adv.loadAttribute( "weight_", weight_ );
  adv.loadAttribute( "description_", description_ );
}

END_NAMESPACE_OPENTURNS
