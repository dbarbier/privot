//                                               -*- C++ -*-
/**
 *  @file  Geometric.cxx
 *  @brief The Geometric distribution
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
#include "Geometric.hxx"
#include "SpecFunc.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Geometric);

static Factory<Geometric> RegisteredFactory("Geometric");

/* Default constructor */
Geometric::Geometric()
  : DiscreteDistribution("Geometric")
  , p_(0.5)
{
  setDimension( 1 );
  computeRange();
}

/* Parameters constructor */
Geometric::Geometric(const NumericalScalar p)
  : DiscreteDistribution("Geometric")
  , p_(p)
{
  if ( (p <= 0.0) || (p > 1.0) ) throw InvalidArgumentException(HERE) << "Error: p must be in ]0, 1], here p=" << p;
  // We set the dimension of the Geometric distribution
  setDimension( 1 );
  computeRange();
}

/* Comparison operator */
Bool Geometric::operator ==(const Geometric & other) const
{
  if (this == &other) return true;
  return p_ == other.p_;
}

/* String converter */
String Geometric::__repr__() const
{
  OSS oss;
  oss << "class=" << Geometric::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " p=" << p_;
  return oss;
}

String Geometric::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(p = " << p_ << ")";
  return oss;
}

/* Virtual constructor */
Geometric * Geometric::clone() const
{
  return new Geometric(*this);
}

/* Compute the numerical range of the distribution given the parameters values */
void Geometric::computeRange()
{
  const NumericalPoint lowerBound(1, 0.0);
  const NumericalPoint upperBound(DistributionImplementation::computeUpperBound());
  const Interval::BoolCollection finiteLowerBound(1, true);
  const Interval::BoolCollection finiteUpperBound(1, false);
  setRange(Interval(lowerBound, upperBound, finiteLowerBound, finiteUpperBound));
}

/* Get one realization of the distribution */
NumericalPoint Geometric::getRealization() const
{
  return NumericalPoint(1, ceil(log(RandomGenerator::Generate()) / log1p(-p_)));
}


/* Get the PDF of the distribution */
NumericalScalar Geometric::computePDF(const NumericalPoint & point) const
{
  NumericalScalar k(point[0]);
  const NumericalScalar supportEpsilon(ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"));
  if ((k < 1.0 - supportEpsilon) || (fabs(k - round(k)) > supportEpsilon)) return 0.0;
  return p_ * pow(1.0 - p_, k - 1.0);
}


/* Get the CDF of the distribution */
NumericalScalar Geometric::computeCDF(const NumericalPoint & point) const
{
  NumericalScalar k(point[0]);
  if (k < 1.0) return 0.0;
  return 1.0 - pow(1.0 - p_, floor(k));
}

NumericalScalar Geometric::computeComplementaryCDF(const NumericalPoint & point) const
{
  NumericalScalar k(point[0]);
  if (k < 1.0) return 1.0;
  return pow(1.0 - p_, floor(k));
}

/* Get the PDFGradient of the distribution */
NumericalPoint Geometric::computePDFGradient(const NumericalPoint & point) const
{
  NumericalScalar k(point[0]);
  const NumericalScalar supportEpsilon(ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"));
  if ((k < 1.0 - supportEpsilon) || (fabs(k - round(k)) > supportEpsilon)) return NumericalPoint(1, 0.0);
  return NumericalPoint(1, (1.0 - k * p_) * pow(1.0 - p_, k - 2.0));
}

/* Get the CDFGradient of the distribution */
NumericalPoint Geometric::computeCDFGradient(const NumericalPoint & point) const
{
  NumericalScalar k(floor(point[0]));
  if ( k < 1.0 ) return NumericalPoint(1, 0.0);
  return NumericalPoint(1, k * pow(1 - p_, k - 1.0));
}

/* Get the quantile of the distribution */
NumericalScalar Geometric::computeScalarQuantile(const NumericalScalar prob,
                                                 const Bool tail,
                                                 const NumericalScalar precision) const
{
  if (tail) return ceil(log(prob) / log1p(-p_));
  return ceil(log1p(-prob) / log1p(-p_));
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex Geometric::computeCharacteristicFunction(const NumericalScalar x) const
{
  const NumericalComplex value(p_ / (exp(NumericalComplex(0.0, -x)) - (1.0 - p_)));
  return value;
}

/* Get the generating function of the distribution, i.e. psi(z) = E(z^X) */
NumericalComplex Geometric::computeGeneratingFunction(const NumericalComplex & z) const
{
  return p_ * z / (1.0 - (1.0 - p_) * z);
}

/* Get the support of a discrete distribution that intersect a given interval */
NumericalSample Geometric::getSupport(const Interval & interval) const
{
  if (interval.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given interval has a dimension that does not match the distribution dimension.";
  const UnsignedLong kMin(static_cast< UnsignedLong > (std::max(ceil(interval.getLowerBound()[0]), 0.0)));
  const UnsignedLong kMax(static_cast< UnsignedLong > (floor(interval.getUpperBound()[0])));
  NumericalSample result(0, 1);
  for (UnsignedLong k = kMin; k <= kMax; ++k)
    result.add(NumericalPoint(1, k));
  return result;
}

/* Compute the mean of the distribution */
void Geometric::computeMean() const
{
  mean_ = NumericalPoint(1, 1 / p_);
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Geometric::getStandardDeviation() const
{
  return NumericalPoint(1, sqrt(1.0 - p_) / p_);
}

/* Get the skewness of the distribution */
NumericalPoint Geometric::getSkewness() const
{
  return NumericalPoint(1, (2.0 - p_) / sqrt(1.0 - p_));
}

/* Get the kurtosis of the distribution */
NumericalPoint Geometric::getKurtosis() const
{
  return NumericalPoint(1, 9.0 + p_ * p_ / (1.0 - p_));
}

/* Compute the covariance of the distribution */
void Geometric::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  covariance_(0, 0) = (1.0 - p_) / (p_ * p_);
  isAlreadyComputedCovariance_ = true;
}

/* Parameters value and description accessor */
Geometric::NumericalPointWithDescriptionCollection Geometric::getParametersCollection() const
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

void Geometric::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = Geometric(parametersCollection[0][0]);
}



/* P accessor */
void Geometric::setP(const NumericalScalar p)
{
  if ( (p <= 0.0) || (p > 1.0) ) throw InvalidArgumentException(HERE) << "Error: p must be in ]0, 1], here p=" << p;
  if (p != p_)
    {
      p_ = p;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

/* P accessor */
NumericalScalar Geometric::getP() const
{
  return p_;
}

/* Method save() stores the object through the StorageManager */
void Geometric::save(Advocate & adv) const
{
  DiscreteDistribution::save(adv);
  adv.saveAttribute( "p_", p_ );
}

/* Method load() reloads the object from the StorageManager */
void Geometric::load(Advocate & adv)
{
  DiscreteDistribution::load(adv);
  adv.loadAttribute( "p_", p_ );
  computeRange();
}



END_NAMESPACE_OPENTURNS
