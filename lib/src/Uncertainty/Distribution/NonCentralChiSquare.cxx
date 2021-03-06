//                                               -*- C++ -*-
/**
 *  @file  NonCentralChiSquare.cxx
 *  @brief The NonCentralChiSquare distribution
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
 *  @date   2011-02-26 21:12:17 +0100 (sam. 26 févr. 2011)
 */
#include <cmath>
#include "NonCentralChiSquare.hxx"
#include "RandomGenerator.hxx"
#include "SpecFunc.hxx"
#include "DistFunc.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(NonCentralChiSquare);

static Factory<NonCentralChiSquare> RegisteredFactory("NonCentralChiSquare");


/* Default constructor */
NonCentralChiSquare::NonCentralChiSquare(const NumericalScalar nu,
                                         const NumericalScalar lambda)
  : NonEllipticalDistribution("NonCentralChiSquare"),
    nu_(0.0),
    lambda_(0.0)
{
  // This call set also the range.
  setNuLambda(nu, lambda);
  setDimension(1);
}

/* Comparison operator */
Bool NonCentralChiSquare::operator ==(const NonCentralChiSquare & other) const
{
  if (this == &other) return true;
  return (nu_ == other.nu_) && (lambda_ == other.lambda_);
}

/* String converter */
String NonCentralChiSquare::__repr__() const
{
  OSS oss;
  oss << "class=" << NonCentralChiSquare::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " nu=" << nu_
      << " lambda=" << lambda_;
  return oss;
}

String NonCentralChiSquare::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(nu = " << nu_ << ", lambda = " << lambda_ << ")";
  return oss;
}

/* Virtual constructor */
NonCentralChiSquare * NonCentralChiSquare::clone() const
{
  return new NonCentralChiSquare(*this);
}

/* Get one realization of the distribution */
NumericalPoint NonCentralChiSquare::getRealization() const
{
  return NumericalPoint(1, DistFunc::rNonCentralChiSquare(nu_, lambda_));
}

/* Get the PDF of the distribution */
NumericalScalar NonCentralChiSquare::computePDF(const NumericalPoint & point) const
{
  pdfEpsilon_ = ResourceMap::GetAsNumericalScalar("DistFunc-Precision");
  return DistFunc::dNonCentralChiSquare(nu_, lambda_, point[0]);
}

/* Get the CDF of the distribution */
NumericalScalar NonCentralChiSquare::computeCDF(const NumericalPoint & point) const
{
  cdfEpsilon_ = ResourceMap::GetAsNumericalScalar("DistFunc-Precision");
  return DistFunc::pNonCentralChiSquare(nu_, lambda_, point[0]);
}

/** Get the PDFGradient of the distribution */
NumericalPoint NonCentralChiSquare::computePDFGradient(const NumericalPoint & point) const
{
  NumericalScalar eps(pow(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"), 1.0 / 3.0));
  NumericalPoint pdfGradient(2);
  pdfGradient[0] = (DistFunc::dNonCentralChiSquare(nu_ + eps, lambda_, point[0]) - DistFunc::dNonCentralChiSquare(nu_ - eps, lambda_, point[0])) / (2.0 * eps);
  pdfGradient[1] = (DistFunc::dNonCentralChiSquare(nu_, lambda_ + eps, point[0]) - DistFunc::dNonCentralChiSquare(nu_, lambda_ - eps, point[0])) / (2.0 * eps);
  return pdfGradient;
}

/** Get the CDFGradient of the distribution */
NumericalPoint NonCentralChiSquare::computeCDFGradient(const NumericalPoint & point) const
{
  NumericalScalar eps(pow(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"), 1.0 / 3.0));
  NumericalPoint cdfGradient(2);
  cdfGradient[0] = (DistFunc::pNonCentralChiSquare(nu_ + eps, lambda_, point[0]) - DistFunc::pNonCentralChiSquare(nu_ - eps, lambda_, point[0])) / (2.0 * eps);
  cdfGradient[1] = (DistFunc::pNonCentralChiSquare(nu_, lambda_ + eps, point[0]) - DistFunc::pNonCentralChiSquare(nu_, lambda_ - eps, point[0])) / (2.0 * eps);
  return cdfGradient;
}

/* Compute the mean of the distribution */
void NonCentralChiSquare::computeMean() const
{
  mean_ = NumericalPoint(1, nu_ + lambda_);
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint NonCentralChiSquare::getStandardDeviation() const
{
  return NumericalPoint(1, sqrt(2.0 * (nu_ + 2.0 * lambda_)));
}

/* Get the skewness of the distribution */
NumericalPoint NonCentralChiSquare::getSkewness() const
{
  if (nu_ == 0.0) throw NotDefinedException(HERE) << "Error: the skewness is not defined for nu=0.";
  if (lambda_ == 0.0) return NumericalPoint(1, 2.0 * M_SQRT2 / sqrt(nu_));
  return NumericalPoint(1, (nu_ + 3.0 * lambda_) * pow(2.0 / (nu_ + 2.0 * lambda_), 1.5));
}

/* Get the kurtosis of the distribution */
NumericalPoint NonCentralChiSquare::getKurtosis() const
{
  if (nu_ == 0.0) throw NotDefinedException(HERE) << "Error: the kurtosis is not defined for nu=0.";
  if (lambda_ == 0.0) return NumericalPoint(1, 3.0 + 12.0 / nu_);
  return NumericalPoint(1, 3.0 + 12.0 * (nu_ + 4.0 * lambda_) / pow(nu_ + 2.0 * lambda_, 2.0));
}

/* Get the moments of the standardized distribution */
NumericalPoint NonCentralChiSquare::getStandardMoment(const UnsignedLong n) const
{
  UniVariatePolynomial p(NumericalPoint(1, 1.0));
  NumericalPoint derivativeFactor(3);
  derivativeFactor[0] = 1.0;
  derivativeFactor[1] = -4.0;
  derivativeFactor[2] = 4.0;
  for (UnsignedLong k = 0; k < n; ++k)
    {
      NumericalPoint polynomialFactor(2);
      polynomialFactor[0] = lambda_ + 4.0 * k + nu_;
      polynomialFactor[1] = -8.0 * k - 2.0 * nu_;
      p = p.derivate() * UniVariatePolynomial(derivativeFactor) + p * UniVariatePolynomial(polynomialFactor);
    }
  return NumericalPoint(1, p.getCoefficients()[0]);
}

/* Compute the covariance of the distribution */
void NonCentralChiSquare::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  covariance_(0, 0) =  2.0 * (nu_ + 2.0 * lambda_);
  isAlreadyComputedCovariance_ = true;
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex NonCentralChiSquare::computeCharacteristicFunction(const NumericalScalar x) const
{
  return exp(computeLogCharacteristicFunction(x));
}

NumericalComplex NonCentralChiSquare::computeLogCharacteristicFunction(const NumericalScalar x) const
{
  const NumericalComplex denominator(1.0, -2.0 * x);
  return NumericalComplex(0.0, lambda_ * x) / denominator - 0.5 * nu_ * log(denominator);
}

/* Parameters value and description accessor */
NonCentralChiSquare::NumericalPointWithDescriptionCollection NonCentralChiSquare::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(2);
  Description description(point.getDimension());
  point[0] = nu_;
  point[1] = lambda_;
  description[0] = "nu";
  description[1] = "lambda";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void NonCentralChiSquare::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = NonCentralChiSquare(parametersCollection[0][0], parametersCollection[0][1]);
}

/* Nu accessor */
void NonCentralChiSquare::setNu(const NumericalScalar nu)
{
  if (nu < 0.0) throw InvalidArgumentException(HERE) << "Nu MUST be strictly positive";
  if (nu != nu_)
    {
      nu_ = nu;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

void NonCentralChiSquare::setNuLambda(const NumericalScalar nu,
                                      const NumericalScalar lambda)
{
  if (nu < 0.0) throw InvalidArgumentException(HERE) << "Nu MUST be strictly positive";
  if (lambda < 0.0) throw InvalidArgumentException(HERE) << "Lambda MUST be strictly positive";
  if ((nu != nu_) || (lambda != lambda_))
    {
      nu_ = nu;
      lambda_ = lambda;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

/* Nu accessor */
NumericalScalar NonCentralChiSquare::getNu() const
{
  return nu_;
}


/* Lambda accessor */
void NonCentralChiSquare::setLambda(const NumericalScalar lambda)
{
  if (lambda < 0.0) throw InvalidArgumentException(HERE) << "Lambda MUST be strictly positive";
  if (lambda != lambda_)
    {
      lambda_ = lambda;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

/* Lambda accessor */
NumericalScalar NonCentralChiSquare::getLambda() const
{
  return lambda_;
}

/* Method save() stores the object through the StorageManager */
void NonCentralChiSquare::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "nu_", nu_ );
  adv.saveAttribute( "lambda_", lambda_ );
}

/* Method load() reloads the object from the StorageManager */
void NonCentralChiSquare::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "nu_", nu_ );
  adv.loadAttribute( "lambda_", lambda_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
