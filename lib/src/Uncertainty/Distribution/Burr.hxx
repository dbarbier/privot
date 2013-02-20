//                                               -*- C++ -*-
/**
 *  @file  Burr.hxx
 *  @brief The Burr distribution
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
 *  @author keller
 *  @date   2011-22-03 11:30:00 +0100 (mer. 23 mars 2011)
 *  Id      Burr.hxx 1473 2010-02-04 15:44:49Z keller
 */
#ifndef OPENTURNS_BURR_HXX
#define OPENTURNS_BURR_HXX

#include "OTprivate.hxx"
#include "NonEllipticalDistribution.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Burr
 *
 * The Burr distribution.
 */
class Burr
  : public NonEllipticalDistribution
{
  CLASSNAME;
public:

  /** Default constructor */
  Burr();

  /** Parameters constructor */
  Burr(const NumericalScalar c,
       const NumericalScalar k);


  /** Comparison operator */
  Bool operator ==(const Burr & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;


  /* Interface inherited from Distribution */
  /** Virtual constructor */
  virtual Burr * clone() const;

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get the DDF of the distribution */
  using NonEllipticalDistribution::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the distribution */
  using NonEllipticalDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;
  using NonEllipticalDistribution::computeLogPDF;
  NumericalScalar computeLogPDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution */
  using NonEllipticalDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the PDFGradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the distribution */
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

  /* Interface specific to Burr */

  /** C accessor */
  void setC(const NumericalScalar c);
  NumericalScalar getC() const;

  /** K accessor */
  void setK(const NumericalScalar k);
  NumericalScalar getK() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:


private:

  /** Get the quantile of the distribution */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** Compute the mean of the distribution */
  void computeMean() const;

  /** Get the covariance of the distribution */
  void computeCovariance() const;

  /** The c of the Burr distribution */
  NumericalScalar c_;

  /** The k of the Burr distribution */
  NumericalScalar k_;

}; /* class Burr */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BURR_HXX */
