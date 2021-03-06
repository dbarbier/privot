//                                               -*- C++ -*-
/**
 *  @file  Poisson.cxx
 *  @brief The Poisson distribution
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include <cmath>
#include "Poisson.hxx"
#include "RandomGenerator.hxx"
#include "SpecFunc.hxx"
#include "DistFunc.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Poisson);

static Factory<Poisson> RegisteredFactory("Poisson");

/* Default constructor */
Poisson::Poisson()
  : DiscreteDistribution("Poisson"),
    lambda_(1.0)
{
  // We set the dimension of the Poisson distribution
  setDimension( 1 );
  computeRange();
}

/* Parameters constructor */
Poisson::Poisson(const NumericalScalar lambda)
  : DiscreteDistribution("Poisson"),
    lambda_(0.0)
{
  // We set the dimension of the Poisson distribution
  setDimension( 1 );
  // This call set also the range.
  setLambda(lambda);
}

/* Comparison operator */
Bool Poisson::operator ==(const Poisson & other) const
{
  if (this == &other) return true;
  return (lambda_ == other.lambda_);
}

/* String converter */
String Poisson::__repr__() const
{
  OSS oss;
  oss << "class=" << Poisson::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " lambda=" << lambda_;
  return oss;
}

String Poisson::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(lambda = " << lambda_ << ")";
  return oss;
}

/* Virtual constructor */
Poisson * Poisson::clone() const
{
  return new Poisson(*this);
}

/* Get one realization of the distribution */
NumericalPoint Poisson::getRealization() const
{
  return NumericalPoint(1, DistFunc::rPoisson(lambda_));
}


/* Get the PDF of the distribution */
NumericalScalar Poisson::computePDF(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);
  if ((k < -ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) || (fabs(k - round(k)) > ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"))) return 0.0;
  return exp(k * log(lambda_) - lambda_ - SpecFunc::LnGamma(k + 1.0));
}


/* Get the CDF of the distribution */
NumericalScalar Poisson::computeCDF(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);
  if (k < -ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return 0.0;
  return DistFunc::pGamma(floor(k) + 1.0, lambda_, true);
}

NumericalScalar Poisson::computeComplementaryCDF(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);
  if (k < -ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return 1.0;
  return DistFunc::pGamma(floor(k) + 1.0, lambda_);
}

/* Get the PDF gradient of the distribution */
NumericalPoint Poisson::computePDFGradient(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);
  NumericalPoint pdfGradient(1, 0.0);
  if ((k < -ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) || (fabs(k - round(k)) > ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"))) return pdfGradient;
  return NumericalPoint(1, (k - lambda_) * exp((k - 1.0) * log(lambda_) - lambda_ - SpecFunc::LnGamma(k + 1.0)));
}


/* Get the CDF gradient of the distribution */
NumericalPoint Poisson::computeCDFGradient(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);
  if (k < -ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return NumericalPoint(1, 0.0);
  return NumericalPoint(1, -exp(floor(k) * log(lambda_) - lambda_ - SpecFunc::LnGamma(floor(k) + 1.0)));
}

/* Get the quantile of the distribution */
NumericalScalar Poisson::computeScalarQuantile(const NumericalScalar prob,
                                               const Bool tail,
                                               const NumericalScalar precision) const
{
  const NumericalScalar p(tail ? 1.0 - prob : prob);
  UnsignedLong quantile((UnsignedLong)(round(DistFunc::qGamma(lambda_, p))));
  NumericalScalar pdf(computePDF(quantile));
  NumericalScalar cdf(computeCDF(quantile));
  if (cdf < p)
    {
      while (cdf < p)
        {
          ++quantile;
          pdf *= lambda_ / quantile;
          cdf += pdf;
        }
      return quantile;
    }
  else
    {
      while ((cdf >= p) && (quantile > 0))
        {
          cdf -= pdf;
          pdf *= quantile / lambda_;
          --quantile;
        }
      if (cdf < p) return quantile + 1;
      return quantile;
    }
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex Poisson::computeCharacteristicFunction(const NumericalScalar x) const
{
  return exp(computeLogCharacteristicFunction(x));
}

NumericalComplex Poisson::computeLogCharacteristicFunction(const NumericalScalar x) const
{
  return lambda_ * (exp(NumericalComplex(0.0, x)) - 1.0);
}

/* Get the generating function of the distribution, i.e. psi(z) = E(z^X) */
NumericalComplex Poisson::computeGeneratingFunction(const NumericalComplex & z) const
{
  return exp(computeLogGeneratingFunction(z));
}

NumericalComplex Poisson::computeLogGeneratingFunction(const NumericalComplex & z) const
{
  return lambda_ * (z - 1.0);
}

/* Compute the mean of the distribution */
void Poisson::computeMean() const
{
  mean_ = NumericalPoint(1, lambda_);
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Poisson::getStandardDeviation() const
{
  return NumericalPoint(1, sqrt(lambda_));
}

/* Get the skewness of the distribution */
NumericalPoint Poisson::getSkewness() const
{
  return NumericalPoint(1, 1.0 / sqrt(lambda_));
}

/* Get the kurtosis of the distribution */
NumericalPoint Poisson::getKurtosis() const
{
  return NumericalPoint(1, 3.0 + 1.0 / lambda_);
}

/* Compute the covariance of the distribution */
void Poisson::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  covariance_(0, 0) = lambda_;
  isAlreadyComputedCovariance_ = true;
}

/* Get the support of a discrete distribution that intersect a given interval */
NumericalSample Poisson::getSupport(const Interval & interval) const
{
  if (interval.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given interval has a dimension that does not match the distribution dimension.";
  const UnsignedLong kMin(static_cast< UnsignedLong > (std::max(ceil(interval.getLowerBound()[0]), 0.0)));
  const UnsignedLong kMax(static_cast< UnsignedLong > (floor(interval.getUpperBound()[0])));
  NumericalSample result(0, 1);
  for (UnsignedLong k = kMin; k <= kMax; ++k)
    result.add(NumericalPoint(1, k));
  return result;
}

/* Parameters value and description accessor */
Poisson::NumericalPointWithDescriptionCollection Poisson::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(1);
  Description description(point.getDimension());
  point[0] = lambda_;
  description[0] = "lambda";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void Poisson::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = Poisson(parametersCollection[0][0]);
}



/* Lambda accessor */
void Poisson::setLambda(const NumericalScalar lambda)
{
  if (lambda <= 0.0) throw InvalidArgumentException(HERE) << "Lambda must be positive, here lambda=" << lambda;
  if (lambda != lambda_)
    {
      lambda_ = lambda;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

/* Lambda accessor */
NumericalScalar Poisson::getLambda() const
{
  return lambda_;
}


/* Method save() stores the object through the StorageManager */
void Poisson::save(Advocate & adv) const
{
  DiscreteDistribution::save(adv);
  adv.saveAttribute( "lambda_", lambda_ );
}

/* Method load() reloads the object from the StorageManager */
void Poisson::load(Advocate & adv)
{
  DiscreteDistribution::load(adv);
  adv.loadAttribute( "lambda_", lambda_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
