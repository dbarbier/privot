//                                               -*- C++ -*-
/**
 *  @file  Exponential.cxx
 *  @brief The Exponential distribution
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
#include "Exponential.hxx"
#include "SpecFunc.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Exponential);

static Factory<Exponential> RegisteredFactory("Exponential");

/* Default constructor */
Exponential::Exponential()
  : NonEllipticalDistribution("Exponential"),
    lambda_(1.0),
    gamma_(0.0)
{
  setDimension( 1 );
  computeRange();
}

/* Parameters constructor */
Exponential::Exponential(const NumericalScalar lambda,
                         const NumericalScalar gamma)
  : NonEllipticalDistribution("Exponential"),
    lambda_(0.0),
    gamma_(gamma)
{
  // We set the dimension of the Exponential distribution
  setDimension( 1 );
  // This call check lambda and set also the range.
  setLambda(lambda);
}

/* Comparison operator */
Bool Exponential::operator ==(const Exponential & other) const
{
  if (this == &other) return true;
  return (lambda_ == other.lambda_) && (gamma_ == other.gamma_);
}

/* String converter */
String Exponential::__repr__() const
{
  OSS oss;
  oss << "class=" << Exponential::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " lambda=" << lambda_
      << " gamma=" << gamma_;
  return oss;
}

String Exponential::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(lambda = " << lambda_ << ", gamma = " << gamma_ << ")";
  return oss;
}

/* Virtual constructor */
Exponential * Exponential::clone() const
{
  return new Exponential(*this);
}

/* Get one realization of the distribution */
NumericalPoint Exponential::getRealization() const
{
  return NumericalPoint(1, gamma_ - log(RandomGenerator::Generate()) / lambda_);
}


/* Get the DDF of the distribution */
NumericalPoint Exponential::computeDDF(const NumericalPoint & point) const
{
  if (point[0] < gamma_) return NumericalPoint(1, 0.0);
  return NumericalPoint(1, -lambda_ * computePDF(point));
}


/* Get the PDF of the distribution */
NumericalScalar Exponential::computePDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  NumericalScalar x(point[0] - gamma_);
  if (x < 0.0) return 0.0;
  return lambda_ * exp(-lambda_ * x);
}

NumericalScalar Exponential::computeLogPDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  NumericalScalar x(point[0] - gamma_);
  if (x < 0.0) return -SpecFunc::MaxNumericalScalar;
  return log(lambda_) - lambda_ * x;
}

/* Get the CDF of the distribution */
NumericalScalar Exponential::computeCDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0] - gamma_);
  if (x < 0.0) return 0.0;
  return 1.0 - exp(-lambda_ * x);
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex Exponential::computeCharacteristicFunction(const NumericalScalar x) const
{
  return exp(NumericalComplex(0.0, x * gamma_)) / NumericalComplex(1.0, -x / lambda_);
}

NumericalComplex Exponential::computeLogCharacteristicFunction(const NumericalScalar x) const
{
  return NumericalComplex(0.0, x * gamma_) - log(NumericalComplex(1.0, - x / lambda_));
}

/* Get the PDFGradient of the distribution */
NumericalPoint Exponential::computePDFGradient(const NumericalPoint & point) const
{
  NumericalScalar x(point[0] - gamma_);
  NumericalPoint pdfGradient(2, 0.0);
  if (x < 0.0) return pdfGradient;
  NumericalScalar expX(exp(-lambda_ * x));
  pdfGradient[0] = (1.0 - lambda_ * x) * expX;
  pdfGradient[1] = lambda_ * lambda_ * expX;
  return pdfGradient;
}

/* Get the CDFGradient of the distribution */
NumericalPoint Exponential::computeCDFGradient(const NumericalPoint & point) const
{
  NumericalScalar x(point[0] - gamma_);
  NumericalPoint cdfGradient(2, 0.0);
  if (x < 0.0) return cdfGradient;
  NumericalScalar expX(exp(-lambda_ * x));
  cdfGradient[0] = x * expX;
  cdfGradient[1] = -lambda_ * expX;
  return cdfGradient;
}

/* Get the quantile of the distribution */
NumericalScalar Exponential::computeScalarQuantile(const NumericalScalar prob,
                                                   const Bool tail,
                                                   const NumericalScalar precision) const
{
  if (tail) return gamma_ - log(prob) / lambda_;
  return gamma_ - log1p(-prob) / lambda_;
}

/* Compute the mean of the distribution */
void Exponential::computeMean() const
{
  mean_ = NumericalPoint(1, gamma_ + 1.0 / lambda_);
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Exponential::getStandardDeviation() const
{
  return NumericalPoint(1, 1.0 / lambda_);
}

/* Get the skewness of the distribution */
NumericalPoint Exponential::getSkewness() const
{
  return NumericalPoint(1, 2.0);
}

/* Get the kurtosis of the distribution */
NumericalPoint Exponential::getKurtosis() const
{
  return NumericalPoint(1, 9.0);
}

/* Compute the covariance of the distribution */
void Exponential::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  covariance_(0, 0) = 1.0 / (lambda_ * lambda_);
  isAlreadyComputedCovariance_ = true;
}

/* Get the moments of the standardized distribution */
NumericalPoint Exponential::getStandardMoment(const UnsignedLong n) const
{
  return NumericalPoint(1, SpecFunc::Gamma(n + 1));
}

/* Get the standard representative in the parametric family, associated with the standard moments */
Exponential::Implementation Exponential::getStandardRepresentative() const
{
  return Exponential(1.0).clone();
}

/* Parameters value and description accessor */
Exponential::NumericalPointWithDescriptionCollection Exponential::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(2);
  Description description(point.getDimension());
  point[0] = lambda_;
  point[1] = gamma_;
  description[0] = "lambda";
  description[1] = "gamma";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void Exponential::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = Exponential(parametersCollection[0][0], parametersCollection[0][1]);
}

/* Lambda accessor */
void Exponential::setLambda(const NumericalScalar lambda)
{
  if (lambda <= 0.) throw InvalidArgumentException(HERE) << "Lambda MUST be positive";
  if (lambda != lambda_)
    {
      lambda_ = lambda;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

/* Lambda accessor */
NumericalScalar Exponential::getLambda() const
{
  return lambda_;
}


/* Gamma accessor */
void Exponential::setGamma(const NumericalScalar gamma)
{
  if (gamma != gamma_)
    {
      gamma_ = gamma;
      isAlreadyComputedMean_ = false;
      // The covariance does not depend on gamma
      computeRange();
    }
}

/* Gamma accessor */
NumericalScalar Exponential::getGamma() const
{
  return gamma_;
}

/* Compute the numerical range of the distribution given the parameters values */
void Exponential::computeRange()
{
  setRange(Interval(NumericalPoint(1, gamma_), NumericalPoint(1, computeScalarQuantile(cdfEpsilon_, true)), Interval::BoolCollection(1, true), Interval::BoolCollection(1, false)));
}

/* Method save() stores the object through the StorageManager */
void Exponential::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "lambda_", lambda_ );
  adv.saveAttribute( "gamma_", gamma_ );
}

/* Method load() reloads the object from the StorageManager */
void Exponential::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "lambda_", lambda_ );
  adv.loadAttribute( "gamma_", gamma_ );
  computeRange();
}




END_NAMESPACE_OPENTURNS
