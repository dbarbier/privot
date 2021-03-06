//                                               -*- C++ -*-
/**
 *  @file  NonCentralStudent.cxx
 *  @brief The NonCentralStudent distribution
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
#include "NonCentralStudent.hxx"
#include "RandomGenerator.hxx"
#include "SpecFunc.hxx"
#include "DistFunc.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(NonCentralStudent);

static Factory<NonCentralStudent> RegisteredFactory("NonCentralStudent");


/* Default constructor */
NonCentralStudent::NonCentralStudent(const NumericalScalar nu,
                                     const NumericalScalar delta,
                                     const NumericalScalar gamma)
  : NonEllipticalDistribution("NonCentralStudent"),
    nu_(0.0),
    delta_(delta),
    gamma_(gamma)
{
  // This call set also the range.
  setNu(nu);
  setDimension(1);
}

/* Comparison operator */
Bool NonCentralStudent::operator ==(const NonCentralStudent & other) const
{
  if (this == &other) return true;
  return (nu_ == other.nu_) && (delta_ == other.delta_) && (gamma_ == other.gamma_);
}

/* String converter */
String NonCentralStudent::__repr__() const
{
  OSS oss;
  oss << "class=" << NonCentralStudent::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " nu=" << nu_
      << " delta=" << delta_
      << " gamma=" << gamma_;
  return oss;
}

String NonCentralStudent::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(nu = " << nu_ << ", delta = " << delta_ << ", gamma = " << gamma_ << ")";
  return oss;
}

/* Virtual constructor */
NonCentralStudent * NonCentralStudent::clone() const
{
  return new NonCentralStudent(*this);
}

/* Get one realization of the distribution */
NumericalPoint NonCentralStudent::getRealization() const
{
  return NumericalPoint(1, gamma_ + DistFunc::rNonCentralStudent(nu_, delta_));
}

/* Get the PDF of the distribution */
NumericalScalar NonCentralStudent::computePDF(const NumericalPoint & point) const
{
  pdfEpsilon_ = ResourceMap::GetAsNumericalScalar("DistFunc-Precision");
  return DistFunc::dNonCentralStudent(nu_, delta_, point[0] - gamma_);
}

/* Get the CDF of the distribution */
NumericalScalar NonCentralStudent::computeCDF(const NumericalPoint & point) const
{
  cdfEpsilon_ = ResourceMap::GetAsNumericalScalar("DistFunc-Precision");
  return DistFunc::pNonCentralStudent(nu_, delta_, point[0] - gamma_);
}

/** Get the PDFGradient of the distribution */
NumericalPoint NonCentralStudent::computePDFGradient(const NumericalPoint & point) const
{
  const NumericalScalar eps(pow(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"), 1.0 / 3.0));
  NumericalPoint pdfGradient(3);
  pdfGradient[0] = (DistFunc::dNonCentralStudent(nu_ + eps, delta_, point[0] - gamma_) - DistFunc::dNonCentralStudent(nu_ - eps, delta_, point[0] - gamma_)) / (2.0 * eps);
  pdfGradient[1] = (DistFunc::dNonCentralStudent(nu_, delta_ + eps, point[0] - gamma_) - DistFunc::dNonCentralStudent(nu_, delta_ - eps, point[0] - gamma_)) / (2.0 * eps);
  pdfGradient[2] = (DistFunc::dNonCentralStudent(nu_, delta_, point[0] - gamma_ - eps) - DistFunc::dNonCentralStudent(nu_, delta_, point[0] - gamma_ + eps)) / (2.0 * eps);
  return pdfGradient;
}

/** Get the CDFGradient of the distribution */
NumericalPoint NonCentralStudent::computeCDFGradient(const NumericalPoint & point) const
{
  const NumericalScalar eps(pow(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"), 1.0 / 3.0));
  NumericalPoint cdfGradient(3);
  cdfGradient[0] = (DistFunc::pNonCentralStudent(nu_ + eps, delta_, point[0] - gamma_) - DistFunc::pNonCentralStudent(nu_ - eps, delta_, point[0] - gamma_)) / (2.0 * eps);
  cdfGradient[1] = (DistFunc::pNonCentralStudent(nu_, delta_ + eps, point[0] - gamma_) - DistFunc::pNonCentralStudent(nu_, delta_ - eps, point[0] - gamma_)) / (2.0 * eps);
  cdfGradient[2] = (DistFunc::pNonCentralStudent(nu_, delta_, point[0] - gamma_ - eps) - DistFunc::pNonCentralStudent(nu_, delta_, point[0] - gamma_ + eps)) / (2.0 * eps);
  return cdfGradient;
}

/* Compute the mean of the distribution */
void NonCentralStudent::computeMean() const
{
  if (nu_ <= 1.0) throw NotDefinedException(HERE) << "Error: the mean is defined only for nu > 1 for a non central Student distribution";
  mean_ = NumericalPoint(1, sqrt(0.5 * nu_) * exp(SpecFunc::LnGamma(0.5 * (nu_ - 1.0)) - SpecFunc::LnGamma(0.5 * nu_)) * delta_ + gamma_);
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint NonCentralStudent::getStandardDeviation() const
{
  if (nu_ <= 2.0) throw NotDefinedException(HERE) << "Error: the standard deviation is defined only for nu > 2 for a non central Student distribution";
  return NumericalPoint(1, sqrt(getCovariance().operator()(0, 0)));
}

/* Get the skewness of the distribution */
NumericalPoint NonCentralStudent::getSkewness() const
{
  NumericalScalar mup1(getMean()[0] - gamma_);
  NumericalScalar mu2(getCovariance().operator()(0, 0));
  return NumericalPoint(1, mup1 * (nu_ * (2.0 * nu_ - 3.0 + delta_ * delta_) / ((nu_ - 2.0) * (nu_ - 3.0)) - 2.0 * mu2) * pow(mu2, -1.5));
}

/* Get the kurtosis of the distribution */
NumericalPoint NonCentralStudent::getKurtosis() const
{
  NumericalScalar mup1(getMean()[0] - gamma_);
  NumericalScalar mu2(getCovariance().operator()(0, 0));
  NumericalScalar delta2(delta_ * delta_);
  return NumericalPoint(1, (nu_ * nu_ * (3.0 + 6.0 * delta2 + delta2 * delta2) / ((nu_ - 2.0) * (nu_ - 4.0)) - mup1 * mup1 * (nu_ * ((nu_ + 1.0) * delta2 + 3.0 * (3.0 * nu_ - 5.0)) / ((nu_ - 2.0) * (nu_ - 3.0)) - 3.0 * mu2)) * pow(mu2, -2.0));
}

/* Get the moments of the standardized distribution */
NumericalPoint NonCentralStudent::getStandardMoment(const UnsignedLong n) const
{
  if (n >= nu_) throw NotDefinedException(HERE) << "Error: cannot compute a standard moment of order greater or equal to the number of degrees of freedom";
  UniVariatePolynomial p(NumericalPoint(1, 1.0));
  for (UnsignedLong k = 0; k < n; ++k) p = p.derivate() + p.incrementDegree(1);
  return NumericalPoint(1, p(delta_) * exp(0.5 * n * log(0.5 * nu_) + SpecFunc::LogGamma(0.5 * (nu_ - n)) - SpecFunc::LogGamma(0.5 * nu_)));
}

/* Get the standard representative in the parametric family, associated with the standard moments */
NonCentralStudent::Implementation NonCentralStudent::getStandardRepresentative() const
{
  return NonCentralStudent(nu_, delta_, 0.0).clone();
}

/* Compute the covariance of the distribution */
void NonCentralStudent::computeCovariance() const
{
  if (nu_ <= 2.0) throw NotDefinedException(HERE) << "Error: the covariance is defined only for nu > 2 for a non central Student distribution";
  covariance_ = CovarianceMatrix(1);
  const NumericalScalar mup1(getMean()[0] - gamma_);
  covariance_(0, 0) =  nu_ / (nu_ - 2.0) * (1.0 + delta_ * delta_) - mup1 * mup1;
  isAlreadyComputedCovariance_ = true;
}

/* Parameters value and description accessor */
NonCentralStudent::NumericalPointWithDescriptionCollection NonCentralStudent::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(3);
  Description description(point.getDimension());
  point[0] = nu_;
  point[1] = delta_;
  point[2] = gamma_;
  description[0] = "nu";
  description[1] = "delta";
  description[2] = "gamma";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void NonCentralStudent::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = NonCentralStudent(parametersCollection[0][0], parametersCollection[0][1], parametersCollection[0][2]);
}

/* Nu accessor */
void NonCentralStudent::setNu(const NumericalScalar nu)
{
  if (nu <= 0.0) throw InvalidArgumentException(HERE) << "Nu MUST be strictly positive";
  if (nu != nu_)
    {
      nu_ = nu;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

/* Nu accessor */
NumericalScalar NonCentralStudent::getNu() const
{
  return nu_;
}


/* Delta accessor */
void NonCentralStudent::setDelta(const NumericalScalar delta)
{
  if (delta != delta_)
    {
      delta_ = delta;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

/* Delta accessor */
NumericalScalar NonCentralStudent::getDelta() const
{
  return delta_;
}

/* Gamma accessor */
void NonCentralStudent::setGamma(const NumericalScalar gamma)
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
NumericalScalar NonCentralStudent::getGamma() const
{
  return gamma_;
}

/* Method save() stores the object through the StorageManager */
void NonCentralStudent::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "nu_", nu_ );
  adv.saveAttribute( "delta_", delta_ );
  adv.saveAttribute( "gamma_", gamma_ );
}

/* Method load() reloads the object from the StorageManager */
void NonCentralStudent::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "nu_", nu_ );
  adv.loadAttribute( "delta_", delta_ );
  adv.loadAttribute( "gamma_", gamma_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
