//                                               -*- C++ -*-
/**
 *  @file  Histogram.hxx
 *  @brief The Histogram distribution
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 *  Id      Histogram.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_HISTOGRAM_HXX
#define OPENTURNS_HISTOGRAM_HXX

#include "OTprivate.hxx"
#include "NonEllipticalDistribution.hxx"
#include "Collection.hxx"
#include "HistogramPair.hxx"
#include "PersistentCollection.hxx"
#include "PersistentObject.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class Histogram
 *
 * The Histogram distribution.
 */
class Histogram
  : public NonEllipticalDistribution
{
  CLASSNAME;

public:

  typedef Collection<HistogramPair>                HistogramPairCollection;
  typedef PersistentCollection<HistogramPair>      HistogramPairPersistentCollection;

  /** Default constructor */
  Histogram();

  /** Parameters constructor */
  Histogram(const NumericalScalar first,
            const HistogramPairCollection & collection);


  /** Comparison operator */
  Bool operator ==(const Histogram & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;



  /* Interface inherited from Distribution */

  /** Virtual constructor */
  virtual Histogram * clone() const;

  /** Get one realization of the Histogram distribution */
  NumericalPoint getRealization() const;

  /** Get the DDF of the Histogram distribution */
  using NonEllipticalDistribution::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the Histogram distribution */
  using NonEllipticalDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the Histogram distribution */
  using NonEllipticalDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the PDFGradient of the Histogram distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the Histogram distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;

  /* Interface specific to Histogram */

  /** First point accessor */
  void setFirst(const NumericalScalar first);
  NumericalScalar getFirst() const;

  /** Collection accessor */
  void setPairCollection(const HistogramPairCollection & collection);
  HistogramPairCollection getPairCollection() const;

  /** Draw the PDF of the Histogram using a specific presentation */
  //        using NonEllipticalDistribution::drawPDF;
  virtual Graph drawPDF() const;

  /** Draw the PDF of the Histogram using a specific presentation */
  virtual Graph drawPDF(const NumericalScalar xMin,
                        const NumericalScalar xMax,
                        const UnsignedLong pointNumber = ResourceMap::GetAsUnsignedLong("DistributionImplementation-DefaultPointNumber")) const;

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

  /** Get the quantile of the Histogram distribution */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** The first point of the collection */
  NumericalScalar first_;

  /** The collection of pair describing the distribution */
  HistogramPairPersistentCollection collection_;

  /** The cumulated bin widths */
  NumericalPoint cumulatedWidth_;

  /** The cumulated bin surface */
  NumericalPoint cumulatedSurface_;

  /** The total surface */
  NumericalScalar surface_;

}; /* class Histogram */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_HISTOGRAM_HXX */
