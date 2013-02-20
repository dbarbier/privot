//                                               -*- C++ -*-
/**
 *  @file  TracyWidomGOE.hxx
 *  @brief The TracyWidomGOE distribution
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
 *  Id      TracyWidomGOE.hxx 1581 2010-07-12 13:45:44Z dutka
 */
#ifndef OPENTURNS_TRACYWIDOMGOE_HXX
#define OPENTURNS_TRACYWIDOMGOE_HXX

#include "NonEllipticalDistribution.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class TracyWidomGOE
 *
 * The TracyWidomGOE distribution.
 */
class TracyWidomGOE
  : public NonEllipticalDistribution
{
  CLASSNAME;
public:

  /** Default constructor */
  TracyWidomGOE();

  /** Comparison operator */
  Bool operator ==(const TracyWidomGOE & other) const;

  /** String converter */
  String __repr__() const;



  /* Interface inherited from Distribution */

  /** Virtual constructor */
  TracyWidomGOE * clone() const;

  /** Get the DDF of the distribution */
  using NonEllipticalDistribution::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the distribution */
  using NonEllipticalDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution */
  using NonEllipticalDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the PDFGradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the roughness, i.e. the L2-norm of the PDF */
  NumericalScalar getRoughness() const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;

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

  /** Get the quantile of the distribution */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

}; /* class TracyWidomGOE */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_TRACYWIDOMGOE_HXX */
