//                                               -*- C++ -*-
/**
 *  @file  LogUniform.hxx
 *  @brief The LogUniform distribution
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
 *  @date   2011-04-11 12:32:27 +0200 (lun. 11 avril 2011)
 *  Id      LogUniform.hxx 1866 2011-04-11 10:32:27Z schueller
 */
#ifndef OPENTURNS_LOGUNIFORM_HXX
#define OPENTURNS_LOGUNIFORM_HXX

#include "NonEllipticalDistribution.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class LogUniform
 *
 * The LogUniform distribution.
 */
class LogUniform
  : public NonEllipticalDistribution
{
  CLASSNAME;
public:

  /** Default constructor */
  LogUniform();

  /** Parameters constructor */
  LogUniform(const NumericalScalar aLog,
             const NumericalScalar bLog);


  /** Comparison operator */
  Bool operator ==(const LogUniform & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;



  /* Interface inherited from Distribution */

  /** Virtual constructor */
  LogUniform * clone() const;

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get the DDF of the distribution, i.e. the gradient of its PDF w.r.t. point */
  using NonEllipticalDistribution::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the distribution, i.e. P(point < X < point+dx) = PDF(point)dx + o(dx) */
  using NonEllipticalDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution, i.e. P(X <= point) = CDF(point) */
  using NonEllipticalDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;
  using NonEllipticalDistribution::computeComplementaryCDF;
  NumericalScalar computeComplementaryCDF(const NumericalPoint & point) const;

  /** Get the gradient of the PDF w.r.t the parameters of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the gradient of the CDF w.r.t the parameters of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Get the raw moments of the standardized distribution */
  NumericalPoint getStandardMoment(const UnsignedLong n) const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using NonEllipticalDistribution::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /* Interface specific to LogUniform */

  /** ALog accessor */
  void setALog(const NumericalScalar aLog);
  NumericalScalar getALog() const;

  /** BLog accessor */
  void setBLog(const NumericalScalar bLog);
  NumericalScalar getBLog() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


protected:

private:

  /** Compute the mean of the distribution */
  void computeMean() const;

  /** Compute the covariance of the distribution */
  void computeCovariance() const;

  /** Get the quantile of the distribution, i.e the value Xp such that P(X <= Xp) = prob */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** The main parameter set of the distribution */
  /** Lower bound on the logarithmic scale */
  NumericalScalar aLog_;
  /** Upper bound on the logarithmic scale */
  NumericalScalar bLog_;

  /** Derivative parameters */
  /** Lower bound on the natural scale */
  NumericalScalar a_;
  /** Upper bound on the natural scale */
  NumericalScalar b_;

}; /* class LogUniform */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LOGUNIFORM_HXX */
