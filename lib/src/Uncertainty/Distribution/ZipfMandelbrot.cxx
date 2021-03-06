//                                               -*- C++ -*-
/**
 *  @file  ZipfMandelbrot.cxx
 *  @brief The ZipfMandelbrot distribution
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
#include "ZipfMandelbrot.hxx"
#include "DistFunc.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(ZipfMandelbrot);

static Factory<ZipfMandelbrot> RegisteredFactory("ZipfMandelbrot");

/* Default constructor */
ZipfMandelbrot::ZipfMandelbrot()
  : DiscreteDistribution("ZipfMandelbrot"),
    isAlreadyComputedHarmonicNumbers_(false),
    harmonicNumbers_(0),
    n_(1),
    q_(0.0),
    s_(1.0)
{
  // We set the dimension of the ZipfMandelbrot distribution
  setDimension( 1 );
  computeRange();
}

/* Parameters constructor */
ZipfMandelbrot::ZipfMandelbrot(const UnsignedLong n,
                               const NumericalScalar q,
                               const NumericalScalar s )
  : DiscreteDistribution("ZipfMandelbrot"),
    isAlreadyComputedHarmonicNumbers_(false),
    harmonicNumbers_(NumericalScalarCollection(0)),
    n_(n),
    q_(q),
    s_(s)
{
  // We set the dimension of the ZipfMandelbrot distribution
  setDimension( 1 );
  computeRange();
}

/* Comparison operator */
Bool ZipfMandelbrot::operator ==(const ZipfMandelbrot & other) const
{
  if (this == &other) return true;
  return (n_ == other.n_) && (q_ == other.q_) && (s_ == other.s_);
}

/* String converter */
String ZipfMandelbrot::__repr__() const
{
  OSS oss;
  oss << "class=" << ZipfMandelbrot::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " n=" << n_
      << " q=" << q_
      << " s=" << s_;
  return oss;
}

String ZipfMandelbrot::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(n = " << n_ << ", q = " << q_ << ", s = " << s_ << ")";
  return oss;
}

/* Virtual constructor */
ZipfMandelbrot * ZipfMandelbrot::clone() const
{
  return new ZipfMandelbrot(*this);
}

/* Get one realization of the distribution */
NumericalPoint ZipfMandelbrot::getRealization() const
{
  const NumericalScalar uniformRealization(1.0 - RandomGenerator::Generate());

  if (!isAlreadyComputedHarmonicNumbers_) computeHarmonicNumbers();

  NumericalScalarCollection::iterator it(lower_bound(harmonicNumbers_.begin(),
                                                     harmonicNumbers_.end(),
                                                     uniformRealization * getHarmonicNumbers(n_))
                                         );
  return NumericalPoint(1, it - harmonicNumbers_.begin() + 1);
}


/* Get the PDF of the distribution */
NumericalScalar ZipfMandelbrot::computePDF(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);

  if ((k < 1 - ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) || (fabs(k - round(k)) > ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) || (k > n_ + ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"))) return 0.0;
  return 1.0 / (pow(round(k) + q_, s_) * getHarmonicNumbers(n_) );
}


/* Get the CDF of the distribution */
NumericalScalar ZipfMandelbrot::computeCDF(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);

  if (k < 1 - ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return 0.0;
  if (k > n_ + ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return 1.0;

  NumericalScalar value (getHarmonicNumbers(static_cast<UnsignedLong>(round(k))) / getHarmonicNumbers(n_));
  return value;
}

/* Compute the mean of the distribution */
void ZipfMandelbrot::computeMean() const
{
  NumericalScalar value(0.0);
  for (UnsignedLong i = 1; i <= n_; ++i) value += i * pow(i + q_, -s_);
  mean_ = NumericalPoint(1, value / getHarmonicNumbers(n_));
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint ZipfMandelbrot::getStandardDeviation() const
{
  return NumericalPoint(1, sqrt(getCovariance()(0, 0)));
}

/* Get the skewness of the distribution */
NumericalPoint ZipfMandelbrot::getSkewness() const
{
  NumericalScalar mean(getMean()[0]);
  NumericalScalar std(getStandardDeviation()[0]);
  NumericalScalar value(0.0);
  for (UnsignedLong i = 1; i <= n_; ++i) value += pow((i - mean) / std, 3) * pow(i + q_, -s_);
  return NumericalPoint(1, value / getHarmonicNumbers(n_));
}

/* Get the kurtosis of the distribution */
NumericalPoint ZipfMandelbrot::getKurtosis() const
{
  NumericalScalar mean(getMean()[0]);
  NumericalScalar std(getStandardDeviation()[0]);
  NumericalScalar value(0.0);
  for (UnsignedLong i = 1; i <= n_; ++i) value += pow((i - mean) / std, 4) * pow(i + q_, -s_);
  return NumericalPoint(1, value / getHarmonicNumbers(n_));
}

/* Compute the covariance of the distribution */
void ZipfMandelbrot::computeCovariance() const
{
  NumericalScalar mean(getMean()[0]);
  NumericalScalar value(0.0);
  for (UnsignedLong i = 1; i <= n_; ++i) value += pow(i - mean, 2) * pow(i + q_, -s_);
  covariance_ = CovarianceMatrix(1);
  covariance_(0, 0) = value / getHarmonicNumbers(n_);
  isAlreadyComputedCovariance_ = true;
}

/* Get the quantile of the distribution */
NumericalScalar ZipfMandelbrot::computeScalarQuantile(const NumericalScalar prob,
                                                      const Bool tail,
                                                      const NumericalScalar precision) const
{
  return round(DiscreteDistribution::computeScalarQuantile(prob, tail));
}

/* Q accessor */
NumericalScalar ZipfMandelbrot::getQ() const
{
  return q_;
}

void ZipfMandelbrot::setQ(const NumericalScalar q)
{
  if (q < 0) throw InvalidArgumentException(HERE) << "q must be >= 0";
  if (q != q_)
    {
      q_ = q;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      isAlreadyCreatedGeneratingFunction_ = false;
    }
}

/* S accessor */
NumericalScalar ZipfMandelbrot::getS() const
{
  return s_;
}

void ZipfMandelbrot::setS(const NumericalScalar s)
{
  if (s <= 0) throw InvalidArgumentException(HERE) << "s must be > 0";
  if (s != s_)
    {
      s_ = s;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      isAlreadyCreatedGeneratingFunction_ = false;
    }
}

/* N accessor */
UnsignedLong ZipfMandelbrot::getN() const
{
  return n_;
}

void ZipfMandelbrot::setN(const UnsignedLong n)
{
  if (n == 0) throw InvalidArgumentException(HERE) << "N must be > 0";
  if (n != n_)
    {
      n_ = n;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      isAlreadyCreatedGeneratingFunction_ = false;
      computeRange();
    }
}


/* Compute the numerical range of the distribution given the parameters values */
void ZipfMandelbrot::computeRange()
{
  const NumericalPoint lowerBound(1, 1.0);
  const NumericalPoint upperBound(1, n_);
  const Interval::BoolCollection finiteLowerBound(1, true);
  const Interval::BoolCollection finiteUpperBound(1, true);
  setRange(Interval(lowerBound, upperBound, finiteLowerBound, finiteUpperBound));
}

/* Get the support of a discrete distribution that intersect a given interval */
NumericalSample ZipfMandelbrot::getSupport(const Interval & interval) const
{
  if (interval.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given interval has a dimension that does not match the distribution dimension.";
  const UnsignedLong kMin(static_cast< UnsignedLong > (std::max(ceil(interval.getLowerBound()[0]), 1.0)));
  const UnsignedLong kMax(static_cast< UnsignedLong > (std::min(floor(interval.getUpperBound()[0]), NumericalScalar(n_))));
  NumericalSample result(0, 1);
  for (UnsignedLong k = kMin; k <= kMax; ++k) result.add(NumericalPoint(1, k));
  return result;
}

/* Parameters value and description accessor */
ZipfMandelbrot::NumericalPointWithDescriptionCollection ZipfMandelbrot::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(3);
  Description description(point.getDimension());
  point[0] = n_;
  point[1] = q_;
  point[2] = s_;
  description[0] = "n";
  description[1] = "q";
  description[2] = "s";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void ZipfMandelbrot::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = ZipfMandelbrot(static_cast< UnsignedLong >(round(parametersCollection[0][0])), parametersCollection[0][1], parametersCollection[0][2]);
}


/* Method save() stores the object through the StorageManager */
void ZipfMandelbrot::save(Advocate & adv) const
{
  DiscreteDistribution::save(adv);
  adv.saveAttribute( "n_", n_ );
  adv.saveAttribute( "q_", q_ );
  adv.saveAttribute( "s_", s_ );
}

/* Method load() reloads the object from the StorageManager */
void ZipfMandelbrot::load(Advocate & adv)
{
  DiscreteDistribution::load(adv);
  adv.loadAttribute( "n_", n_ );
  adv.loadAttribute( "q_", q_ );
  adv.loadAttribute( "s_", s_ );
  isAlreadyComputedHarmonicNumbers_ = false;
  harmonicNumbers_ = NumericalScalarCollection(0);
  computeRange();
}


/* Method getHarmonicNumbers returns the k-th harmonic number for the current distribution */
NumericalScalar ZipfMandelbrot::getHarmonicNumbers(const UnsignedLong k ) const
{
  if (! isAlreadyComputedHarmonicNumbers_)computeHarmonicNumbers();

  if (k < 1) throw InvalidArgumentException(HERE) << "k must be >= 1" ;
  if (k > n_) throw InvalidArgumentException(HERE) << "k must be <= N";

  return harmonicNumbers_[k - 1];
}

/* Method  computeHarmonicNumbers computes and stores the  of the k harmonic numbers for
   k = 1..n
   harmonicNumbers_[i] = \sum_{l=1}^i 1./( (i+q)**s )
*/
void ZipfMandelbrot::computeHarmonicNumbers() const
{
  harmonicNumbers_ = NumericalScalarCollection(n_);
  harmonicNumbers_[0] = pow(1.0 + q_, -s_);
  for (UnsignedLong i = 2; i <= n_; ++i)
    {
      const NumericalScalar hiqs(pow(i + q_, -s_));
      harmonicNumbers_[i - 1] = harmonicNumbers_[i - 2] + hiqs;
    }
  isAlreadyComputedHarmonicNumbers_ = true;
}


END_NAMESPACE_OPENTURNS
