//                                               -*- C++ -*-
/**
 *  @file  ProcessSample.hxx
 *  @brief ProcessSample class
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
 *  Id      ProcessSample.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_PROCESSSAMPLE_HXX
#define OPENTURNS_PROCESSSAMPLE_HXX

#include "PersistentObject.hxx"
#include "NumericalPoint.hxx"
//#include "TimeGrid.hxx"
#include "TimeSeries.hxx"
#include "NumericalSample.hxx"
#include "Collection.hxx"
#include "PersistentCollection.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class ProcessSample
 *
 * An interface for time series
 */
class ProcessSample
  : public PersistentObject
{
  CLASSNAME;

public:

  /** Some typedefs to ease reading */
  typedef Collection<TimeSeries>               TimeSeriesCollection;
  typedef PersistentCollection<TimeSeries>     TimeSeriesPersistentCollection;
  typedef TimeSeriesPersistentCollection             InternalType;

  /** Default constructor */
  ProcessSample();

  /** Constructors */
  ProcessSample(const UnsignedLong size,
                const TimeSeries & timeSeries);
  ProcessSample(const RegularGrid & timeGrid,
                const UnsignedLong size,
                const UnsignedLong dimension);
  ProcessSample(const TimeSeriesCollection & collection);

  /** Partial copy constructor */
  void add(const TimeSeries & timeSeries);

#ifndef SWIG

  /** Operators accessors */
  TimeSeries & operator [] (const UnsignedLong i);
  const TimeSeries & operator [] (const UnsignedLong i) const;

#endif

  /** Method __getitem__() for Python */
  TimeSeries __getitem__ (const UnsignedLong i) const;

  /** Method __setitem__() is for Python */
  void __setitem__(const UnsignedLong i,
                   const TimeSeries & timeSeries);

  /** Virtual constructor */
  ProcessSample * clone() const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Time grid accessors */
  RegularGrid getTimeGrid() const;

  /** Size accessor */
  UnsignedLong getSize() const;

  /** Dimension accessor */
  UnsignedLong getDimension() const;

  /** Mean accessor */
  TimeSeries computeMean() const;

  /** Temporal mean accessor */
  NumericalSample computeTemporalMean() const;

  /**  Method computeQuantilePerComponent() gives the quantile per component of the sample */
  TimeSeries computeQuantilePerComponent(const NumericalScalar prob) const;

  /** Draw a marginal of the timeSerie */
  Graph drawMarginal(const UnsignedLong index = 0) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /* To be optimized by :
   * keeping 1 timeGrid
   * concatenate Numerical data into a NumericalSample*/

  /** Dimension of the process sample, ie the dimension of the underlying time series */
  UnsignedLong dimension_;

  /** TimeGrid on which the ProcessSample focuses */
  RegularGrid timeGrid_;

  /** NumericalSample which stocks the data */
  TimeSeriesPersistentCollection data_;

}; /* class ProcessSample */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PROCESSSAMPLE_HXX */
