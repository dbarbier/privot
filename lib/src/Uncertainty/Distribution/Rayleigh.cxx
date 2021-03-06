//                                               -*- C++ -*-
/**
 *  @file  Rayleigh.cxx
 *  @brief The Rayleigh distribution
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
 *  @date   2008-09-24 09:07:16 +0200 (mer, 24 sep 2008)
 */
#include <cmath>
#include "Rayleigh.hxx"
#include "SpecFunc.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(Rayleigh);

static Factory<Rayleigh> RegisteredFactory("Rayleigh");

/* Default constructor */
Rayleigh::Rayleigh()
  : NonEllipticalDistribution("Rayleigh"),
    sigma_(1.0),
    gamma_(0.0)
{
  setDimension(1);
  computeRange();
}

/* Parameters constructor */
Rayleigh::Rayleigh(const NumericalScalar sigma,
                   const NumericalScalar gamma)
  : NonEllipticalDistribution("Rayleigh"),
    sigma_(0.0), gamma_(gamma)
{
  // This call set also the range
  setSigma(sigma);
  setDimension(1);
}

/* Comparison operator */
Bool Rayleigh::operator ==(const Rayleigh & other) const
{
  if (this == &other) return true;
  return (sigma_ == other.sigma_) && (gamma_ == other.gamma_);
}

/* String converter */
String Rayleigh::__repr__() const
{
  OSS oss;
  oss << "class=" << Rayleigh::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " sigma=" << sigma_
      << " gamma=" << gamma_;
  return oss;
}

String Rayleigh::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(sigma = " << sigma_ << ", gamma = " << gamma_ << ")";
  return oss;
}

/* Virtual constructor */
Rayleigh * Rayleigh::clone() const
{
  return new Rayleigh(*this);
}

/* Compute the numerical range of the distribution given the parameters values */
void Rayleigh::computeRange()
{
  const NumericalPoint lowerBound(1, gamma_);
  const NumericalPoint upperBound(computeUpperBound());
  const Interval::BoolCollection finiteLowerBound(1, true);
  const Interval::BoolCollection finiteUpperBound(1, false);
  setRange(Interval(lowerBound, upperBound, finiteLowerBound, finiteUpperBound));
}


/* Get one realization of the distribution */
NumericalPoint Rayleigh::getRealization() const
{
  return NumericalPoint(1, gamma_ + sigma_ * sqrt(-2.0 * log(RandomGenerator::Generate())));
}


/* Get the DDF of the distribution */
NumericalPoint Rayleigh::computeDDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0] - gamma_);
  if (x <= 0.0) return NumericalPoint(1, 0.0);
  const NumericalScalar y(x / sigma_);
  const NumericalScalar sigma2(sigma_ * sigma_);
  return NumericalPoint(1, -exp(-0.5 * y * y) * (x - sigma_) * (x + sigma_) / (sigma2 * sigma2));
}


/* Get the PDF of the distribution */
NumericalScalar Rayleigh::computePDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const NumericalScalar x(point[0] - gamma_);
  if (x <= 0.0) return 0.0;
  const NumericalScalar y(x / (sigma_ * sigma_));
  return y * exp(-0.5 * x * y);
}

NumericalScalar Rayleigh::computeLogPDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const NumericalScalar x(point[0] - gamma_);
  if (x <= 0.0) return -SpecFunc::MaxNumericalScalar;
  const NumericalScalar y(x / (sigma_ * sigma_));
  return log(y) - 0.5 * x * y;
}


/* Get the CDF of the distribution */
NumericalScalar Rayleigh::computeCDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0] - gamma_);
  if (x <= 0.0) return 0.0;
  const NumericalScalar y(x / sigma_);
  return 1.0 - exp(-0.5 * y * y);
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X))
   phi(x) = (1 - sigma * x * exp(-sigma^2 * x^2 / 2) * sqrt(pi / 2) * (erfi(sigma * x / sqrt(2)) - i)) * exp(i * gamma * x)
   erfi(t) = -i * erf(i * t) = 2 / sqrt(pi) * int(exp(u^2), u=0..t)
   dawson(t) = exp(-t^2) * int(exp(u^2), t=0..t)
   erfi(t) = 2 / sqrt(pi) * exp(t^2) * dawson(t)
   phi(x) = (1 - sigma * x * exp(-sigma^2 * x^2 / 2) * sqrt(pi / 2) * (2 / sqrt(pi) * exp(sigma^2 * x^2 / 2) * dawson(sigma * x / sqrt(2)) - i)) * exp(i * gamma * x)
   = (1 - sigma * x * (sqrt(2) * dawson(sigma * x / sqrt(2)) - i * exp(-sigma^2 * x^2 / 2) * sqrt(pi/2))) * exp(i * gamma * x)
   = (1 - t * (2 * dawson(t) - i * exp(-t * t) * sqrt(pi))) * exp(i * gamma * x) with t = sigma * x / sqrt(2)
*/
NumericalComplex Rayleigh::computeCharacteristicFunction(const NumericalScalar x) const
{
  const NumericalScalar t(sigma_ * x / sqrt(2.0));
  return NumericalComplex(1 - 2 * t * SpecFunc::Dawson(t), t * exp(-t * t) * sqrt(M_PI)) * exp(NumericalComplex(0.0, x * gamma_));
}

/* Get the PDFGradient of the distribution */
NumericalPoint Rayleigh::computePDFGradient(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0] - gamma_);
  NumericalPoint pdfGradient(2, 0.0);
  if (x <= 0.0) return pdfGradient;
  const NumericalScalar sigma2(sigma_ * sigma_);
  const NumericalScalar factor1(computePDF(point) / sigma2);
  const NumericalScalar factor2((x - sigma_) * (x + sigma_));
  pdfGradient[0] = factor1 * (factor2 - sigma2) / sigma_;
  pdfGradient[1] = factor1 *  factor2 / x;
  return pdfGradient;
}

/* Get the CDFGradient of the distribution */
NumericalPoint Rayleigh::computeCDFGradient(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0] - gamma_);
  NumericalPoint cdfGradient(2, 0.0);
  const NumericalScalar pdf(computePDF(point));
  if (x <= 0.0) return cdfGradient;
  cdfGradient[0] = -x * pdf / sigma_;
  cdfGradient[1] = -pdf;
  return cdfGradient;
}

/* Get the quantile of the distribution */
NumericalScalar Rayleigh::computeScalarQuantile(const NumericalScalar prob,
                                                const Bool tail,
                                                const NumericalScalar precision) const
{
  if (tail) return gamma_ + sigma_ * sqrt(-2.0 * log(prob));
  return gamma_ + sigma_ * sqrt(-2.0 * log1p(-prob));
}

/* Compute the mean of the distribution */
void Rayleigh::computeMean() const
{
  // 1.253314137315500251207882 = sqrt(pi/2)
  mean_ = NumericalPoint(1, gamma_ + 1.253314137315500251207882 * sigma_);
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Rayleigh::getStandardDeviation() const
{
  // 0.6551363775620335530939357 = sqrt(2 - pi / 2)
  return NumericalPoint(1, 0.6551363775620335530939357 * sigma_);
}

/* Get the skewness of the distribution */
NumericalPoint Rayleigh::getSkewness() const
{
  // 0.631110657818937138918970 = 2 * sqrt(pi / (4 - pi)) * (pi - 3) / (4 - pi)
  return NumericalPoint(1, 0.631110657818937138918970);
}

/* Get the kurtosis of the distribution */
NumericalPoint Rayleigh::getKurtosis() const
{
  // 3.245089300687638062848667 = 3 - (6 * pi^2 - 24 * pi + 16) / (4 - pi)^2
  return NumericalPoint(1, 3.245089300687638062848667);
}

/* Get the moments of the distribution */
NumericalPoint Rayleigh::getStandardMoment(const UnsignedLong n) const
{
  return NumericalPoint(1, exp(0.5 * n * M_LN2 + SpecFunc::LnGamma(1.0 + 0.5 * n)));
}

/* Get the standard representative in the parametric family, associated with the standard moments */
Rayleigh::Implementation Rayleigh::getStandardRepresentative() const
{
  return Rayleigh(1.0, 0.0).clone();
}

/* Compute the covariance of the distribution */
void Rayleigh::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  // 0.429203673205103380768678 = (4 - pi) / 2
  covariance_(0, 0) = 0.429203673205103380768678 * sigma_ * sigma_;
  isAlreadyComputedCovariance_ = true;
}

/* Parameters value and description accessor */
Rayleigh::NumericalPointWithDescriptionCollection Rayleigh::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(2);
  Description description(point.getDimension());
  point[0] = sigma_;
  description[0] = "sigma";
  point[1] = gamma_;
  description[1] = "gamma";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void Rayleigh::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = Rayleigh(parametersCollection[0][0], parametersCollection[0][1]);
}


/* Sigma accessor */
void Rayleigh::setSigma(const NumericalScalar sigma)
{
  if (sigma <= 0.) throw InvalidArgumentException(HERE) << "Sigma MUST be positive";
  if (sigma != sigma_)
    {
      sigma_ = sigma;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

NumericalScalar Rayleigh::getSigma() const
{
  return sigma_;
}

/* Gamma accessor */
void Rayleigh::setGamma(const NumericalScalar gamma)
{
  if (gamma != gamma_)
    {
      gamma_ = gamma;
      isAlreadyComputedMean_ = false;
      // The covariance does not depend on gamma
      computeRange();
    }
}

NumericalScalar Rayleigh::getGamma() const
{
  return gamma_;
}

/* Method save() stores the object through the StorageManager */
void Rayleigh::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "sigma_", sigma_ );
  adv.saveAttribute( "gamma_", gamma_ );
}

/* Method load() reloads the object from the StorageManager */
void Rayleigh::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "sigma_", sigma_ );
  adv.loadAttribute( "gamma_", gamma_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
