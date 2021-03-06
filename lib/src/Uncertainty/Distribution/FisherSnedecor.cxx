//                                               -*- C++ -*-
/**
 *  @file  FisherSnedecor.cxx
 *  @brief The Fisher-Snedecor distribution
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
 *  @author dutka
 *  @date   2010-02-04 16:44:49 +0100 (jeu. 04 févr. 2010)
 */
#include <cmath>
#include "FisherSnedecor.hxx"
#include "RandomGenerator.hxx"
#include "IdentityMatrix.hxx"
#include "PersistentObjectFactory.hxx"
#include "DistFunc.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(FisherSnedecor);

static Factory<FisherSnedecor> RegisteredFactory("FisherSnedecor");

/* Default constructor */
FisherSnedecor::FisherSnedecor()
  : NonEllipticalDistribution("FisherSnedecor")
  , d1_(1.0)
  , d2_(1.0)
  , normalizationFactor_(0.0)
{
  setDimension( 1 );
  update();
  computeRange();
}

/* Parameters constructor */
FisherSnedecor::FisherSnedecor(const NumericalScalar d1,
                               const NumericalScalar d2)
  : NonEllipticalDistribution("FisherSnedecor")
  , d1_(d1)
  , d2_(d2)
  , normalizationFactor_(0.0)
{
  if (d1 <= 0.0) throw InvalidArgumentException(HERE) << "Error: cannot build a FisherSnedecor distribution with d1=" << d1 << " <= 0.";
  if (d2 <= 0.0) throw InvalidArgumentException(HERE) << "Error: cannot build a FisherSnedecor distribution with d2=" << d2 << " <= 0.";
  setDimension( 1 );
  update();
  computeRange();
}

/* Comparison operator */
Bool FisherSnedecor::operator ==(const FisherSnedecor & other) const
{
  if (this == &other) return true;
  return (d1_ == other.d1_) && (d2_ == other.d2_);
}

/* String converter */
String FisherSnedecor::__repr__() const
{
  OSS oss;
  oss << "class=" << FisherSnedecor::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " d1=" << d1_
      << " d2=" << d2_;
  return oss;
}

String FisherSnedecor::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(d1 = " << d1_ << ", d2 = " << d2_ << ")";
  return oss;
}

/* Virtual constructor */
FisherSnedecor * FisherSnedecor::clone() const
{
  return new FisherSnedecor(*this);
}

/* Compute the numerical range of the distribution given the parameters values */
void FisherSnedecor::computeRange()
{
  setRange(Interval(NumericalPoint(1, 0.0), NumericalPoint(1, computeScalarQuantile(cdfEpsilon_, true)), Interval::BoolCollection(1, true), Interval::BoolCollection(1, false)));
}

/* Update the derivative attributes */
void FisherSnedecor::update()
{
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  normalizationFactor_ = 0.5 * d1_ * log(d1_ / d2_) - SpecFunc::LnBeta(0.5 * d1_, 0.5 * d2_);
}

/* Get one realization of the distribution */
NumericalPoint FisherSnedecor::getRealization() const
{
  return NumericalPoint(1, d2_ * DistFunc::rGamma(0.5 * d1_) / (d1_ * DistFunc::rGamma(0.5 * d2_)));
}

/* Get the PDF of the distribution */
NumericalScalar FisherSnedecor::computePDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot compute the PDF of a FisherSnedecor distribution at a point of dimension not equal to 1.";
  const NumericalScalar x(point[0]);
  if (x <= 0.0) return 0.0;
  return exp(normalizationFactor_ + (0.5 * d1_ - 1.0) * log(x) - 0.5 * (d1_ + d2_) * log1p(d1_ * x / d2_));
}

/* Get the CDF of the distribution */
NumericalScalar FisherSnedecor::computeCDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot compute the CDF of a FisherSnedecor distribution at a point of dimension not equal to 1.";
  NumericalScalar x(point[0]);
  if (x <= 0) return 0.0;
  return DistFunc::pBeta(0.5 * d1_, 0.5 * d2_, d1_ * x / (d1_ * x + d2_));
}

/* Get the quantile of the distribution */
NumericalScalar FisherSnedecor::computeScalarQuantile(const NumericalScalar prob,
                                                      const Bool tail,
                                                      const NumericalScalar precision) const
{
  const NumericalScalar p(tail ? 1.0 - prob : prob);
  const NumericalScalar q(DistFunc::qBeta(0.5 * d1_, 0.5 * d2_, p));
  const NumericalScalar value(d2_ * q / (d1_ * (1.0 - q)));
  return value;
}

/* Compute the mean of the distribution */
void FisherSnedecor::computeMean() const
{
  if (d2_ <= 2.0) throw NotDefinedException(HERE) << "Error: the mean is defined only when d2 > 2.";
  mean_ =  NumericalPoint(1, d2_ / (d2_ - 2));
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint FisherSnedecor::getStandardDeviation() const
{
  return NumericalPoint(1, sqrt(getCovariance()(0, 0)));
}

/* Get the skewness of the distribution */
NumericalPoint FisherSnedecor::getSkewness() const
{
  if (d2_ <= 6.0) throw NotDefinedException(HERE) << "Error: the skewness is defined only when d2 > 6.";
  return NumericalPoint(1, (2.0 * d1_ + d2_ - 2.0) * sqrt(8.0 * (d2_ - 4.0)) / ((d2_ - 6.0) * sqrt(d1_ * (d1_ + d2_ - 2.0))));
}

/* Get the kurtosis of the distribution */
NumericalPoint FisherSnedecor::getKurtosis() const
{
  if (d2_ <= 8.0) throw NotDefinedException(HERE) << "Error: the kurtosis is defined only when d2 > 6.";
  return NumericalPoint(1, 3.0 * (d2_ - 4.0) * (16.0 + d2_ * (-16.0 + 4.0 * d2_) + d1_ * (-20.0 + d2_ * (8.0 + d2_) + d1_ * (10.0 + d2_))) / (d1_ * (d1_ + d2_ - 2.0) * (d2_ - 6.0) * (d2_ - 8.0)));
}

/* Get the moments of the standardized distribution */
NumericalPoint FisherSnedecor::getStandardMoment(const UnsignedLong n) const
{
  if (2 * n >= d2_) throw NotDefinedException(HERE) << "Error: The raw moment of a FisherSnedecor distribution is defined only up to order d2/2, here n=" << n << " and d2=" << d2_;
  return NumericalPoint(1, exp(n * log(d2_ / d1_) + SpecFunc::LogGamma(0.5 * d1_ + n) + SpecFunc::LogGamma(0.5 * d2_ - n) - SpecFunc::LogGamma(0.5 * d1_) - SpecFunc::LogGamma(0.5 * d2_)));
}

/* Compute the covariance of the distribution */
void FisherSnedecor::computeCovariance() const
{
  if (d2_ <= 4.0) throw NotDefinedException(HERE) << "Error: the covariance is defined only when d2 > 4.";
  covariance_ = CovarianceMatrix(1);
  covariance_(0, 0) = 2.0 * d2_ * d2_ * (d1_ + d2_ - 2.0) / (d1_ * (d2_ - 4.0) * pow(d2_ - 2, 2));
  isAlreadyComputedMean_ = true;
}

/* Parameters value and description accessor */
FisherSnedecor::NumericalPointWithDescriptionCollection FisherSnedecor::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(2);
  Description description(point.getDimension());
  point[0] = d1_;
  point[1] = d2_;
  description[0] = "d1";
  description[1] = "d2";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void FisherSnedecor::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = FisherSnedecor(parametersCollection[0][0], parametersCollection[0][1]);
}
/* D1 accessor */
void FisherSnedecor::setD1(const NumericalScalar d1)
{
  if (d1_ <= 0.0) throw InvalidArgumentException(HERE) << "Error d1 of a FisherSnedecor distribution must be positive";
  if (d1_ != d1)
    {
      d1_ = d1;
      update();
      computeRange();
    }
}

NumericalScalar FisherSnedecor::getD1() const
{
  return d1_;
}


/* D2 accessor */
void FisherSnedecor::setD2(const NumericalScalar d2)
{
  if (d2_ <= 0.0) throw InvalidArgumentException(HERE) << "Error d2 of a FisherSnedecor distribution must be positive";
  if (d2_ != d2)
    {
      d2_ = d2;
      update();
      computeRange();
    }
}

NumericalScalar FisherSnedecor::getD2() const
{
  return d2_;
}


/* Method save() stores the object through the StorageManager */
void FisherSnedecor::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "d1_", d1_ );
  adv.saveAttribute( "d2_", d2_ );
  adv.saveAttribute( "normalizationFactor_", normalizationFactor_ );
}

/* Method load() reloads the object from the StorageManager */
void FisherSnedecor::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "d1_", d1_ );
  adv.loadAttribute( "d2_", d2_ );
  adv.loadAttribute( "normalizationFactor_", normalizationFactor_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
