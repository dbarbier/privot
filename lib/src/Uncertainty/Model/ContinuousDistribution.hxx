//                                               -*- C++ -*-
/**
 *  @file  ContinuousDistribution.hxx
 *  @brief Abstract top-level class for Continuous distributions
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 *  Id      ContinuousDistribution.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_CONTINUOUSDISTRIBUTION_HXX
#define OPENTURNS_CONTINUOUSDISTRIBUTION_HXX

#include "UsualDistribution.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ContinuousDistribution
 *
 * A subclass for Continuous usual distributions.
 */
class ContinuousDistribution
  : public UsualDistribution
{

  CLASSNAME;
public:


  /** Default constructor */
  explicit ContinuousDistribution(const String & name = DefaultName);


  /** Virtual constructor */
  virtual ContinuousDistribution * clone() const;

  /** Comparison operator */
  Bool operator ==(const ContinuousDistribution & other) const;

  /** Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalCDF(const NumericalScalar x, const NumericalPoint & y) const;

  /** Get the PDF of the distributionImplementation */
  using DistributionImplementation::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the distributionImplementation */
  using DistributionImplementation::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the distributionImplementation */
  using DistributionImplementation::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the probability content of an interval */
  NumericalScalar computeProbability(const Interval & interval) const;

  /** String converter */
  String __repr__() const;


  /* Methods inherited from upper classes */

  /** Tell if the distribution is continuous */
  Bool isContinuous() const;

protected:

  /** Compute the shifted moment of the distribution */
  NumericalPoint computeShiftedMoment(const UnsignedLong n,
                                      const NumericalPoint & shift) const;

  /** Compute the covariance of the distribution */
  void computeCovariance() const;

private:

}; /* class ContinuousDistribution */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_CONTINUOUSDISTRIBUTION_HXX */
