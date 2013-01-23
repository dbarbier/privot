//                                               -*- C++ -*-
/**
 *  @file  Rice.cxx
 *  @brief The Rice distribution
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
#include "Rice.hxx"
#include "SpecFunc.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"
#include "DistFunc.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Rice);

static Factory<Rice> RegisteredFactory("Rice");

/* Default constructor */
Rice::Rice()
  : NonEllipticalDistribution("Rice")
  , sigma_(1.0)
  , nu_(0.0)
{
  setDimension(1);
  computeRange();
}

/* Parameters constructor */
Rice::Rice(const NumericalScalar sigma,
           const NumericalScalar nu)
  : NonEllipticalDistribution("Rice")
  , sigma_(0.0)
  , nu_(nu)
{
  // This call sets also the range
  setSigma(sigma);
  setDimension(1);
}

/* Comparison operator */
Bool Rice::operator ==(const Rice & other) const
{
  if (this == &other) return true;
  return (sigma_ == other.sigma_) && (nu_ == other.nu_);
}

/* String converter */
String Rice::__repr__() const
{
  OSS oss;
  oss << "class=" << Rice::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " sigma=" << sigma_
      << " nu=" << nu_;
  return oss;
}

String Rice::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(sigma = " << sigma_ << ", nu = " << nu_ << ")";
  return oss;
}

/* Virtual constructor */
Rice * Rice::clone() const
{
  return new Rice(*this);
}

/* Compute the numerical range of the distribution given the parameters values */
void Rice::computeRange()
{
  const NumericalPoint lowerBound(1, 0.0 );
  const NumericalPoint upperBound(computeUpperBound());
  const Interval::BoolCollection finiteLowerBound(1, true);
  const Interval::BoolCollection finiteUpperBound(1, false);
  setRange(Interval(lowerBound, upperBound, finiteLowerBound, finiteUpperBound));
}


/* Get one realization of the distribution */
NumericalPoint Rice::getRealization() const
{
  const NumericalScalar lambda(pow(nu_ / sigma_, 2));
  return NumericalPoint(1, sigma_ * sqrt(DistFunc::rNonCentralChiSquare(2, lambda )) ) ;
}


/* Get the PDF of the distribution */
NumericalScalar Rice::computePDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const NumericalScalar x(point[0]);
  if (x <= 0.0) return 0.0;
  const NumericalScalar lambda(pow(nu_ / sigma_, 2));
  const NumericalScalar z(x * pow(1.0 / sigma_, 2));
  return 2.0 * z * DistFunc::dNonCentralChiSquare(2, lambda , x * z ) ;
}


/* Get the CDF of the distribution */
NumericalScalar Rice::computeCDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  if (point[0] <= 0.0) return 0.0;
  const NumericalScalar lambda(pow(nu_ / sigma_, 2));
  const NumericalScalar y(pow(point[0] / sigma_, 2));
  return DistFunc::pNonCentralChiSquare(2, lambda , y );
}

NumericalScalar Rice::computeComplementaryCDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  if (point[0] <= 0.0) return 1.0;
  const NumericalScalar lambda(pow(nu_ / sigma_, 2));
  const NumericalScalar y(pow(point[0] / sigma_, 2));
  return DistFunc::pNonCentralChiSquare(2, lambda , y, true );
}

/* Compute the mean of the distribution */
void Rice::computeMean() const
{
  //1.253314137315500251207882 = sqrt(pi/2)
  const NumericalScalar x(-0.5 * pow(nu_ / sigma_, 2) );
  mean_ = NumericalPoint(1, sigma_ * 1.253314137315500251207882 * SpecFunc::HyperGeom_1_1(-0.5, 1, x));
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Rice::getStandardDeviation() const
{
  if (!isAlreadyComputedCovariance_) computeCovariance();
  return NumericalPoint(1, sqrt(covariance_(0, 0)));
}

/* Get the moments of the standardized distribution */
NumericalPoint Rice::getStandardMoment(const UnsignedLong n) const
{
  if (n == 0) return NumericalPoint(1, 1.0);
  return NumericalPoint(1, pow(2, 0.5 * n) * SpecFunc::Gamma(1.0 + 0.5 * n) * SpecFunc::HyperGeom_1_1(-0.5 * n, 1.0, -0.5 * nu_ * nu_ / (sigma_ * sigma_)));
}

/* Get the standard representative in the parametric family, associated with the standard moments */
Rice::Implementation Rice::getStandardRepresentative() const
{
  return Rice(1.0, nu_ / sigma_).clone();
}


/* Compute the covariance of the distribution */
void Rice::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  NumericalScalar covariance(0.0);
  const NumericalScalar mu(getMean()[0]);
  covariance = 2.0 * sigma_ * sigma_ + (nu_ - mu) * (nu_ + mu);
  covariance_(0, 0) = covariance;
  isAlreadyComputedCovariance_ = true;
}

/* Parameters value and description accessor */
Rice::NumericalPointWithDescriptionCollection Rice::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(2);
  Description description(point.getDimension());
  point[0] = sigma_;
  description[0] = "sigma";
  point[1] = nu_;
  description[1] = "nu";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void Rice::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = Rice(parametersCollection[0][0], parametersCollection[0][1]);
}


/* Sigma accessor */
void Rice::setSigma(const NumericalScalar sigma)
{
  if (sigma <= 0.0) throw InvalidArgumentException(HERE) << "Sigma MUST be positive";
  if (sigma != sigma_)
    {
      sigma_ = sigma;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

NumericalScalar Rice::getSigma() const
{
  return sigma_;
}

/* Nu accessor */
void Rice::setNu(const NumericalScalar nu)
{
  if (nu <= 0.0) throw InvalidArgumentException(HERE) << "Nu MUST be positive";
  if (nu != nu_)
    {
      nu_ = nu;
      isAlreadyComputedMean_ = false;
      // The covariance does not depend on nu
      computeRange();
    }
}

NumericalScalar Rice::getNu() const
{
  return nu_;
}

/* Method save() stores the object through the StorageManager */
void Rice::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "sigma_", sigma_ );
  adv.saveAttribute( "nu_", nu_ );
}

/* Method load() reloads the object from the StorageManager */
void Rice::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "sigma_", sigma_ );
  adv.loadAttribute( "nu_", nu_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
