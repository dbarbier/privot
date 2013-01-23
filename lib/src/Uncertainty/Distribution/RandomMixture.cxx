//                                               -*- C++ -*-
/**
 *  @file  RandomMixture.cxx
 *  @brief Abstract top-level class for all RandomMixtures
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <algorithm>

#include "RandomMixture.hxx"
#include "SpecFunc.hxx"
#include "PersistentObjectFactory.hxx"
#include "MethodBoundNumericalMathEvaluationImplementation.hxx"
#include "TNC.hxx"
#include "Interval.hxx"
#include "NumericalMathFunction.hxx"
#include "Log.hxx"

BEGIN_NAMESPACE_OPENTURNS

TEMPLATE_CLASSNAMEINIT(PersistentCollection<NumericalComplex>);
static Factory<PersistentCollection<NumericalComplex> > RegisteredFactory2("PersistentCollection<NumericalComplex>");
TEMPLATE_CLASSNAMEINIT(PersistentCollection<Distribution>);
static Factory<PersistentCollection<Distribution> > RegisteredFactory3("PersistentCollection<Distribution>");


typedef Collection<Distribution> DistributionCollection;
typedef Collection<NumericalPoint> NumericalPointCollection;


CLASSNAMEINIT(RandomMixture);

static Factory<RandomMixture> RegisteredFactory("RandomMixture");

/* Default constructor */
RandomMixture::RandomMixture(const DistributionCollection & coll,
                             const NumericalScalar constant)
  : DistributionImplementation("RandomMixture"),
    distributionCollection_(),
    constant_(constant),
    blockMin_(ResourceMap::GetAsUnsignedLong( "RandomMixture-DefaultBlockMin" )),
    blockMax_(ResourceMap::GetAsUnsignedLong( "RandomMixture-DefaultBlockMax" )),
    maxSize_(ResourceMap::GetAsUnsignedLong( "RandomMixture-DefaultMaxSize"  )),
    storedSize_(0),
    characteristicValuesCache_(0),
    alpha_(ResourceMap::GetAsNumericalScalar( "RandomMixture-DefaultAlpha" )),
    beta_(0.0),
    pdfPrecision_(ResourceMap::GetAsNumericalScalar( "RandomMixture-DefaultPDFEpsilon" )),
    cdfPrecision_(ResourceMap::GetAsNumericalScalar( "RandomMixture-DefaultCDFEpsilon" )),
    equivalentNormal_()
{
  // We could NOT set distributionCollection_ in the member area of the constructor
  // because we must check before if the collection is valid (ie, if all the
  // distributions of the collection have the same dimension). We do this by calling
  // the setDistributionCollection() method that do it for us.
  // This call set also the range.
  setDistributionCollection( coll );
}

/* Default constructor */
RandomMixture::RandomMixture(const DistributionCollection & coll,
                             const NumericalPoint & weights,
                             const NumericalScalar constant)
  : DistributionImplementation("RandomMixture"),
    distributionCollection_(),
    constant_(constant),
    blockMin_(ResourceMap::GetAsUnsignedLong( "RandomMixture-DefaultBlockMin" )),
    blockMax_(ResourceMap::GetAsUnsignedLong( "RandomMixture-DefaultBlockMax" )),
    maxSize_(ResourceMap::GetAsUnsignedLong( "RandomMixture-DefaultMaxSize"  )),
    storedSize_(0),
    characteristicValuesCache_(0),
    alpha_(ResourceMap::GetAsNumericalScalar( "RandomMixture-DefaultAlpha" )),
    beta_(0.0),
    pdfPrecision_(ResourceMap::GetAsNumericalScalar( "RandomMixture-DefaultPDFEpsilon" )),
    cdfPrecision_(ResourceMap::GetAsNumericalScalar( "RandomMixture-DefaultCDFEpsilon" )),
    equivalentNormal_()
{
  // We could NOT set distributionCollection_ in the member area of the constructor
  // because we must check before if the collection is valid (ie, if all the
  // distributions of the collection have the same dimension). We do this by calling
  // the setDistributionCollection() method that do it for us.
  if (weights.getDimension() != coll.getSize()) throw InvalidArgumentException(HERE) << "Error: the weights collection must have the same size as the distribution collection";
  DistributionCollection weightedCollection(coll);
  const UnsignedLong size(coll.getSize());
  for (UnsignedLong i = 0; i < size; ++i) weightedCollection[i].setWeight(weights[i]);
  // This call set also the range.
  setDistributionCollection(weightedCollection);
}

/* Compute the numerical range of the distribution given the parameters values */
void RandomMixture::computeRange()
{
  const NumericalScalar m(getPositionIndicator());
  const NumericalScalar s(getDispersionIndicator());
  const UnsignedLong size(distributionCollection_.getSize());
  Interval range(constant_, constant_);
  for (UnsignedLong i = 0; i < size; ++i) range += distributionCollection_[i].getRange() * distributionCollection_[i].getWeight();
  setRange(range.intersect(Interval(m - beta_ * s, m + beta_ * s)));
}

/* Comparison operator */
Bool RandomMixture::operator ==(const RandomMixture & other) const
{
  if (this == &other) return true;
  return (distributionCollection_ == other.distributionCollection_) && (constant_ == other.constant_);
}

/* String converter */
String RandomMixture::__repr__() const
{
  OSS oss;
  oss << "class=" << RandomMixture::GetClassName()
      << " name=" << getName()
      << " distribution collection=" << distributionCollection_
      << " constant=" << constant_;
  return oss;
}

/* String converter */
String RandomMixture::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(";
  const UnsignedLong size(distributionCollection_.getSize());
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar w(distributionCollection_[i].getWeight());
      if (i > 0)
	{
	  if (w > 0.0) oss << " + ";
	  else oss << " - ";
	}
      if ((i == 0) && (w < 0.0)) oss << "-";
      const String coeff(OSS() << fabs(w));
      if (coeff != "1") oss << fabs(w) << " * ";
      oss << distributionCollection_[i].__str__();
    }
  oss << ")";
  return oss;
}

/* Weights distribution accessor */
void RandomMixture::setWeights(const NumericalPoint & weights)
{
  if (weights.getDimension() != distributionCollection_.getSize()) throw InvalidArgumentException(HERE) << "Error: the weights collection must have the same size as the distribution collection";
  const UnsignedLong size(distributionCollection_.getSize());
  for (UnsignedLong i = 0; i < size; ++i) distributionCollection_[i].setWeight(weights[i]);
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  computePositionIndicator();
  computeDispersionIndicator();
  computeBeta();
  // Must be after computeBeta
  computeRange();
  computeReferenceBandwidth();
  computeEquivalentNormal();
}

NumericalPoint RandomMixture::getWeights() const
{
  const UnsignedLong size(distributionCollection_.getSize());
  NumericalPoint weights(size);
  for (UnsignedLong i = 0; i < size; ++i) weights[i] = distributionCollection_[i].getWeight();
  return weights;
}


/* Distribution collection accessor */
void RandomMixture::setDistributionCollection(const DistributionCollection & coll)
{
  const UnsignedLong size(coll.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a RandomMixture based on an empty distribution collection.";
  Bool hasNormalAtom(false);
  NumericalScalar aggregatedMean(0.0);
  NumericalScalar aggregatedVariance(0.0);
  distributionCollection_ = DistributionCollection(0);
  for(UnsignedLong i = 0; i < size; ++i)
    {
      if (coll[i].getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: a RandomMixture cannot be built from a collection of distributions of dimension not equal to 1";
      // Knowledge-based optimization
      if (coll[i].getImplementation()->getClassName() == "Normal")
        {
          hasNormalAtom = true;
          const NumericalScalar w(coll[i].getWeight());
          aggregatedMean += w * coll[i].getMean()[0];
          aggregatedVariance += w * w * coll[i].getCovariance()(0, 0);
        } // atom is a Normal
      else if (coll[i].getImplementation()->getClassName() == "Dirac")
        {
          const NumericalScalar w(coll[i].getWeight());
          constant_ += w * coll[i].getSupport()[0][0];
        } // atom is a Dirac
      else if (coll[i].getImplementation()->getClassName() == "RandomMixture")
        {
          const NumericalScalar w(coll[i].getWeight());
          const RandomMixture * mixture(static_cast< const RandomMixture * >(coll[i].getImplementation().get()));
          // As the random mixture atom has already been built, all its Dirac components have been merged into its constant and all its
          // Normal components have been merged into a single component in the last place, if any.
          // First, merge the constant
          constant_ += w * mixture->constant_;
          // Second, merge the potential Normal component
          UnsignedLong atomSize(mixture->distributionCollection_.getSize());
          if (mixture->distributionCollection_[atomSize - 1].getImplementation()->getClassName() == "Normal")
            {
              hasNormalAtom = true;
              aggregatedMean += w * mixture->distributionCollection_[atomSize - 1].getMean()[0];
              aggregatedVariance += w * w * mixture->distributionCollection_[atomSize - 1].getCovariance()(0, 0);
	      --atomSize;
            }
	  // Third, merge the remaining components
	  for (UnsignedLong j = 0; j < atomSize; ++j)
	    {
	      Distribution atom(mixture->distributionCollection_[j]);
	      atom.setWeight(atom.getWeight() * w);
	      distributionCollection_.add(atom);
	    }
        } // atom is a RandomMixture
      else distributionCollection_.add(coll[i]);
    }
  // Set the aggregated normal as the last atom
  if (hasNormalAtom) distributionCollection_.add(Normal(aggregatedMean, sqrt(aggregatedVariance)));
  setDimension(1);
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  computePositionIndicator();
  computeDispersionIndicator();
  computeBeta();
  // Must be after computeBeta
  computeRange();
  computeReferenceBandwidth();
  computeEquivalentNormal();
}

/* Constant accessor */
void RandomMixture::setConstant(const NumericalScalar constant)
{
  if (constant != constant_)
    {
      constant_ = constant;
      isAlreadyComputedMean_ = false;
      // The covariance does not depend on the constant
      computeRange();
    }
}

NumericalScalar RandomMixture::getConstant() const
{
  return constant_;
}

/* Distribution collection accessor */
const DistributionCollection & RandomMixture::getDistributionCollection() const
{
  return distributionCollection_;
}

/* Virtual constructor */
RandomMixture * RandomMixture::clone() const
{
  return new RandomMixture(*this);
}

/* Get one realization of the RandomMixture */
NumericalPoint RandomMixture::getRealization() const
{
  const UnsignedLong size(distributionCollection_.getSize());
  NumericalScalar realization(constant_);
  for (UnsignedLong i = 0; i < size; ++i) realization += distributionCollection_[i].getWeight() * distributionCollection_[i].getRealization()[0];
  return NumericalPoint(1, realization);
}


/* Get the DDF of the RandomMixture */
NumericalPoint RandomMixture::computeDDF(const NumericalPoint & point) const
{
  // A specific implementation should be used. For now, we simply explicitely call the default implementation
  return DistributionImplementation::computeDDF(point);
}

/* Get the PDF of the RandomMixture. It uses the Poisson inversion formula as described in the reference:
   "Abate, J. and Whitt, W. (1992). The Fourier-series method for inverting
   transforms of probability distributions. Queueing Systems 10, 5--88., 1992",
   formula 5.5.
   We use an incremental update of the trigonometric functions and reduce the complex arithmetic to a real
   arithmetic for performance purpose.

   Here, we recall the Poisson summation formula:
   \sum_{k\in Z}p(x+2k\pi/h) = h/2\pi\sum_{j\in Z}\phi(jh)\exp(-Ihjx)
   We can rewrite this formula as:
   \sum_{k\in Z}p(x+2k\pi/h) = h/2\pi\sum_{j\in Z}\left[\phi(jh) - \psi(jh)\right]\exp(-Ihjx) + \sum_{k\in Z}q(x+2k\pi/h),
   where q is the PDF and \psi the characteristic function of the normal distribution with the same mean and
   the same variance as the mixture. Take h such as p(x+2k\pi/h) << p(x) for k\neq 0, then:
   p(x) \simeq h/2\pi\sum_{j\in Z}\left[\phi(jh) - \psi(jh)\right]\exp(-Ihjx) + \sum_{k\in Z}q(x+2k\pi/h).
   The second sum \sum_{k\in Z}q(x+2k\pi/h) will be approximated using only few terms, as the condition on h will almost
   gives q(x+2k\pi/h) << q(x) for k\neq 0. Call this sum Q(x, h), and define \delta as delta(t) = \phi(t) - \psi(t).
   We unroll the complex arithmetic for performance purpose:
   p(x) \simeq h/2\pi\sum_{j\neq 0}\delta(jh)\exp(-Ihjx) + Q(x, h) as \delta(0) = 0
   \simeq h/\pi\sum_{j>0} Re(\delta(jh)) * cos(jhx) + Im(\delta(jh)) * sin(jhx) + Q(x, h)
*/
NumericalScalar RandomMixture::computePDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const UnsignedLong size(distributionCollection_.getSize());
  const NumericalScalar x(point[0]);
  // Special case for combination containing only one contributor
  if (size == 1) return distributionCollection_[0].computePDF((x - constant_) / distributionCollection_[0].getWeight());
  const Interval range(getRange());
  const NumericalScalar lowerBound(range.getLowerBound()[0]);
  const NumericalScalar upperBound(range.getUpperBound()[0]);
  // Check range
  if ((x <= lowerBound) || (x >= upperBound)) return 0.0;
  // We unroll the complex arithmetic and we perform incremental update in order to improve the performances
  const NumericalScalar hX(referenceBandwidth_ * x);
  const NumericalScalar cosHX(cos(hX));
  const NumericalScalar sinHX(sin(hX));
  const NumericalScalar factor(referenceBandwidth_ / M_PI);
  NumericalScalar value(computeEquivalentNormalPDFSum(x));
  NumericalScalar cosMHX(cosHX);
  NumericalScalar sinMHX(sinHX);
  UnsignedLong k(1);
  const NumericalScalar precision(pdfPrecision_);
  const UnsignedLong kmin(1 << blockMin_);
  const UnsignedLong kmax(1 << blockMax_);
  NumericalScalar error(2.0 * precision);
  while ( (k < kmin) || ( (k < kmax) && (error > precision))) // (error > std::max(precision, fabs(precision * value))) ) )
    {
      error = 0.0;
      for (UnsignedLong m = k + 1; m <= 2 * k; ++m)
        {
          const NumericalScalar tmp(cosMHX);
          const NumericalComplex cfValue(computeDeltaCharacteristicFunction(m - 1));
          error += factor * (cfValue.real() * cosMHX + cfValue.imag() * sinMHX);
          cosMHX = tmp * cosHX - sinMHX * sinHX;
          sinMHX = sinMHX * cosHX + tmp * sinHX;
        }
      value += error;
      error = fabs(error);
      k *= 2;
    }
  // For very low level of PDF, the computed value can be slightly negative. Round it up to zero.
  if (value < 0.0) value = 0.0;
  pdfEpsilon_ = error;
  return value;
}

/*  Compute the PDF of 1D distributions over a regular grid. The precision is reduced as this method is for drawing purpose only. */
NumericalSample RandomMixture::computePDF(const NumericalScalar xMin,
                                          const NumericalScalar xMax,
                                          const UnsignedLong pointNumber,
                                          const NumericalScalar precision) const
{
  const NumericalScalar oldPDFPrecision(pdfPrecision_);
  pdfPrecision_ = std::max(precision / distributionCollection_.getSize(), pdfPrecision_);
  pdfPrecision_ = oldPDFPrecision;
  return DistributionImplementation::computePDF(xMin, xMax, pointNumber, precision);
}

/* Get the CDF of the RandomMixture */
NumericalScalar RandomMixture::computeCDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const UnsignedLong size(distributionCollection_.getSize());
  const NumericalScalar x(point[0]);
  // Special case for combination containing only one contributor Y = alpha * X + beta
  // for alpha > 0.0:
  // P(Y < y) = P(X < (y - beta) / alpha) = CDF_X((y - beta) / alpha)
  // for alpha < 0.0:
  // P(Y < y) = P(X > (y - beta) / alpha) = 1.0 - CDF_X((y - beta) / alpha)
  if (size == 1)
    {
      const NumericalScalar alpha(distributionCollection_[0].getWeight());
      if (alpha > 0.0) return distributionCollection_[0].computeCDF((x - constant_) / alpha);
      // If alpha < 0.0, compute the CDF with negated tail
      return distributionCollection_[0].computeComplementaryCDF((x - constant_) / alpha);
    }
  // Check range
  const Interval range(getRange());
  const NumericalScalar lowerBound(range.getLowerBound()[0]);
  const NumericalScalar upperBound(range.getUpperBound()[0]);
  if (x <= lowerBound) return 0.0;
  if (x >= upperBound) return 1.0;
  // Here we call computeProbability with a ]-inf, x] interval
  const Interval interval(NumericalPoint(1, lowerBound), point, getRange().getFiniteLowerBound(), Interval::BoolCollection(1, true));
  return computeProbability(interval);
}

NumericalScalar RandomMixture::computeComplementaryCDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const UnsignedLong size(distributionCollection_.getSize());
  const NumericalScalar x(point[0]);
  // Special case for combination containing only one contributor Y = alpha * X + beta
  // for alpha > 0.0:
  // P(Y < y) = P(X < (y - beta) / alpha) = CDF_X((y - beta) / alpha)
  // for alpha < 0.0:
  // P(Y < y) = P(X > (y - beta) / alpha) = 1.0 - CDF_X((y - beta) / alpha)
  if (size == 1)
    {
      const NumericalScalar alpha(distributionCollection_[0].getWeight());
      if (alpha > 0.0) return distributionCollection_[0].computeComplementaryCDF((x - constant_) / alpha);
      // If alpha < 0.0, compute the CDF with negated tail
      return distributionCollection_[0].computeCDF((x - constant_) / alpha);
    }
  // Check range
  const Interval range(getRange());
  const NumericalScalar lowerBound(range.getLowerBound()[0]);
  const NumericalScalar upperBound(range.getUpperBound()[0]);
  if (x <= lowerBound) return 1.0;
  if (x >= upperBound) return 0.0;
  // Here we call computeProbability with a [x, +inf[ interval
  const Interval interval(point, NumericalPoint(1, upperBound), Interval::BoolCollection(1, true), getRange().getFiniteUpperBound());
  return computeProbability(interval);
}

/*  Compute the CDF of 1D distributions over a regular grid. The precision is reduced as this method is for drawing purpose only. */
NumericalSample RandomMixture::computeCDF(const NumericalScalar xMin,
                                          const NumericalScalar xMax,
                                          const UnsignedLong pointNumber,
                                          const NumericalScalar precision) const
{
  const NumericalScalar oldCDFPrecision(cdfPrecision_);
  //        cdfPrecision_ = std::max(precision / distributionCollection_.getSize(), cdfPrecision_);
  //         if (getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot compute the CDF over a regular 1D grid if the dimension is > 1";
  //         NumericalSample result(pointNumber, 2);
  //         NumericalScalar x(xMin);
  //         NumericalScalar step((xMax - xMin) / NumericalScalar(pointNumber - 1.0));
  //      result[0][0] = x;
  //      result[0][1] = computeCDF(x, tail);
  //         for (UnsignedLong i = 1; i < pointNumber; ++i)
  //           {
  //          x += step;
  //             result[i][0] = x;
  //          if (tail)
  //            result[i][1] = result[i-1][1] - computeProbability(Interval(x - step, x));
  //          else
  //            result[i][1] = result[i-1][1] + computeProbability(Interval(x - step, x));
  //           }
  NumericalSample result(DistributionImplementation::computeCDF(xMin, xMax, pointNumber, precision));
  cdfPrecision_ = oldCDFPrecision;
  return result;
}

/* Get the probability content of an interval. It uses the Poisson inversion formula as described in the reference:
   "Abate, J. and Whitt, W. (1992). The Fourier-series method for inverting
   transforms of probability distributions. Queueing Systems 10, 5--88., 1992",
   formula 5.14.
   We use an incremental update of the trigonometric functions and reduce the complex arithmetic to a real
   arithmetic for performance purpose.
*/
NumericalScalar RandomMixture::computeProbability(const Interval & interval) const
{
  if (interval.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given interval must have dimension=1, here dimension=" << interval.getDimension();

  if (interval.isNumericallyEmpty()) return 0.0;
  const UnsignedLong size(distributionCollection_.getSize());
  // Special case for combination containing only one contributor
  if (size == 1)
    {
      cdfEpsilon_ = ResourceMap::GetAsNumericalScalar( "RandomMixture-DefaultCDFEpsilon" );
      const NumericalScalar lower(interval.getLowerBound()[0]);
      const NumericalScalar upper(interval.getUpperBound()[0]);
      const NumericalScalar weight(getWeight());
      // Negative weight, swap upper and lower bound flags
      if (weight < 0.0)
        {
          const Interval adjustedInterval(NumericalPoint(1, (upper - constant_) / weight), NumericalPoint(1, (lower - constant_) / weight), interval.getFiniteUpperBound(), interval.getFiniteUpperBound());
          return distributionCollection_[0].computeProbability(adjustedInterval.intersect(getRange()));
        }
      else
        {
          const Interval adjustedInterval(NumericalPoint(1, (lower - constant_) / weight), NumericalPoint(1, (upper - constant_) / weight), interval.getFiniteUpperBound(), interval.getFiniteUpperBound());
          return distributionCollection_[0].computeProbability(interval.intersect(getRange()));
        }
    }
  const Interval clippedInterval(getRange().intersect(interval));
  // Quick return if there is no mass in the clipped interval
  if (clippedInterval.isNumericallyEmpty()) return 0.0;
  const Bool finiteLowerBound(clippedInterval.getFiniteLowerBound()[0]);
  const Bool finiteUpperBound(clippedInterval.getFiniteUpperBound()[0]);
  // Quick return for integral over the whole real line
  if (!finiteLowerBound && !finiteUpperBound) return 1.0;
  const NumericalScalar lowerBound(clippedInterval.getLowerBound()[0]);
  const NumericalScalar upperBound(clippedInterval.getUpperBound()[0]);
  // Quick return if the clipped interval contains the range
  if ((lowerBound <= getRange().getLowerBound()[0]) && (upperBound >= getRange().getUpperBound()[0])) return 1.0;
  // The Fourier series method is not numerically stable for large mixtures, default to standard integration
  if (distributionCollection_.getSize() >= ResourceMap::GetAsUnsignedLong( "RandomMixture-SmallSize" ))
    {
      setIntegrationNodesNumber(128);
      const NumericalSample nodesAndWeights(getGaussNodesAndWeights());
      const UnsignedLong numberOfNodes(nodesAndWeights.getDimension());
      NumericalScalar cdf(0.0);
      const NumericalScalar halfLength(0.5 * (upperBound - lowerBound));
      cdfEpsilon_ = 0.0;
      // Map [x-8*s,x] into [-1,1]
      for (UnsignedLong i = 0; i < numberOfNodes; ++i)
        {
          const NumericalScalar w(nodesAndWeights[1][i]);
          const NumericalScalar xi(nodesAndWeights[0][i]);
          NumericalScalar z(lowerBound + (1.0 + xi) * halfLength);
          const NumericalScalar pdf(computePDF(z));
          // Accumulate CDF value
          cdf += w * pdf;
          // Compute an approximation of the rounding error
          cdfEpsilon_ += w * getPDFEpsilon();
        }
      cdf *= halfLength;
      return cdf;
    }
  // Small size case: use Fourier series
  const NumericalScalar precision(cdfPrecision_);
  NumericalScalar error(2.0 * precision);
  // We unroll the complex arithmetic and we perform incremental update in order to improve the performances
  NumericalScalar mH(referenceBandwidth_);
  const NumericalScalar a(referenceBandwidth_ * lowerBound);
  const NumericalScalar b(referenceBandwidth_ * upperBound);
  const NumericalScalar cosHUpper(cos(b));
  const NumericalScalar sinHUpper(sin(b));
  const NumericalScalar cosHLower(cos(a));
  const NumericalScalar sinHLower(sin(a));
  NumericalScalar cosMHUpper(cosHUpper);
  NumericalScalar sinMHUpper(sinHUpper);
  NumericalScalar cosMHLower(cosHLower);
  NumericalScalar sinMHLower(sinHLower);
  const NumericalScalar factor(referenceBandwidth_ / M_PI);
  NumericalComplex cfValue(0.0);
  const NumericalScalar equivalentNormalContribution(computeEquivalentNormalCDFSum(lowerBound, upperBound));
  NumericalScalar value(equivalentNormalContribution);
  UnsignedLong k(1);
  const UnsignedLong kmin(1 << blockMin_);
  const UnsignedLong kmax(1 << blockMax_);
  while ( (k < kmax) && (error > std::max(precision, fabs(precision * value)) || k < kmin) )
    {
      error = 0.0;
      for (UnsignedLong m = k + 1; m <= 2 * k; ++m)
        {
          cfValue = computeDeltaCharacteristicFunction(m - 1);
          error += factor * (cfValue.real() * (sinMHUpper - sinMHLower) + cfValue.imag() * (cosMHLower - cosMHUpper)) / mH;
          NumericalScalar tmp(cosMHUpper * cosHUpper - sinMHUpper * sinHUpper);
          sinMHUpper = sinMHUpper * cosHUpper + cosMHUpper * sinHUpper;
          cosMHUpper = tmp;
          tmp = cosMHLower * cosHLower - sinMHLower * sinHLower;
          sinMHLower = sinMHLower * cosHLower + cosMHLower * sinHLower;
          cosMHLower = tmp;
          mH += referenceBandwidth_;
        }
      value += error;
      error = fabs(error);
      k *= 2;
    }
  cdfEpsilon_ = error;
  // For extrem values of the argument, the computed value can be slightly outside of [0,1]. Truncate it.
  return (value < 0.0 ? 0.0 : (value > 1.0 ? 1.0 : value));
}

/*  Compute the quantile over a regular grid */
NumericalSample RandomMixture::computeQuantile(const NumericalScalar qMin,
                                               const NumericalScalar qMax,
                                               const UnsignedLong pointNumber,
                                               const NumericalScalar precision,
                                               const Bool tail) const
{
  if (getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot compute the quantile over a regular 1D grid if the dimension is > 1";
  const NumericalScalar oldCDFPrecision(cdfPrecision_);
  const NumericalScalar oldPDFPrecision(pdfPrecision_);
  NumericalSample result(pointNumber, 2);
  NumericalScalar q(qMin);
  const NumericalScalar step((qMax - qMin) / NumericalScalar(pointNumber - 1.0));
  for (UnsignedLong i = 0; i < pointNumber; ++i)
    {
      result[i][0] = q;
      result[i][1] = computeQuantile(q)[0];
      q += step;
    }
  cdfPrecision_ = oldCDFPrecision;
  pdfPrecision_ = oldPDFPrecision;
  return result;
  // const NumericalScalar dX((xMax - xMin) / (pointNumber - 1.0));
  // const NumericalSample dataCDF(computeCDF(xMin, xMax, pointNumber, cdfPrecision_, tail));
  // NumericalSample result(pointNumber, 2);
  // NumericalScalar q(qMin);
  // const NumericalScalar step((qMax - qMin) / NumericalScalar(pointNumber - 1.0));
  // // First point, exact
  // result[0][0] = qMin;
  // result[0][1] = xMin;
  // // Last point, exact
  // result[pointNumber - 1][0] = qMax;
  // result[pointNumber - 1][1] = xMax;
  // UnsignedLong indexCDF(0);
  // // Intermediate points, inverse quadratic interpolation
  // for (UnsignedLong i = 1; i < pointNumber - 1; ++i)
  //   {
  //     q += step;
  //     // We are sure that indexCDF will be in [1, pointNumber-1] as dataCDF[0] = qMin and dataCDF[pointNumber-1] = qMax
  //     while ((indexCDF < pointNumber) && (dataCDF[indexCDF][1] < q)) ++indexCDF;
  //     // here we use inverse quadratic interpolation
  //     // for q=dataCDF[indexCDF-1], CDF=CDFm, x=xMin+(indexCDF-1)*Dx
  //     //     q=dataCDF[indexCDF], CDF=CDFc, x=xMin+indexCDF*Dx
  //     //     q=dataCDF[indexCDF+1], CDF=CDFp, x=xMin+(indexCDF+1)*Dx
  //     // so x(q)=a*q^2+b*q+c with a, b, c obtained using a CAS software
  //     UnsignedLong index(indexCDF);
  //     // If we are so close to the upper bound that indexCDF = pointNumber - 1
  //     if (indexCDF >= pointNumber - 1) index = pointNumber - 2;
  //     if (indexCDF == 0.0) index = 1;
  //     const NumericalScalar cdfM(dataCDF[index - 1][1]);
  //     const NumericalScalar cdfC(dataCDF[index    ][1]);
  //     const NumericalScalar cdfP(dataCDF[index + 1][1]);
  //     const NumericalScalar denominator((cdfP - cdfC) * (cdfP - cdfM) * (cdfC - cdfM));
  //     result[i][0] = q;
  //     // Interpolation
  //     const NumericalScalar c((xMin * (cdfC * cdfC * cdfM -
  //                                      cdfC * cdfC * cdfP -
  //                                      cdfC * cdfM * cdfM +
  //                                      cdfC * cdfP * cdfP +
  //                                      cdfM * cdfM * cdfP -
  //                                      cdfM * cdfP * cdfP) +
  //                              dX * cdfC * (cdfC * cdfM +
  //                                           cdfC * cdfP -
  //                                           cdfM * cdfM -
  //                                           cdfP * cdfP)
  //                              ) / denominator + index * dX);
  //     const NumericalScalar b(-((cdfC + cdfM) * (cdfC - cdfM) + (cdfC + cdfP) * (cdfC - cdfP)) * dX / denominator);
  //     const NumericalScalar a(dX * ((cdfC - cdfM) - (cdfP - cdfC)) / denominator);
  //     result[i][1] = a * q * q + b * q + c;
  //   }
  // cdfPrecision_ = oldCDFPrecision;
  // pdfPrecision_ = oldPDFPrecision;
  // return result;
}

/* Quantile computation for dimension=1 */
NumericalScalar RandomMixture::computeScalarQuantile(const NumericalScalar prob,
                                                     const Bool tail,
                                                     const NumericalScalar precision) const
{
  // Special case for random mixture with only 1 atom: Y = alpha * X + beta
  // find Yq such that P(Y < Yq) = q
  // i.e. for alpha > 0
  // P(X < (Yq - beta) / alpha) = q -> Yq = alpha * Xq + beta where P(X < Xq) = q
  // and for alpha < 0
  // P(X > (Yq - beta) / alpha) = q i.e. P(X < (Yq - beta) / alpha) = r with r = 1-q -> Yq = alpha * Xr + beta
  if (distributionCollection_.getSize() == 1)
    {
      const NumericalScalar alpha(distributionCollection_[0].getWeight());
      return distributionCollection_[0].computeQuantile((alpha > 0.0) ? (prob) : (1.0 - prob))[0] * alpha + constant_;
    }
  // General case
  return DistributionImplementation::computeScalarQuantile(prob, tail, precision);
}




/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex RandomMixture::computeCharacteristicFunction(const NumericalScalar x) const
{
  if (x == 0.0) return 1.0;
  return exp(computeLogCharacteristicFunction(x));
}

NumericalComplex RandomMixture::computeLogCharacteristicFunction(const NumericalScalar x) const
{
  if (x == 0.0) return 0.0;
  NumericalComplex logCfValue(0.0, constant_ * x);
  const UnsignedLong size(distributionCollection_.getSize());
  const NumericalScalar smallScalar(0.5 * log(SpecFunc::MinNumericalScalar));
  for(UnsignedLong i = 0; i < size; ++i)
    {
      logCfValue += distributionCollection_[i].computeLogCharacteristicFunction(distributionCollection_[i].getWeight() * x);
      // Early exit for null value
      if (logCfValue.real() < smallScalar) break;
    } /* end for */
  return logCfValue;
}

/* Compute a value of the characteristic function on a prescribed discretization. As the value associated with index == 0 is known, it is not stored so for index > 0, the corresponding value is at position index-1 */
NumericalComplex RandomMixture::computeDeltaCharacteristicFunction(const UnsignedLong index) const
{
  if (index == 0) return 0.0;
  // The cached values are computed and stored in an ascending order without hole: this function is always called on sequence starting from 0 to n-1
  // Usual case first: the index is within the already computed values
  if (index <= storedSize_) return characteristicValuesCache_[index - 1];
  // If the index is higher than the maximum allowed storage
  if (index > maxSize_)
    {
      LOGINFO(OSS() << "Cache exceeded in RandomMixture::computeDeltaCharacteristicFunction, consider increasing maxSize_ to " << index);
      const NumericalScalar x(index * referenceBandwidth_);
      return computeCharacteristicFunction(x) - equivalentNormal_.computeCharacteristicFunction(x);
    }
  // Here, the index has not been computed so far, fill-in the gap
  if (index > storedSize_)
    {
      for (UnsignedLong i = storedSize_ + 1; i <= index; ++i)
        characteristicValuesCache_.add(computeCharacteristicFunction(i * referenceBandwidth_) - equivalentNormal_.computeCharacteristicFunction(i * referenceBandwidth_));
      storedSize_ = index;
      return characteristicValuesCache_[storedSize_ - 1];
    }
  // Should never go there
  throw InvalidArgumentException(HERE) << "Error: trying to access to a cached characteristic value in an incorrect pattern.";
}

/* Get the PDF gradient of the distribution */
NumericalPoint RandomMixture::computePDFGradient(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  throw NotYetImplementedException(HERE);
}

/* Get the CDF gradient of the distribution */
NumericalPoint RandomMixture::computeCDFGradient(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  throw NotYetImplementedException(HERE);
}

/* Compute the mean of the RandomMixture */
void RandomMixture::computeMean() const
{
  NumericalScalar mean(constant_);
  const UnsignedLong size(distributionCollection_.getSize());
  for(UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar wi(distributionCollection_[i].getWeight());
      const NumericalScalar meanI(distributionCollection_[i].getMean()[0]);
      mean += wi * meanI;
    } /* end for */
  mean_ = NumericalPoint(1, mean);
  isAlreadyComputedMean_ = true;
}

/* Compute the covariance of the RandomMixture */
void RandomMixture::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  NumericalScalar covariance(0.0);
  const UnsignedLong size(distributionCollection_.getSize());
  for(UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar wi(distributionCollection_[i].getWeight());
      covariance += wi * wi * distributionCollection_[i].getCovariance().operator()(0, 0);
    }
  covariance_(0, 0) = covariance;
  isAlreadyComputedCovariance_ = true;
}

/* Get the standard deviation of the RandomMixture */
NumericalPoint RandomMixture::getStandardDeviation() const
{
  return NumericalPoint(1, sqrt(getCovariance().operator()(0, 0)));
}

/* Get the skewness of the RandomMixture */
NumericalPoint RandomMixture::getSkewness() const
{
  NumericalScalar variance(0.0);
  NumericalScalar skewness(0.0);
  const UnsignedLong size(distributionCollection_.getSize());
  for(UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar wi(distributionCollection_[i].getWeight());
      const NumericalScalar wi2(wi * wi);
      const NumericalScalar vi(distributionCollection_[i].getCovariance().operator()(0, 0));
      variance += wi2 * vi;
      skewness += wi2 * wi * distributionCollection_[i].getSkewness()[0] * pow(vi, 1.5);
    } /* end for */
  skewness *= pow(variance, -1.5);
  return NumericalPoint(1, skewness);
}

/* Get the kurtosis of the RandomMixture */
NumericalPoint RandomMixture::getKurtosis() const
{
  NumericalScalar variance(0.0);
  NumericalScalar kurtosis(0.0);
  const UnsignedLong size(distributionCollection_.getSize());
  NumericalPoint v(size);
  NumericalPoint w2(size);
  for(UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar wi(distributionCollection_[i].getWeight());
      const NumericalScalar wi2(wi * wi);
      w2[i] = wi2;
      const NumericalScalar vi(distributionCollection_[i].getCovariance().operator()(0, 0));
      v[i] = vi;
      variance += wi2 * vi;
      kurtosis += wi2 * wi2 * distributionCollection_[i].getKurtosis()[0] * vi * vi;
      for (UnsignedLong j = 0; j < i; ++j) kurtosis += 6.0 * wi2 * w2[j] * vi * v[j];
    } /* end for */
  kurtosis /= variance * variance;
  return NumericalPoint(1, kurtosis);
}

/** Parameters value and description accessor */
RandomMixture::NumericalPointWithDescriptionCollection RandomMixture::getParametersCollection() const
{
  const UnsignedLong size(distributionCollection_.getSize());
  NumericalPointWithDescriptionCollection parameters(1);
  Description parametersDescription;
  // Form a big NumericalPoint from the parameters of each atom
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const String prefix(distributionCollection_[i].getName());
      const NumericalPointWithDescription atomParameters(distributionCollection_[i].getParametersCollection()[0]);
      const Description atomDescription(atomParameters.getDescription());
      const UnsignedLong atomParameterDimension(atomParameters.getDimension());
      // Add the current atom parameters
      for (UnsignedLong j = 0; j < atomParameterDimension; ++j)
        {
          parameters[0].add(atomParameters[j]);
          parametersDescription.add(OSS() << prefix << "_" << atomDescription[j]);
        }
    }
  parameters[0].setDescription(parametersDescription);
  parameters[0].setName(getName());
  return parameters;
} // getParametersCollection

/* Get a positon indicator for a 1D distribution */
NumericalScalar RandomMixture::getPositionIndicator() const
{
  if (!isAlreadyComputedPositionIndicator_) computePositionIndicator();
  return positionIndicator_;
}

/* Compute a positon indicator for a 1D distribution */
void RandomMixture::computePositionIndicator() const
{
  positionIndicator_ = 0.0;
  const UnsignedLong size(distributionCollection_.getSize());
  // Assume an additive behaviour of the position indicator. It is true for the mean value, and almost true for the median of moderatly skewed distributions
  for(UnsignedLong i = 0; i < size; ++i) positionIndicator_ += distributionCollection_[i].getWeight() * distributionCollection_[i].getPositionIndicator();
  isAlreadyComputedPositionIndicator_ = true;
}


/* Get a dispersion indicator for a 1D distribution */
NumericalScalar RandomMixture::getDispersionIndicator() const
{
  if (!isAlreadyComputedDispersionIndicator_) computeDispersionIndicator();
  return dispersionIndicator_;
}

/* Compute a dispersion indicator for a 1D distribution */
void RandomMixture::computeDispersionIndicator() const
{
  dispersionIndicator_ = 0.0;
  const UnsignedLong size(distributionCollection_.getSize());
  // Assume a quadratic additive behaviour of the dispersion indicator. It is true for the standard deviation value, and almost true for the interquartile of moderatly skewed distributions
  for(UnsignedLong i = 0; i < size; ++i) dispersionIndicator_ += pow(distributionCollection_[i].getWeight() * distributionCollection_[i].getDispersionIndicator(), 2.0);
  dispersionIndicator_ = sqrt(dispersionIndicator_);
  isAlreadyComputedDispersionIndicator_ = true;
}

/* BlockMin accessor */
void RandomMixture::setBlockMin(const UnsignedLong blockMin)
{
  blockMin_ = blockMin;
}

UnsignedLong RandomMixture::getBlockMin() const
{
  return blockMin_;
}

/* BlockMax accessor */
void RandomMixture::setBlockMax(const UnsignedLong blockMax)
{
  blockMax_ = blockMax;
}

UnsignedLong RandomMixture::getBlockMax() const
{
  return blockMax_;
}

/* MaxSize accessor */
void RandomMixture::setMaxSize(const UnsignedLong maxSize)
{
  maxSize_ = maxSize;
  characteristicValuesCache_.resize(maxSize);
}

UnsignedLong RandomMixture::getMaxSize() const
{
  return maxSize_;
}

/* Alpha accessor */
void RandomMixture::setAlpha(const NumericalScalar alpha)
{
  if (alpha <= 0.0) throw InvalidArgumentException(HERE) << "Error: the alpha parameter must be strictly positive";
  alpha_ = alpha;
  computeBeta();
  // Must be after computeBeta
  computeRange();
  computeReferenceBandwidth();
}

NumericalScalar RandomMixture::getAlpha() const
{
  return alpha_;
}

/* Beta accessor */
void RandomMixture::computeBeta()
{
  NumericalScalar step(getDispersionIndicator());
  const NumericalScalar position(getPositionIndicator());
  NumericalScalar x(position + alpha_ * step);
  const NumericalScalar stepFactor(1.1);
  const NumericalScalar stepMax(100.0 * step);
  while ((abs(computeCharacteristicFunction(x)) > pdfPrecision_) && (step < stepMax))
    {
      x += step;
      step *= stepFactor;
    }
  beta_ = 2.0 * (x - position) / getDispersionIndicator();
}

void RandomMixture::setBeta(const NumericalScalar beta)
{
  beta_ = beta;
  computeReferenceBandwidth();
}

NumericalScalar RandomMixture::getBeta() const
{
  return beta_;
}

/* Reference bandwidth accessor */
void RandomMixture::setReferenceBandwidth(const NumericalScalar bandwidth)
{
  referenceBandwidth_ = bandwidth;
  // Reset the cached values
  storedSize_ = 0;
  characteristicValuesCache_ = NumericalComplexPersistentCollection(0);
}

NumericalScalar RandomMixture::getReferenceBandwidth() const
{
  return referenceBandwidth_;
}

/* PDF precision accessor. For other distributions, it is a read-only attribute. */
void RandomMixture::setPDFPrecision(const NumericalScalar pdfPrecision)
{
  pdfPrecision_ = pdfPrecision;
}

/* CDF precision accessor. For other distributions, it is a read-only attribute. */
void RandomMixture::setCDFPrecision(const NumericalScalar cdfPrecision)
{
  cdfPrecision_ = cdfPrecision;
}

/* Compute the reference bandwidth. It is defined as the largest bandwidth
   that allow a precise computation of the PDF over the range
   [positionIndicator_ +/- beta * dispersionIndicator_] */
void RandomMixture::computeReferenceBandwidth()
{
  // For small mixtures, simply use the numerical range
  if (distributionCollection_.getSize() <= ResourceMap::GetAsUnsignedLong( "RandomMixture-SmallSize" ))
    {
      referenceBandwidth_ = 2.0 * M_PI / (getRange().getUpperBound()[0] - getRange().getLowerBound()[0]);
      // Shrink a little bit the bandwidth if the range is finite
      if (getRange().getFiniteLowerBound()[0] && getRange().getFiniteUpperBound()[0]) referenceBandwidth_ *= 0.5;
    }
  // Else use a kind of Normal approximation
  else referenceBandwidth_ = 2.0 * M_PI / ((beta_ + 4.0 * alpha_) * getDispersionIndicator());
  // Reset the cached values
  storedSize_ = 0;
  characteristicValuesCache_ = NumericalComplexPersistentCollection(0);
}

/* Compute the equivalent normal distribution, i.e. with the same mean and
   the same standard deviation */
void RandomMixture::computeEquivalentNormal()
{
  if (distributionCollection_.getSize() > 0) equivalentNormal_ = Normal(getMean()[0], getStandardDeviation()[0]);
  else equivalentNormal_ = Normal();
}

/* Compute the left-hand sum in Poisson's summation formula for the equivalent normal */
NumericalScalar RandomMixture::computeEquivalentNormalPDFSum(const NumericalScalar x) const
{
  NumericalScalar value(equivalentNormal_.computePDF(x));
  UnsignedLong i(0);
  NumericalScalar delta(0.0);
  do
    {
      ++i;
      const NumericalScalar step(2.0 * M_PI * i / referenceBandwidth_);
      delta = equivalentNormal_.computePDF(x + step) + equivalentNormal_.computePDF(x - step);
      value += delta;
    }
  while (delta > pdfEpsilon_ * value);
  return value;
}

/* Compute the left-hand sum in Poisson's summation formula for the equivalent normal */
NumericalScalar RandomMixture::computeEquivalentNormalCDFSum(const NumericalScalar s,
                                                             const NumericalScalar t) const
{
  NumericalScalar value(equivalentNormal_.computeProbability(Interval(s, t)));
  UnsignedLong i(0);
  NumericalScalar delta(0.0);
  do
    {
      ++i;
      const NumericalScalar step(2.0 * M_PI * i / referenceBandwidth_);
      delta = (equivalentNormal_.computeCDF(t + step) - equivalentNormal_.computeCDF(s + step)) + (equivalentNormal_.computeCDF(t - step) - equivalentNormal_.computeCDF(s - step));
      value += delta;
    }
  while (delta > cdfEpsilon_ * value);
  return value;
}

/* Adjust a given bandwidth with respect to a reference bandwidth,
   in order to be an integral divisor or multiple of the reference bandwidth */
NumericalScalar RandomMixture::adjustBandwidth(const NumericalScalar bandwidth) const
{
  // If the given bandwidth is a strict divisor of the reference bandwidth
  if (bandwidth < referenceBandwidth_) return referenceBandwidth_ / floor(referenceBandwidth_ / bandwidth);
  // If the given bandwidth is a strict multiple of the reference bandwidth
  if (bandwidth > referenceBandwidth_) return floor(bandwidth / referenceBandwidth_) * referenceBandwidth_;
  // else the given bandwidth is equal to the reference bandwidth
  return referenceBandwidth_;
}

struct Pair
{
  NumericalScalar norm_;
  Distribution distribution_;
  Pair(): norm_(), distribution_() {}
  Pair(const NumericalScalar norm, const Distribution distribution): norm_(norm), distribution_(distribution) {}
  ~Pair() {}
  Bool operator < (const Pair & other) const
  {
    return norm_ < other.norm_;
  }
};

typedef Collection<Pair> PairCollection;

struct KolmogorovProjection
{
  /** Constructor from a distribution and a data set */
  KolmogorovProjection(const NumericalSample & data,
                       const DistributionFactory & factory):
    data_(data),
    factory_(factory) {};

  /** Compute the Kolmogorov distance based on the given data, for a given parameter set */
  NumericalPoint computeNorm(const NumericalPoint & parameters) const
  {
    NumericalScalar norm(0.0);
    try
      {
        const Distribution candidate(factory_.build(NumericalPointCollection(1, parameters)));
        for (UnsignedLong i = 0; i < data_.getSize(); ++i)
          norm += pow(candidate.computeCDF(data_[i][0]) - data_[i][1], 2);
        return NumericalPoint(1, norm);
      }
    catch(...)
      {
        return NumericalPoint(1, SpecFunc::MaxNumericalScalar);
      }
  }

  /** factory accessor */
  void setDistributionFactory(const DistributionFactory & factory)
  {
    factory_ = factory;
  }

  NumericalSample data_;
  DistributionFactory factory_;
};

/** Project a RandomMixture over a Collection of DistributionFactory by using a regular sampling and Kolmogorov distance. */
DistributionCollection RandomMixture::project(const DistributionFactoryCollection & factoryCollection,
                                              NumericalPoint & kolmogorovNorm,
                                              const UnsignedLong size) const
{
  const UnsignedLong factorySize(factoryCollection.getSize());
  PairCollection result(factorySize);
  const NumericalScalar mean(getMean()[0]);
  const NumericalScalar sigma(getStandardDeviation()[0]);
  // Sample the CDF uniformly over [mean +/- alpha * sigma
  const NumericalScalar xMin(mean - alpha_ * sigma);
  const NumericalScalar xMax(mean + alpha_ * sigma);
  const NumericalSample data(computeCDF(xMin, xMax, size));
  // Create a NumericalMathFunction to use the TNC algorithm for finding the best candidate for projection
  Distribution bestCandidate;
  KolmogorovProjection projection(data, factoryCollection[0]);
  // Loop over the factories
  for (UnsignedLong i = 0; i < factorySize; ++i)
    {
      DistributionFactory factory(factoryCollection[i]);
      projection.setDistributionFactory(factory);
      NumericalPoint startingPoint;
      try
        {
          startingPoint = factory.build(getSample(size)).getParametersCollection()[0];
        }
      catch(...)
        {
          startingPoint = factory.build().getParametersCollection()[0];
          LOGWARN(OSS() << "Initialization of the projection for the factory " << factory.getClassName() << " failed. Using default initial parameters " << startingPoint);
        }
      Interval bounds(NumericalPoint(startingPoint.getDimension(), SpecFunc::MinNumericalScalar),
                      NumericalPoint(startingPoint.getDimension(), SpecFunc::MaxNumericalScalar),
                      Interval::BoolCollection(startingPoint.getDimension(), false),
                      Interval::BoolCollection(startingPoint.getDimension(), false));
      NumericalMathFunction objectiveFunction(bindMethod<KolmogorovProjection, NumericalPoint, NumericalPoint>(projection, &KolmogorovProjection::computeNorm, startingPoint.getDimension(), 1));
      TNC optimAlgo(TNCSpecificParameters(), objectiveFunction, bounds, startingPoint, TNC::Result::MINIMIZATION, false);
      // Use more tighter parameters than the default ones
      optimAlgo.setMaximumEvaluationsNumber(ResourceMap::GetAsUnsignedLong( "RandomMixture-ProjectionMaximumIteration" ));
      optimAlgo.setMaximumAbsoluteError(ResourceMap::GetAsNumericalScalar( "RandomMixture-ProjectionMaximumAbsoluteError" ));
      optimAlgo.setMaximumRelativeError(ResourceMap::GetAsNumericalScalar( "RandomMixture-ProjectionMaximumRelativeError" ));
      optimAlgo.setMaximumObjectiveError(ResourceMap::GetAsNumericalScalar( "RandomMixture-ProjectionMaximumObjectiveError" ));
      optimAlgo.setMaximumConstraintError(ResourceMap::GetAsNumericalScalar( "RandomMixture-ProjectionMaximumConstraintError" ));
      try
        {
          optimAlgo.run();
        }
      catch (InvalidArgumentException & ex)
        {
          LOGWARN(OSS() << "Factory " << factory.getClassName() << " failed, reason: " << ex);
        }
      // The optimization use L2 norm, we quantify it using the L-inf norm
      const Distribution bestCandidate(factory.build(NumericalPointCollection(1, optimAlgo.getResult().getOptimizer())));
      NumericalScalar kolmogorov(0.0);
      for (UnsignedLong j = 0; j < size; ++j)
        kolmogorov = std::max(kolmogorov, fabs(bestCandidate.computeCDF(data[j][0]) - data[j][1]));
      result[i] = Pair(kolmogorov, bestCandidate);
    }
  // Sort the results
  std::stable_sort(result.begin(), result.end());
  // Extract the results
  DistributionCollection distributionCollection(factorySize);
  kolmogorovNorm = NumericalPoint(factorySize);
  for (UnsignedLong i = 0; i < factorySize; ++i)
    {
      distributionCollection[i] = result[i].distribution_;
      kolmogorovNorm[i] = result[i].norm_;
    }
  return distributionCollection;
}


/* Method save() stores the object through the StorageManager */
void RandomMixture::save(Advocate & adv) const
{
  DistributionImplementation::save(adv);
  adv.saveAttribute( "distributionCollection_", distributionCollection_  );
  adv.saveAttribute( "constant_", constant_ );
  adv.saveAttribute( "positionIndicator_", positionIndicator_ );
  adv.saveAttribute( "isAlreadyComputedPositionIndicator_", isAlreadyComputedPositionIndicator_ );
  adv.saveAttribute( "dispersionIndicator_", dispersionIndicator_ );
  adv.saveAttribute( "isAlreadyComputedDispersionIndicator_", isAlreadyComputedDispersionIndicator_ );
  adv.saveAttribute( "blockMin_", blockMin_ );
  adv.saveAttribute( "blockMax_", blockMax_ );
  adv.saveAttribute( "referenceBandwidth_", referenceBandwidth_ );
  adv.saveAttribute( "maxSize_", maxSize_ );
  adv.saveAttribute( "storedSize_", storedSize_ );
  adv.saveAttribute( "characteristicValuesCache_", characteristicValuesCache_ );
  adv.saveAttribute( "alpha_", alpha_ );
  adv.saveAttribute( "beta_", beta_ );
  adv.saveAttribute( "pdfPrecision_", pdfPrecision_ );
  adv.saveAttribute( "cdfPrecision_", cdfPrecision_ );
} // save

/* Method load() reloads the object from the StorageManager */
void RandomMixture::load(Advocate & adv)
{
  DistributionImplementation::load(adv);
  adv.loadAttribute( "distributionCollection_", distributionCollection_  );
  adv.loadAttribute( "constant_", constant_ );
  adv.loadAttribute( "positionIndicator_", positionIndicator_ );
  adv.loadAttribute( "isAlreadyComputedPositionIndicator_", isAlreadyComputedPositionIndicator_ );
  adv.loadAttribute( "dispersionIndicator_", dispersionIndicator_ );
  adv.loadAttribute( "isAlreadyComputedDispersionIndicator_", isAlreadyComputedDispersionIndicator_ );
  adv.loadAttribute( "blockMin_", blockMin_ );
  adv.loadAttribute( "blockMax_", blockMax_ );
  adv.loadAttribute( "referenceBandwidth_", referenceBandwidth_ );
  adv.loadAttribute( "maxSize_", maxSize_ );
  adv.loadAttribute( "storedSize_", storedSize_ );
  adv.loadAttribute( "characteristicValuesCache_", characteristicValuesCache_ );
  adv.loadAttribute( "alpha_", alpha_ );
  adv.loadAttribute( "beta_", beta_ );
  adv.loadAttribute( "pdfPrecision_", pdfPrecision_ );
  adv.loadAttribute( "cdfPrecision_", cdfPrecision_ );
  computePositionIndicator();
  computeDispersionIndicator();
  computeBeta();
  // Must be after computeBeta
  computeRange();
  computeReferenceBandwidth();
  computeEquivalentNormal();
} // load

END_NAMESPACE_OPENTURNS
