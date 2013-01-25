//                                               -*- C++ -*-
/**
 *  @file  KSubset.cxx
 *  @brief The KSubset distribution
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include <cmath>
#include "Collection.hxx"
#include "KSubset.hxx"
#include "Binomial.hxx"
#include "Poisson.hxx"
#include "TruncatedDistribution.hxx"
#include "RandomGenerator.hxx"
#include "SpecFunc.hxx"
#include "DistFunc.hxx"
#include "Exception.hxx"
#include "ResourceMap.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

typedef Collection<UnsignedLong>     UnsignedLongCollection;

CLASSNAMEINIT(KSubset);

static Factory<KSubset> RegisteredFactory("KSubset");

/* Default constructor */
KSubset::KSubset()
  : DiscreteDistribution("KSubset")
  , k_(0)
  , n_(0)
{
  setK(1);
  setN(1);
}

/* Parameters constructor */
KSubset::KSubset(const UnsignedLong k,
		 const UnsignedLong n)
  : DiscreteDistribution("KSubset")
  , k_(0)
  , n_(0)
{
  // This method compute the range also
  setK(k);
  setN(n);
}

/* Comparison operator */
Bool KSubset::operator ==(const KSubset & other) const
{
  if (this == &other) return true;
  return (k_ == other.k_) && (n_ == other.n_);
}

/* String converter */
String KSubset::__repr__() const
{
  OSS oss;
  oss << "class=" << KSubset::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " k=" << k_
      << " n=" << n_;
  return oss;
}

String KSubset::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(k = " << k_ << ", n = " << n_ << ")";
  return oss;
}

/* Virtual constructor */
KSubset * KSubset::clone() const
{
  return new KSubset(*this);
}

/* Compute the numerical range of the distribution given the parameters values */
void KSubset::computeRange()
{
  const NumericalPoint lowerBound(k_, 0.0);
  const NumericalPoint upperBound(k_, n_ - 1.0);
  const Interval::BoolCollection finiteLowerBound(k_, true);
  const Interval::BoolCollection finiteUpperBound(k_, true);
  setRange(Interval(lowerBound, upperBound, finiteLowerBound, finiteUpperBound));
}

/* Get one realization of the distribution */
NumericalPoint KSubset::getRealization() const
{
  NumericalPoint realization(k_);
  Indices buffer(n_);
  buffer.fill();
  for (UnsignedLong i = 0; i < k_; ++i)
    {
      UnsignedLong index(i + RandomGenerator::IntegerGenerate(n_ - i));
      realization[i] = buffer[index];
      buffer[index] = buffer[i];
    }
  return realization;
}

/* Get the PDF of the distribution */
NumericalScalar KSubset::computeLogPDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << dimension << ", here dimension=" << point.getDimension();
  Indices x(k_);
  const NumericalScalar supportEpsilon(ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"));
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar k(point[i]);
      if ((k < -supportEpsilon) || (k > n_ + supportEpsilon)) return -SpecFunc::MaxNumericalScalar;
      const UnsignedLong ik(static_cast< UnsignedLong > (round(k)));
      if (fabs(k - ik) > supportEpsilon) return -SpecFunc::MaxNumericalScalar;
      x[i] = ik;
    }
  if (!x.check(n_ - 1)) return 0.0;
  return SpecFunc::LnGamma(k_ + 1) - SpecFunc::LnGamma(n_ + 1);
}

NumericalScalar KSubset::computePDF(const NumericalPoint & point) const
{
  const NumericalScalar logPDF(computeLogPDF(point));
  if (logPDF == -SpecFunc::MaxNumericalScalar) return 0.0;
  return exp(logPDF);
}

/* Get the CDF of the distribution */
NumericalScalar KSubset::computeCDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << dimension << ", here dimension=" << point.getDimension();

  if (dimension == 1) return static_cast < NumericalScalar >(k_) / n_;
  NumericalPoint sortedPoint(dimension);
  const NumericalScalar supportEpsilon(ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"));
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar x(point[i]);
      if (x < -supportEpsilon) return 0.0;
      sortedPoint[i] = std::min(n_ - 1.0, floor(x + supportEpsilon));
    }
  std::sort(sortedPoint.begin(), sortedPoint.end());
  NumericalScalar cdfValue(1.0);
  for (UnsignedLong i = 0; i < dimension; ++i) cdfValue *= (sortedPoint[i] + 1.0 - i) / (n_ - i);
  return cdfValue;
}

/* Compute the scalar quantile of the 1D KSubset distribution */
NumericalScalar KSubset::computeScalarQuantile(const NumericalScalar prob,
					       const Bool tail,
					       const NumericalScalar precision) const
{
  const UnsignedLong i(static_cast< UnsignedLong >(ceil(prob * (n_ - 1.0))));
  return (tail ? n_ - 1.0 - i : i);
} // computeScalarQuantile

/* Get the i-th marginal distribution */
KSubset::Implementation KSubset::getMarginal(const UnsignedLong i) const
{
  const UnsignedLong dimension(getDimension());
  if (i >= dimension) throw InvalidArgumentException(HERE) << "The index of a marginal distribution must be in the range [0, dim-1]";
  return new KSubset(1, n_);
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
KSubset::Implementation KSubset::getMarginal(const Indices & indices) const
{
  const UnsignedLong dimension(getDimension());
  if (!indices.check(dimension - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal distribution must be in the range [0, dim-1] and  must be different";
  // Special case for dimension 1
  if (dimension == 1) return clone();
  // General case
  const UnsignedLong outputDimension(indices.getSize());
  return new KSubset(outputDimension, n_);
} // getMarginal(Indices)

/* Get the support of a discrete distribution that intersect a given interval */
NumericalSample KSubset::getSupport(const Interval & interval) const
{
  if (interval.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given interval has a dimension that does not match the distribution dimension.";
  throw NotYetImplementedException(HERE);
}

/* Compute the mean of the distribution */
void KSubset::computeMean() const
{
  mean_ = NumericalPoint(k_, 0.5 * (n_ - 1.0));
  isAlreadyComputedMean_ = true;
}

/* Compute the covariance of the distribution */
void KSubset::computeCovariance() const
{
  const NumericalScalar var((n_ * n_ - 1.0) / 12.0);
  const NumericalScalar cov(-(n_ + 1.0) / 12.0);
  covariance_ = CovarianceMatrix(k_, NumericalPoint(k_ * k_, cov));
  for (UnsignedLong i = 0; i < k_; ++i) covariance_(i, i) = var;
  isAlreadyComputedCovariance_ = true;
}

/* Parameters value and description accessor */
KSubset::NumericalPointWithDescriptionCollection KSubset::getParametersCollection() const
{
  const UnsignedLong dimension(getDimension());
  NumericalPointWithDescriptionCollection parameters((dimension == 1 ? 1 : dimension + 1));
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      NumericalPointWithDescription point(1);
      point[0] = n_;
      Description description(1);
      description[0] = "n";
      point.setDescription(description);
      point.setName(getDescription()[i]);
      parameters[i] = point;
    }
  if (dimension > 1)
    {
      NumericalPointWithDescription point(2);
      Description description(2);
      point[0] = k_;
      description[0] = "k";
      point[1] = n_;
      description[1] = "n";
      point.setDescription(description);
      point.setName("dependence");
      parameters[dimension] = point;
    }
  return parameters;
}

/* K accessor */
void KSubset::setK(const UnsignedLong k)
{
  if (k == 0) throw InvalidArgumentException(HERE) << "Error: k must be > 0.";
  if (k != k_)
    {
      k_ = k;
      setDimension(k);
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      isAlreadyCreatedGeneratingFunction_ = false;
    }
}

/* K accessor */
UnsignedLong KSubset::getK() const
{
  return k_;
}

/* N accessor */
void KSubset::setN(const UnsignedLong n)
{
  if (n == 0) throw InvalidArgumentException(HERE) << "Error: n must be > 0.";
  if (n != n_)
    {
      n_ = n;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

UnsignedLong KSubset::getN() const
{
  return n_;
}

/* Method save() stores the object through the StorageManager */
void KSubset::save(Advocate & adv) const
{
  DiscreteDistribution::save(adv);
  adv.saveAttribute( "k_", k_ );
  adv.saveAttribute( "n_", n_ );
}

/* Method load() reloads the object from the StorageManager */
void KSubset::load(Advocate & adv)
{
  DiscreteDistribution::load(adv);
  adv.loadAttribute( "k_", k_ );
  adv.loadAttribute( "n_", n_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
