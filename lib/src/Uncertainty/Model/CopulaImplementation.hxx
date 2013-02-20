//                                               -*- C++ -*-
/**
 *  @file  CopulaImplementation.hxx
 *  @brief Abstract top-level class for all copula
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
 *  Id      CopulaImplementation.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_COPULAIMPLEMENTATION_HXX
#define OPENTURNS_COPULAIMPLEMENTATION_HXX

#include "OTprivate.hxx"
#include "ContinuousDistribution.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class CopulaImplementation
 *
 * A subclass for Continuous usual distributions.
 */
class CopulaImplementation
  : public ContinuousDistribution
{
  CLASSNAME;
public:

  /** Default constructor */
  explicit CopulaImplementation(const String & name = DefaultName);

  /** Virtual constructor */
  virtual CopulaImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const CopulaImplementation & other) const;

  /** Get the mean of the distribution */
  NumericalPoint getMean() const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Get the i-th marginal distribution */
  using ContinuousDistribution::getMarginal;
#ifndef SWIG
  Implementation getMarginal(const UnsignedLong i) const;
#endif

  /** Get the copula of a distribution */
  Implementation getCopula() const;

  /** String converter */
  String __repr__() const;


  /* Methods inherited from upper classes */

  /** Generic implementation of the quantile computation for continuous distributions */
  NumericalPoint computeQuantile(const NumericalScalar prob,
                                 const Bool tail = false) const;

  /** Draw the PDF of the copula when its dimension is 2 */
  using ContinuousDistribution::drawPDF;
  Graph drawPDF(const Indices & pointNumber) const;

  /** Draw the CDF of the copula when its dimension is 2 */
  using ContinuousDistribution::drawCDF;
  Graph drawCDF(const Indices & pointNumber) const;

protected:

  /** Compute the covariance of the distribution */
  void computeCovariance() const;


private:

  /** Compute the mathematical and numerical range of the distribution.
      Its mathematical range is the smallest closed interval outside
      of which the PDF is zero, and the numerical range is the interval
      outside of which the PDF is rounded to zero in double precision */
  void computeRange();

  // Structure used to implement the computeQuantile() method efficiently
  struct QuantileWrapper
  {
    QuantileWrapper(const DistributionImplementation * p_distribution)
      : p_distribution_(p_distribution)
      , dimension_(p_distribution->getDimension())
    {
      // Nothing to do
    }

    NumericalPoint computeDiagonal(const NumericalPoint & u) const
    {
      const NumericalPoint point(dimension_, u[0]);
      const NumericalScalar cdf(p_distribution_->computeCDF(point));
      const NumericalPoint value(1, cdf);
      return value;
    }

    const DistributionImplementation * p_distribution_;
    const UnsignedLong dimension_;
  }; // struct QuantileWrapper


}; /* class CopulaImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COPULAIMPLEMENTATION_HXX */
