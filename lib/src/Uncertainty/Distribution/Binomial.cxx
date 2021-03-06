//                                               -*- C++ -*-
/**
 *  @file  Binomial.cxx
 *  @brief The Binomial distribution
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
#include "Binomial.hxx"
#include "SpecFunc.hxx"
#include "DistFunc.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Binomial);

static Factory<Binomial> RegisteredFactory("Binomial");

/* Default constructor */
Binomial::Binomial()
  : DiscreteDistribution("Binomial"),
    n_(1),
    p_(0.5)
{
  // We set the dimension of the Binomial distribution
  setDimension( 1 );
  computeRange();
}

/* Parameters constructor */
Binomial::Binomial(const UnsignedLong n,
                   const NumericalScalar p)
  : DiscreteDistribution("Binomial"),
    n_(n),
    p_(p)
{
  // We set the dimension of the Binomial distribution
  setDimension( 1 );
  computeRange();
}

/* Comparison operator */
Bool Binomial::operator ==(const Binomial & other) const
{
  if (this == &other) return true;
  return (n_ == other.n_) && (p_ == other.p_);
}

/* String converter */
String Binomial::__repr__() const
{
  OSS oss;
  oss << "class=" << Binomial::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " n=" << n_
      << " p=" << p_;
  return oss;
}

String Binomial::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(n = " << n_ << ", p = " << p_ << ")";
  return oss;
}

/* Virtual constructor */
Binomial * Binomial::clone() const
{
  return new Binomial(*this);
}

/* Get one realization of the distribution */
NumericalPoint Binomial::getRealization() const
{
  return NumericalPoint(1, DistFunc::rBinomial(n_, p_));
}


/* Get the PDF of the distribution */
NumericalScalar Binomial::computePDF(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);
  const NumericalScalar supportEpsilon(ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"));
  if ((k < -supportEpsilon) || (fabs(k - round(k)) > supportEpsilon) || (k > n_ + supportEpsilon)) return 0.0;
  return exp(SpecFunc::LnGamma(n_ + 1.0) - SpecFunc::LnGamma(n_ - k + 1.0) - SpecFunc::LnGamma(k + 1.0) + k * log(p_) + (n_ - k) * log1p(-p_));
}


/* Get the CDF of the distribution */
NumericalScalar Binomial::computeCDF(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);
  const NumericalScalar supportEpsilon(ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"));
  if (k < -supportEpsilon) return 0.0;
  if (k > n_ + supportEpsilon) return 1.0;
  // Complementary relation for incomplete regularized Beta function: I(a, b, x) = 1 - I(b, a, 1-x)
  NumericalScalar value = SpecFunc::BetaRatioInc(n_ - floor(k), floor(k) + 1, 1.0 - p_);
  return value;
}

NumericalScalar Binomial::computeComplementaryCDF(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);
  const NumericalScalar supportEpsilon(ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"));
  if (k < -supportEpsilon) return 1.0;
  if (k > n_ + supportEpsilon) return 0.0;
  // Complementary relation for incomplete regularized Beta function: I(a, b, x) = 1 - I(b, a, 1-x)
  NumericalScalar value = SpecFunc::BetaRatioInc(floor(k) + 1, n_ - floor(k), p_);
  return value;
}

/* Get the PDF gradient of the distribution */
NumericalPoint Binomial::computePDFGradient(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);
  NumericalPoint pdfGradient(1, 0.0);
  const NumericalScalar supportEpsilon(ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"));
  if ((k < -supportEpsilon) || (fabs(k - round(k)) > supportEpsilon)) return pdfGradient;
  throw NotYetImplementedException(HERE);
}


/* Get the CDF gradient of the distribution */
NumericalPoint Binomial::computeCDFGradient(const NumericalPoint & point) const
{
  const NumericalScalar k(point[0]);
  const NumericalScalar supportEpsilon(ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"));
  if (k < -supportEpsilon) return NumericalPoint(1, 0.0);
  throw NotYetImplementedException(HERE);
}

/* Compute the mean of the distribution */
void Binomial::computeMean() const
{
  mean_ = NumericalPoint(1, n_ * p_);
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Binomial::getStandardDeviation() const
{
  return NumericalPoint(1, sqrt(n_ * p_ * (1.0 - p_)));
}

/* Get the skewness of the distribution */
NumericalPoint Binomial::getSkewness() const
{
  if ((p_ == 0.0) || (p_ == 1.0)) throw NotDefinedException(HERE) << "Error: the skewness is not defined for the Binomial distribution when p is zero or one.";
  return NumericalPoint(1, (1.0 - 2.0 * p_) / sqrt(n_ * p_ * (1.0 - p_)));
}

/* Get the kurtosis of the distribution */
NumericalPoint Binomial::getKurtosis() const
{
  if ((p_ == 0.0) || (p_ == 1.0)) throw NotDefinedException(HERE) << "Error: the kurtosis is not defined for the Binomial distribution when p is zero or one.";
  return NumericalPoint(1, 3.0 - 6.0 / n_ + 1.0 / (n_ * p_ * (1.0 - p_)));
}

/* Compute the covariance of the distribution */
void Binomial::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  covariance_(0, 0) = n_ * p_ * (1.0 - p_);
  isAlreadyComputedCovariance_ = true;
}

/* Get the support of a discrete distribution that intersect a given interval */
NumericalSample Binomial::getSupport(const Interval & interval) const
{
  if (interval.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given interval has a dimension that does not match the distribution dimension.";
  const UnsignedLong kMin(static_cast< UnsignedLong > (std::max(ceil(interval.getLowerBound()[0]), 0.0)));
  const UnsignedLong kMax(static_cast< UnsignedLong > (std::min(floor(interval.getUpperBound()[0]), NumericalScalar(n_))));
  NumericalSample result(0, 1);
  for (UnsignedLong k = kMin; k <= kMax; ++k) result.add(NumericalPoint(1, k));
  return result;
}

/* Parameters value and description accessor */
Binomial::NumericalPointWithDescriptionCollection Binomial::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(2);
  Description description(point.getDimension());
  point[0] = n_;
  description[0] = "n";
  point[1] = p_;
  description[1] = "p";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void Binomial::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = Binomial(static_cast<UnsignedLong>(parametersCollection[0][0]), parametersCollection[0][1]);
}

/* P accessor */
void Binomial::setP(const NumericalScalar p)
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
NumericalScalar Binomial::getP() const
{
  return p_;
}

/* N accessor */
void Binomial::setN(const UnsignedLong n)
{
  if (n != n_)
    {
      n_ = n;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

/* N accessor */
UnsignedLong Binomial::getN() const
{
  return n_;
}

/* Compute the numerical range of the distribution given the parameters values */
void Binomial::computeRange()
{
  // Degenerate cases
  if (p_ == 0.0) setRange(Interval(0.0, 0.0));
  if (p_ == 1.0) setRange(Interval(n_, n_));
  // Usual case
  setRange(Interval(0.0, n_));
}

/* Get the quantile of the distribution */
NumericalScalar Binomial::computeScalarQuantile(const NumericalScalar prob,
                                                const Bool tail,
                                                const NumericalScalar precision) const
{
  LOGDEBUG(OSS() << "in Binomial::computeScalarQuantile, prob=" << prob << ", tail=" << (tail ? "true" : "false"));
  if (prob <= 0.0) return (tail ? n_ : 0.0);
  if (prob >= 1.0) return (tail ? 0.0 : n_);
  // Initialization by the Cornish-Fisher expansion
  NumericalScalar qNorm(DistFunc::qNormal(prob, tail));
  NumericalScalar gamma1(getSkewness()[0]);
  NumericalScalar gamma2(getKurtosis()[0] - 3.0);
  NumericalScalar quantile(round(getMean()[0] + getStandardDeviation()[0] * (qNorm + (qNorm * qNorm - 1.0) * gamma1 / 6.0 + qNorm * (qNorm * qNorm - 3.0) * gamma2 / 24.0 - qNorm * (2.0 * qNorm * qNorm - 5.0) * gamma1 * gamma1 / 36.0)));
  if (quantile < 0.0) quantile = 0.0;
  if (quantile > n_) quantile = n_;
  NumericalScalar cdf(computeCDF(quantile, tail));
  LOGDEBUG(OSS() << "in Binomial::computeScalarQuantile, Cornish-Fisher estimate=" << quantile << ", cdf=" << cdf);
  NumericalScalar oldCDF(cdf);
  const NumericalScalar step(tail ? -1.0 : 1.0);
  while (cdf >= prob)
    {
      quantile -= step;
      oldCDF = cdf;
      cdf = computeCDF(quantile, tail);
      LOGDEBUG(OSS() << "in Binomial::computeScalarQuantile, backward search, quantile=" << quantile << ", cdf=" << cdf);
    }
  if (cdf < oldCDF)
    {
      quantile += step;
      LOGDEBUG(OSS() << "in Binomial::computeScalarQuantile, final quantile=" << quantile);
      return quantile;
    }
  while (cdf < prob)
    {
      quantile += step;
      oldCDF = cdf;
      cdf = computeCDF(quantile, tail);
      LOGDEBUG(OSS() << "in Binomial::computeScalarQuantile, forward search, quantile=" << quantile << ", cdf=" << cdf);
    }
  LOGDEBUG(OSS() << "in Binomial::computeScalarQuantile, final quantile=" << quantile);
  return quantile;
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex Binomial::computeCharacteristicFunction(const NumericalScalar x) const
{
  const NumericalComplex value(1.0 - p_ + p_ * exp(NumericalComplex(0.0, x)));
  return pow(value, n_);
}

NumericalComplex Binomial::computeLogCharacteristicFunction(const NumericalScalar x) const
{
  const NumericalComplex value(1.0 - p_ + p_ * exp(NumericalComplex(0.0, x)));
  return NumericalScalar(n_) * log(value);
}

/* Get the generating function of the distribution, i.e. psi(z) = E(z^X) */
NumericalComplex Binomial::computeGeneratingFunction(const NumericalComplex & z) const
{
  const NumericalComplex value(1.0 - p_ + p_ * z);
  return pow(value, n_);
}

NumericalComplex Binomial::computeLogGeneratingFunction(const NumericalComplex & z) const
{
  const NumericalComplex value(1.0 - p_ + p_ * z);
  return NumericalComplex(n_) * log(value);
}

/* Method save() stores the object through the StorageManager */
void Binomial::save(Advocate & adv) const
{
  DiscreteDistribution::save(adv);
  adv.saveAttribute( "n_", n_ );
  adv.saveAttribute( "p_", p_ );
}

/* Method load() reloads the object from the StorageManager */
void Binomial::load(Advocate & adv)
{
  DiscreteDistribution::load(adv);
  adv.loadAttribute( "n_", n_ );
  adv.loadAttribute( "p_", p_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
