//                                               -*- C++ -*-
/**
 *  @file  Chi.cxx
 *  @brief The Chi distribution
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
#include "Chi.hxx"
#include "Gamma.hxx"
#include "RandomGenerator.hxx"
#include "SpecFunc.hxx"
#include "DistFunc.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(Chi);

static Factory<Chi> RegisteredFactory("Chi");

/* Default constructor */
Chi::Chi()
  : NonEllipticalDistribution("Chi"),
    nu_(1.0), normalizationFactor_(1.0)
{
  setDimension(1);
  computeRange();
}

/* Parameters constructor */
Chi::Chi(const NumericalScalar nu)
  : NonEllipticalDistribution("Chi"),
    nu_(0.0), normalizationFactor_(0.0)
{
  setNu(nu);
  setDimension(1);
  computeRange();
  update();
}

/* Comparison operator */
Bool Chi::operator ==(const Chi & other) const
{
  if (this == &other) return true;
  return nu_ == other.nu_;
}

/* String converter */
String Chi::__repr__() const
{
  OSS oss;
  oss << "class=" << Chi::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " nu=" << nu_;
  return oss;
}

String Chi::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(nu = " << nu_ << ")";
  return oss;
}

/* Nu accessor */
void Chi::setNu(const NumericalScalar nu)
{
  if (nu <= 0.0) throw InvalidArgumentException(HERE) << "Nu MUST be positive";
  if (nu_ != nu)
    {
      nu_ = nu;
      update();
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
    }
}

NumericalScalar Chi::getNu() const
{
  return nu_;
}


/* Virtual constructor */
Chi * Chi::clone() const
{
  return new Chi(*this);
}

/* Compute the numerical range of the distribution given the parameters values */
void Chi::computeRange()
{
  const NumericalPoint lowerBound(1, 0.0);
  const NumericalPoint upperBound(computeUpperBound());
  const Interval::BoolCollection finiteLowerBound(1, true);
  const Interval::BoolCollection finiteUpperBound(1, false);
  setRange(Interval(lowerBound, upperBound, finiteLowerBound, finiteUpperBound));

}

/** Update the derivative attributes */
void Chi::update()
{
  normalizationFactor_ = (1.0 - 0.5 * nu_) * M_LN2 - SpecFunc::LnGamma(0.5 * nu_);
}


/* Get one realization of the distribution */
NumericalPoint Chi::getRealization() const
{
  return NumericalPoint(1, M_SQRT2 * sqrt(DistFunc::rGamma(0.5 * nu_)));
}


/* Get the DDF of the distribution */
NumericalPoint Chi::computeDDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0]);
  if (x <= 0.0) return NumericalPoint(1, 0.0);
  return NumericalPoint(1, (( nu_ - 1.0) / x - x) * computePDF(point));
}


/* Get the PDF of the distribution */
NumericalScalar Chi::computePDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0]);
  if (x <= 0.0) return 0.0;
  return exp( normalizationFactor_ + (nu_ - 1) * log(x) - 0.5 * x * x );
}


/* Get the CDF of the distribution */
NumericalScalar Chi::computeCDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0]);
  // No test here as the CDF is continuous for all nu_
  if (x <= 0.0) return 0.0;
  return DistFunc::pGamma(0.5 * nu_, 0.5 * x * x);
}

NumericalScalar Chi::computeComplementaryCDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0]);
  // No test here as the CDF is continuous for all nu_
  if (x <= 0.0) return 1.0;
  return DistFunc::pGamma(0.5 * nu_, 0.5 * x * x, true);
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X))
   Its value here is phi(u) = M(0.5 * \nu, 0.5, -0.5 * u ^ 2) + \sqrt(2) * \Gamma((\nu + 1) * 0.5) * M(1.5 * \nu, 1.5, -0.5 * u^2)/ \Gamma(\nu * 0.5) * \imath where
   M(a, b, c) is the hypergeometric function given by M(p1, q1, x)= sum_{n = 0}^{\infty} [prod_{k = 0} ^ {n - 1} (p1 + k) / (q1 + k)] * x^n / n! */
NumericalComplex Chi::computeCharacteristicFunction(const NumericalScalar x) const
{
  const NumericalScalar t( -0.5 * x * x);
  const NumericalScalar real(SpecFunc::HyperGeom_1_1(0.5 * nu_, 0.5, t ));
  const NumericalScalar imag(M_SQRT2 * t * exp(SpecFunc::LnGamma((nu_ + 1.0) * 0.5) - SpecFunc::LnGamma(0.5 * nu_)) * SpecFunc::HyperGeom_1_1((nu_ + 1.0) * 0.5, 1.5, t));
  const NumericalComplex result(real, imag);
  return result;
}

/* Get the PDFGradient of the distribution */
NumericalPoint Chi::computePDFGradient(const NumericalPoint & point) const
{
  NumericalPoint pdfGradient(1, 0.0);
  NumericalScalar x(point[0]);
  if (x <= 0.0) return pdfGradient;
  NumericalScalar pdf(computePDF(point));
  /*        pdfGradient[0] = 0.5 * (2. * log(x / sqrt(2)) - SpecFunc::Psi(0.5 * nu_)) * pdf;*/
  pdfGradient[0] = 0.5 * (2. * log(x / M_SQRT2) - SpecFunc::Psi(0.5 * nu_)) * pdf;
  return pdfGradient;
}

/* Get the CDFGradient of the distribution */
NumericalPoint Chi::computeCDFGradient(const NumericalPoint & point) const
{
  NumericalPoint cdfGradient(1, 0.0);
  NumericalScalar x(point[0]);
  if (x <= 0.0) return cdfGradient;
  NumericalScalar eps(pow(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"), 1.0 / 3.0));
  cdfGradient[0] = (DistFunc::pGamma(0.5 * (nu_ + eps), 0.5 * x * x) - DistFunc::pGamma(0.5 * (nu_ - eps), 0.5 * x * x)) / (2.0 * eps);
  return cdfGradient;
}

/* Get the quantile of the distribution */
NumericalScalar Chi::computeScalarQuantile(const NumericalScalar prob,
                                           const Bool tail,
                                           const NumericalScalar precision) const
{
  return M_SQRT2 * sqrt(DistFunc::qGamma(0.5 * nu_, prob, tail));
}


void Chi::computeMean() const
{
  mean_ = NumericalPoint(1, M_SQRT2 * exp( SpecFunc::LnGamma(0.5 * (nu_ + 1.0)) - SpecFunc::LnGamma(0.5 * nu_)));
  isAlreadyComputedMean_ = true;
}

void Chi::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  covariance_(0, 0) = nu_ - 2.0 * exp(2.0 * (SpecFunc::LnGamma(0.5 * (nu_ + 1.0)) - SpecFunc::LnGamma(0.5 * nu_)));
  isAlreadyComputedCovariance_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Chi::getStandardDeviation() const
{
  return NumericalPoint(1, sqrt(getCovariance()(0, 0)) );
}

/* Get the skewness of the distribution */
NumericalPoint Chi::getSkewness() const
{
  const NumericalScalar mu(getMean()[0]);
  const NumericalScalar sigma(getStandardDeviation()[0]);
  return NumericalPoint(1, mu * (1 - 2.0 * sigma * sigma) / pow(sigma, 3.0));
}

/* Get the kurtosis of the distribution */
NumericalPoint Chi::getKurtosis() const
{
  const NumericalScalar mu(getMean()[0]);
  const NumericalScalar sigma(getStandardDeviation()[0]);
  const NumericalScalar gamma1(getSkewness()[0]);
  return NumericalPoint(1, 3.0 + 2.0 * (1.0 - sigma * (mu * gamma1 + sigma)) / pow(sigma, 2.0));
}

/* Get the moments of the standardized distribution */
NumericalPoint Chi::getStandardMoment(const UnsignedLong n) const
{
  return NumericalPoint(1, exp(0.5 * n * M_LN2 + SpecFunc::LnGamma(0.5 * (n + nu_)) - SpecFunc::LnGamma(0.5 * nu_)));
}

/* Parameters value and description accessor */
Chi::NumericalPointWithDescriptionCollection Chi::getParametersCollection() const
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

void Chi::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = Chi(parametersCollection[0][0]);
}

/* Method save() stores the object through the StorageManager */
void Chi::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "nu_", nu_ );
  adv.saveAttribute( "normalizationFactor_", normalizationFactor_ );
}

/* Method load() reloads the object from the StorageManager */
void Chi::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "nu_", nu_ );
  adv.loadAttribute( "normalizationFactor_", normalizationFactor_ );
  update();
  computeRange();
}

END_NAMESPACE_OPENTURNS
