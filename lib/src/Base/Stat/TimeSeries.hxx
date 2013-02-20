//                                               -*- C++ -*-
/**
 *  @file  TimeSeries.hxx
 *  @brief The class TimeSeries implements samples indexed by time
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
 *  Id      TimeSeries.hxx 2608 2012-07-16 13:59:45Z schueller
 */
#ifndef OPENTURNS_TIMESERIES_HXX
#define OPENTURNS_TIMESERIES_HXX

#include <iostream>              // for std::ostream
#include "TimeSeriesImplementation.hxx"
#include "TypedInterfaceObject.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class TimeSeries
 */

class TimeSeries
  : public TypedInterfaceObject<TimeSeriesImplementation>
{
  CLASSNAME;

public:

  /* Some typedefs for easy reading */

public:

  /**
   * Default constructor
   * Build a TimeSeries of 1 dimension and with size equal to 0
   * and default time grid.
   */
  TimeSeries();

  /** Constructor with size and dimension */
  TimeSeries(const UnsignedLong size,
             const UnsignedLong dim);

  /** Constructor from a TimeGrid and a dimension */
  TimeSeries(const RegularGrid & tg,
             const UnsignedLong dim);
  /** Constructor from a TimeGrid and a sample */
  TimeSeries(const RegularGrid & tg,
             const NumericalSample & sample);

  /** Constructor from a NumericalPoint (all elements are equal to the NumericalPoint) */
  // TimeSeries(UnsignedLong size,
  //         const NumericalPoint & point);


  /** Constructor from implementation */
  TimeSeries(const TimeSeriesImplementation & implementation);

private:

  /** Constructor from implementation */
  TimeSeries(const Implementation & implementation);

public:

  /** Comparison operator */
  Bool operator ==(const TimeSeries & other) const;

#ifndef SWIG
  TSI_point operator [] (const UnsignedLong index);
  TSI_const_point operator [] (const UnsignedLong index) const;
  TSI_point at (const UnsignedLong index);
  TSI_const_point at (const UnsignedLong index) const;
  NumericalScalar & operator () (const UnsignedLong i,
                                 const UnsignedLong j);
  const NumericalScalar & operator () (const UnsignedLong i,
                                       const UnsignedLong j) const;
  NumericalScalar & at (const UnsignedLong i,
                        const UnsignedLong j);
  const NumericalScalar & at (const UnsignedLong i,
                              const UnsignedLong j) const;

  void erase(TimeSeriesImplementation::iterator first,
             TimeSeriesImplementation::iterator last);

  /** Accessor to values */
  TSI_point getValueAtIndex(const UnsignedLong index);
  TSI_const_point getValueAtIndex(const UnsignedLong index) const;
  void setValueAtIndex(const UnsignedLong index,
                       const NumericalPoint & val);

  TSI_point getValueAtNearestTime(const NumericalScalar timestamp);
  TSI_const_point getValueAtNearestTime(const NumericalScalar timestamp) const;
  void setValueAtNearestTime(const NumericalScalar timestamp,
                             const NumericalPoint & val);

#else
  /** Accessor to values */
  NumericalPoint getValueAtIndex(const UnsignedLong index);
  NumericalPoint getValueAtIndex(const UnsignedLong index) const;
  void setValueAtIndex(const UnsignedLong index,
                       const NumericalPoint & val);

  NumericalPoint getValueAtNearestTime(const NumericalScalar timestamp);
  NumericalPoint getValueAtNearestTime(const NumericalScalar timestamp) const;
  void setValueAtNearestTime(const NumericalScalar timestamp,
                             const NumericalPoint & val);

#endif

  RegularGrid getTimeGrid() const;

  /* Method __len__() is for Python */
  UnsignedLong __len__() const;

  /* Method __contains__() is for Python */
  Bool __contains__(const NumericalPoint & val) const;


  const NumericalPoint __getitem__ (const UnsignedLong index) const;
  void __setitem__ (const UnsignedLong index,
                    const NumericalPoint & val);

  void erase(const UnsignedLong first,
             const UnsignedLong last);
  void erase(const UnsignedLong index);

  /** erase the whole sample */
  void clear();

  /**
   * String converter
   * This method shows human readable information on the
   * internal state of an TimeSeries. It is used when streaming
   * the TimeSeries or for user information.
   */
  String __repr__() const;

  String __str__(const String & offset = "") const;

  /** Description accessor */
  void setDescription(const Description & description);
  Description getDescription() const;

  /** Dimension accessor */
  UnsignedLong getDimension() const;

  /** Size accessor */
  UnsignedLong getSize() const;

  /** Method add() appends an element to the collection */
  void add(const NumericalPoint & point);

  /** Method add() appends a sample to the collection */
  void add(const NumericalSample & sample);

  /** Append another time series to the collection. The time grids must match (one follows the other) */
  void add(const TimeSeries & continuer);

  /** Return the values stored in the time series as a sample */
  NumericalSample getSample() const;
  NumericalSample getNumericalSample() const;

  /** Compute the temporal mean of the time series */
  NumericalPoint getTemporalMean() const;

  /** Draw a marginal of the timeSerie */
  Graph drawMarginal(const UnsignedLong index = 0) const;

}; /* class TimeSeries */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_TIMESERIES_HXX */
