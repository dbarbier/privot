//                                               -*- C++ -*-
/**
 *  @file  ZipfMandelbrot.hxx
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
 *  Id      ZipfMandelbrot.hxx 1581 2010-07-12 13:45:44Z dutka
 */
#ifndef OPENTURNS_ZIPFMANDELBROT_HXX
#define OPENTURNS_ZIPFMANDELBROT_HXX

#include "OTprivate.hxx"
#include "DiscreteDistribution.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ZipfMandelbrot
 *
 * The ZipfMandelbrot distribution.
 */
class ZipfMandelbrot
  : public DiscreteDistribution
{
  CLASSNAME;
public:

  typedef Collection<NumericalScalar> NumericalScalarCollection;

  /** Default constructor */
  ZipfMandelbrot();

  /** Parameters constructor */
  ZipfMandelbrot(const UnsignedLong n,
                 const NumericalScalar q,
                 const NumericalScalar s);

  /** Comparison operator */
  Bool operator ==(const ZipfMandelbrot & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;



  /* Interface inherited from Distribution */

  /** Virtual constructor */
  virtual ZipfMandelbrot * clone() const;

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get the PDF of the distribution */
  using DiscreteDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution */
  using DiscreteDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /* Interface specific to ZipfMandelbrot */

  /** Q accessor */
  void setQ(const NumericalScalar q);
  NumericalScalar getQ() const;

  /** S accessor */
  void setS(const NumericalScalar s);
  NumericalScalar getS() const;

  /** N accessor */
  void setN(const UnsignedLong n);
  UnsignedLong getN() const;

  /** Get the support of a discrete distribution that intersect a given interval */
  using DistributionImplementation::getSupport;
  NumericalSample getSupport(const Interval & interval) const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using DiscreteDistribution::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  /* Compute generalized harmonic numbers */
  NumericalScalar getHarmonicNumbers(UnsignedLong const) const;
  void computeHarmonicNumbers() const;

  mutable Bool isAlreadyComputedHarmonicNumbers_;
  mutable NumericalScalarCollection harmonicNumbers_;


private:

  /** Get the quantile of the distribution */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** Compute the mean of the distribution */
  void computeMean() const;

  /** Compute the covariance of the distribution */
  void computeCovariance() const;

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** The maximum value of the ZipfMandelbrot distribution */
  UnsignedLong n_;

  /** The q probability parameter of the ZipfMandelbrot distribution */
  NumericalScalar q_;

  /** The s probability parameter of the ZipfMandelbrot distribution */
  NumericalScalar s_;

}; /* class ZipfMandelbrot */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ZIPFMANDELBROT_HXX */
