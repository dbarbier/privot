//                                               -*- C++ -*-
/**
 *  @file  ChiSquare.cxx
 *  @brief The ChiSquare distribution
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
 *  @date   2008-11-23 16:03:50 +0100 (dim, 23 nov 2008)
 */
#include <cmath>
#include "ChiSquare.hxx"
#include "Gamma.hxx"
#include "RandomGenerator.hxx"
#include "SpecFunc.hxx"
#include "DistFunc.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ChiSquare);

static Factory<ChiSquare> RegisteredFactory("ChiSquare");

/* Default constructor */
ChiSquare::ChiSquare()
  : NonEllipticalDistribution("ChiSquare"),
    nu_(1.0), normalizationFactor_(1.0)
{
  setDimension(1);
  computeRange();
}

/* Parameters constructor */
ChiSquare::ChiSquare(const NumericalScalar nu)
  : NonEllipticalDistribution("ChiSquare"),
    nu_(0.0), normalizationFactor_(0.0)
{
  // This call set also the range.
  setNu(nu);
  setDimension(1);
  update();
}

/* Comparison operator */
Bool ChiSquare::operator ==(const ChiSquare & other) const
{
  if (this == &other) return true;
  return nu_ == other.nu_;
}

/* String converter */
String ChiSquare::__repr__() const
{
  OSS oss;
  oss << "class=" << ChiSquare::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " nu=" << nu_;
  return oss;
}

String ChiSquare::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(nu = " << nu_ << ")";
  return oss;
}

/* Nu accessor */
void ChiSquare::setNu(const NumericalScalar nu)
{
  if (nu <= 0.0) throw InvalidArgumentException(HERE) << "Nu MUST be positive";
  if (nu != nu_)
    {
      nu_ = nu;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
      update();
    }
}

NumericalScalar ChiSquare::getNu() const
{
  return nu_;
}


/* Virtual constructor */
ChiSquare * ChiSquare::clone() const
{
  return new ChiSquare(*this);
}

/* Compute the numerical range of the distribution given the parameters values */
void ChiSquare::computeRange()
{
  setRange(Gamma(0.5, 0.5 * nu_, 0.0).getRange());
}

/** Update the derivative attributes */
void ChiSquare::update()
{
  normalizationFactor_ = pow(0.5, 0.5 * nu_) / SpecFunc::Gamma(0.5 * nu_);
}


/* Get one realization of the distribution */
NumericalPoint ChiSquare::getRealization() const
{
  return NumericalPoint(1, 2.0 * DistFunc::rGamma(0.5 * nu_));
}


/* Get the DDF of the distribution */
NumericalPoint ChiSquare::computeDDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  if (x <= 0.0) return NumericalPoint(1, 0.0);
  return NumericalPoint(1, ((0.5 * nu_ - 1.0) / x - 0.5) * computePDF(point));
}


/* Get the PDF of the distribution */
NumericalScalar ChiSquare::computePDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  if (x <= 0.0) return 0.0;
  return normalizationFactor_ * pow(x, 0.5 * nu_ - 1) * exp(-0.5 * x);
}


/* Get the CDF of the distribution */
NumericalScalar ChiSquare::computeCDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  // No test here as the CDF is continuous for all nu_
  if (x <= 0.0) return 0.0;
  return DistFunc::pGamma(0.5 * nu_, 0.5 * x);
}

NumericalScalar ChiSquare::computeComplementaryCDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  // No test here as the CDF is continuous for all nu_
  if (x <= 0.0) return 1.0;
  return DistFunc::pGamma(0.5 * nu_, 0.5 * x, true);
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex ChiSquare::computeCharacteristicFunction(const NumericalScalar x) const
{
  return pow(NumericalComplex(1.0, -2.0 * x), -0.5 * nu_);
}

NumericalComplex ChiSquare::computeLogCharacteristicFunction(const NumericalScalar x) const
{
  return -0.5 * nu_ * log(NumericalComplex(1.0, -2.0 * x));
}

/* Get the PDFGradient of the distribution */
NumericalPoint ChiSquare::computePDFGradient(const NumericalPoint & point) const
{
  NumericalPoint pdfGradient(1, 0.0);
  const NumericalScalar x(point[0]);
  if (x <= 0.0) return pdfGradient;
  NumericalScalar pdf(computePDF(point));
  pdfGradient[0] = 0.5 * (log(0.5 * x) - SpecFunc::Psi(0.5 * nu_)) * pdf;
  return pdfGradient;
}

/* Get the CDFGradient of the distribution */
NumericalPoint ChiSquare::computeCDFGradient(const NumericalPoint & point) const
{
  NumericalPoint cdfGradient(1, 0.0);
  const NumericalScalar x(point[0]);
  if (x <= 0.0) return cdfGradient;
  NumericalScalar eps(pow(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"), 1.0 / 3.0));
  cdfGradient[0] = (DistFunc::pGamma(0.5 * (nu_ + eps), 0.5 * x) - DistFunc::pGamma(0.5 * (nu_ - eps), 0.5 * x)) / (2.0 * eps);
  return cdfGradient;
}

/* Get the quantile of the distribution */
NumericalScalar ChiSquare::computeScalarQuantile(const NumericalScalar prob,
                                                 const Bool tail,
                                                 const NumericalScalar precision) const
{
  return 2.0 * DistFunc::qGamma(0.5 * nu_, prob, tail);
}

/* Compute the mean of the distribution */
void ChiSquare::computeMean() const
{
  mean_ = NumericalPoint(1, nu_);
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint ChiSquare::getStandardDeviation() const
{
  return NumericalPoint(1, sqrt(2.0 * nu_));
}

/* Get the skewness of the distribution */
NumericalPoint ChiSquare::getSkewness() const
{
  return NumericalPoint(1, sqrt(8.0 / nu_));
}

/* Get the kurtosis of the distribution */
NumericalPoint ChiSquare::getKurtosis() const
{
  return NumericalPoint(1, 3.0 + 12.0 / nu_);
}

/* Compute the covariance of the distribution */
void ChiSquare::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  covariance_(0, 0) = 2.0 * nu_;
  isAlreadyComputedCovariance_ = true;
}

/* Get the moments of the standardized distribution */
NumericalPoint ChiSquare::getStandardMoment(const UnsignedLong n) const
{
  return NumericalPoint(1, exp(n * M_LN2 + SpecFunc::LnGamma(n + 0.5 * nu_) - SpecFunc::LnGamma(0.5 * nu_)));
}

/* Parameters value and description accessor */
ChiSquare::NumericalPointWithDescriptionCollection ChiSquare::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(1);
  Description description(point.getDimension());
  point[0] = nu_;
  description[0] = "nu";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void ChiSquare::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = ChiSquare(parametersCollection[0][0]);
}

/* Method save() stores the object through the StorageManager */
void ChiSquare::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "nu_", nu_ );
  adv.saveAttribute( "normalizationFactor_", normalizationFactor_ );
}

/* Method load() reloads the object from the StorageManager */
void ChiSquare::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "nu_", nu_ );
  adv.loadAttribute( "normalizationFactor_", normalizationFactor_ );
  computeRange();
}




END_NAMESPACE_OPENTURNS
