//                                               -*- C++ -*-
/**
 *  @file  Bernoulli.cxx
 *  @brief The Bernoulli distribution
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
 *  @date   2010-07-12 15:45:44 +0200 (lun. 12 juil. 2010)
 */
#include <cmath>
#include "Bernoulli.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Bernoulli);

static Factory<Bernoulli> RegisteredFactory("Bernoulli");

/* Default constructor */
Bernoulli::Bernoulli()
  : DiscreteDistribution("Bernoulli"),
    p_(0.5)
{
  // We set the dimension of the Bernoulli distribution
  setDimension( 1 );
  computeRange();
}

/* Parameters constructor */
Bernoulli::Bernoulli(const NumericalScalar p)
  : DiscreteDistribution("Bernoulli"),
    p_(p)
{
  // We set the dimension of the Bernoulli distribution
  setDimension( 1 );
  computeRange();
}

/* Comparison operator */
Bool Bernoulli::operator ==(const Bernoulli & other) const
{
  if (this == &other) return true;
  return p_ == other.p_;
}

/* String converter */
String Bernoulli::__repr__() const
{
  OSS oss;
  oss << "class=" << Bernoulli::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " p=" << p_;
  return oss;
}

String Bernoulli::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(p = " << p_ << ")";
  return oss;
}

/* Virtual constructor */
Bernoulli * Bernoulli::clone() const
{
  return new Bernoulli(*this);
}

/* Get one realization of the distribution */
NumericalPoint Bernoulli::getRealization() const
{
  return NumericalPoint(1, (RandomGenerator::Generate() < p_ ? 1.0 : 0.0));
}


/* Get the PDF of the distribution */
NumericalScalar Bernoulli::computePDF(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);
  if (fabs(k) < ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return 1.0 - p_;
  if (fabs(k - 1.0) < ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return p_;
  return 0.0;
}


/* Get the CDF of the distribution */
NumericalScalar Bernoulli::computeCDF(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);
  // k < 0.0
  if (k < -ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return 0.0;
  // k >= 1.0
  if (k > 1.0 - ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return 1.0;
  // k > 0.0 && k < 1.0
  return 1.0 - p_;
}

/* Get the PDF gradient of the distribution */
NumericalPoint Bernoulli::computePDFGradient(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);
  NumericalPoint pdfGradient(1, 0.0);
  if ((k < -ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) || (fabs(k - round(k)) > ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"))) return pdfGradient;
  throw NotYetImplementedException(HERE);
}


/* Get the CDF gradient of the distribution */
NumericalPoint Bernoulli::computeCDFGradient(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);
  if (k < -ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return NumericalPoint(1, 0.0);
  throw NotYetImplementedException(HERE);
}

/* Get the quantile of the distribution */
NumericalScalar Bernoulli::computeScalarQuantile(const NumericalScalar prob,
                                                 const Bool tail,
                                                 const NumericalScalar precision) const
{
  if (prob < 1.0 - p_) return (tail ? 1.0 : 0.0);
  return (tail ? 0.0 : 1.0);
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex Bernoulli::computeCharacteristicFunction(const NumericalScalar x) const
{
  const NumericalComplex value(1.0 - p_ + p_ * exp(NumericalComplex(0.0, x)));
  return value;
}

/* Get the generating function of the distribution, i.e. psi(z) = E(z^X) */
NumericalComplex Bernoulli::computeGeneratingFunction(const NumericalComplex & z) const
{
  const NumericalComplex value(1.0 - p_ + p_ * z);
  return value;
}

/* Compute the mean of the distribution */
void Bernoulli::computeMean() const
{
  mean_ = NumericalPoint(1, p_);
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Bernoulli::getStandardDeviation() const
{
  return NumericalPoint(1, sqrt(p_ * (1.0 - p_)));
}

/* Get the skewness of the distribution */
NumericalPoint Bernoulli::getSkewness() const
{
  if ((p_ == 0.0) || (p_ == 1.0)) throw NotDefinedException(HERE) << "Error: the skewness is not defined for the Bernoulli distribution when p is zero or one.";
  return NumericalPoint(1, (1.0 - 2.0 * p_) / sqrt(p_ * (1.0 - p_)));
}

/* Get the kurtosis of the distribution */
NumericalPoint Bernoulli::getKurtosis() const
{
  if ((p_ == 0.0) || (p_ == 1.0)) throw NotDefinedException(HERE) << "Error: the kurtosis is not defined for the Bernoulli distribution when p is zero or one.";
  return NumericalPoint(1, 3.0 + (6.0 * p_ * (1.0 - p_) + 1.0) / (p_ * (1.0 - p_)));
}

/* Get the moments of the standardized distribution */
NumericalPoint Bernoulli::getStandardMoment(const UnsignedLong n) const
{
  if (n == 0) return NumericalPoint(1, 0.0);
  return NumericalPoint(1, p_);
}

/* Compute the covariance of the distribution */
void Bernoulli::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  covariance_(0, 0) = p_ * (1.0 - p_);
  isAlreadyComputedCovariance_ = true;
}

/* Get the support of a discrete distribution that intersect a given interval */
NumericalSample Bernoulli::getSupport(const Interval & interval) const
{
  if (interval.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given interval has a dimension that does not match the distribution dimension.";
  const UnsignedLong kMin(static_cast< UnsignedLong > (std::max(ceil(interval.getLowerBound()[0]), 0.0)));
  const UnsignedLong kMax(static_cast< UnsignedLong > (std::min(floor(interval.getUpperBound()[0]), 1.0)));
  NumericalSample result(0, 1);
  for (UnsignedLong k = kMin; k <= kMax; ++k) result.add(NumericalPoint(1, k));
  return result;
}

/* Parameters value and description accessor */
Bernoulli::NumericalPointWithDescriptionCollection Bernoulli::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(1);
  Description description(point.getDimension());
  point[0] = p_;
  description[0] = "p";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void Bernoulli::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = Bernoulli(parametersCollection[0][0]);
}

/* P accessor */
void Bernoulli::setP(const NumericalScalar p)
{
  if ((p < 0.0) || (p > 1.0)) throw InvalidArgumentException(HERE) << "P must be in [0, 1], here p=" << p;
  if (p != p_)
    {
      p_ = p;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

/* P accessor */
NumericalScalar Bernoulli::getP() const
{
  return p_;
}


/* Compute the numerical range of the distribution given the parameters values */
void Bernoulli::computeRange()
{
  // Degenerate cases
  if (p_ == 0.0) setRange(Interval(0.0, 0.0));
  if (p_ == 1.0) setRange(Interval(1.0, 1.0));
  // Usual case
  setRange(Interval(0.0, 1.0));
}

/* Method save() stores the object through the StorageManager */
void Bernoulli::save(Advocate & adv) const
{
  DiscreteDistribution::save(adv);
  adv.saveAttribute( "p_", p_ );
}

/* Method load() reloads the object from the StorageManager */
void Bernoulli::load(Advocate & adv)
{
  DiscreteDistribution::load(adv);
  adv.loadAttribute( "p_", p_ );
}

END_NAMESPACE_OPENTURNS
