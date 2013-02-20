//                                               -*- C++ -*-
/**
 *  @file  TimeSeries.cxx
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
 */
#include <iomanip>
#include <fstream>
#include "TimeSeries.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(TimeSeries);


/* Default constructor */
TimeSeries::TimeSeries()
  : TypedInterfaceObject<TimeSeriesImplementation>(new TimeSeriesImplementation(0, 1))
{
  // Nothing to do
}

/* Constructor with size and dimension */
TimeSeries::TimeSeries(const UnsignedLong size,
                       const UnsignedLong dim)
  : TypedInterfaceObject<TimeSeriesImplementation>(new TimeSeriesImplementation(size, dim))
{
  // Nothing to do
}

/* Constructor from implementation */
TimeSeries::TimeSeries(const TimeSeriesImplementation & implementation)
  : TypedInterfaceObject<TimeSeriesImplementation>(implementation.clone())
{
  // Nothing to do
}

/* Constructor from a TimeGrid and a dimension */
TimeSeries::TimeSeries(const RegularGrid & tg,
                       const UnsignedLong dim)
  : TypedInterfaceObject<TimeSeriesImplementation>(new TimeSeriesImplementation(tg, dim))
{
  // Nothing to do
}

/* Constructor from a TimeGrid and a sample */
TimeSeries::TimeSeries(const RegularGrid & tg,
                       const NumericalSample & sample)
  : TypedInterfaceObject<TimeSeriesImplementation>(new TimeSeriesImplementation(tg, sample))
{
  // Nothing to do
}

/* Constructor from implementation */
TimeSeries::TimeSeries(const Implementation & implementation)
  : TypedInterfaceObject<TimeSeriesImplementation>(implementation)
{
  // Nothing to do
}

/* Constructor from a NumericalPoint (all elements are equal to the NumericalPoint) */
// TimeSeries::TimeSeries(UnsignedLong size,
//                             const NumericalPoint & point)
//   : TypedInterfaceObject<TimeSeriesImplementation>(new TimeSeriesImplementation(size, point))
// {
//   // Nothing to do
// }

/* Comparison operator */
Bool TimeSeries::operator ==(const TimeSeries & other) const
{
  return true;
}


TSI_point TimeSeries::operator [] (const UnsignedLong index)
{
#ifdef DEBUG_BOUNDCHECKING
  copyOnWrite();
  return this->at(index);
#else
  copyOnWrite();
  return (*getImplementation())[index];
#endif /* DEBUG_BOUNDCHECKING */
}

TSI_const_point TimeSeries::operator [] (const UnsignedLong index) const
{
#ifdef DEBUG_BOUNDCHECKING
  return this->at(index);
#else
  return (*getImplementation())[index];
#endif /* DEBUG_BOUNDCHECKING */
}


NumericalScalar & TimeSeries::operator () (const UnsignedLong i,
                                           const UnsignedLong j)
{
#ifdef DEBUG_BOUNDCHECKING
  // No copyOnWrite() as the at() method already do it
  return this->at(i, j);
#else
  copyOnWrite();
  return (*getImplementation())[i][j];
#endif /* DEBUG_BOUNDCHECKING */
}

const NumericalScalar & TimeSeries::operator () (const UnsignedLong i,
                                                 const UnsignedLong j) const
{
#ifdef DEBUG_BOUNDCHECKING
  return this->at(i, j);
#else
  return (*getImplementation())[i][j];
#endif /* DEBUG_BOUNDCHECKING */
}


TSI_point TimeSeries::at (const UnsignedLong index)
{
  if (index >= getSize()) throw OutOfBoundException(HERE) << "Index (" << index << ") is not less than size (" << getSize() << ")";
  copyOnWrite();
  return (*getImplementation())[index];
}

TSI_const_point TimeSeries::at (const UnsignedLong index) const
{
  if (index >= getSize()) throw OutOfBoundException(HERE) << "Index (" << index << ") is not less than size (" << getSize() << ")";
  return (*getImplementation())[index];
}

NumericalScalar & TimeSeries::at (const UnsignedLong i,
                                  const UnsignedLong j)
{
  if (i >= getSize()) throw OutOfBoundException(HERE) << "i (" << i << ") is not less than size (" << getSize() << ")";
  if (j > getDimension()) throw OutOfBoundException(HERE) << "j (" << j << ") is greater than dimension (" << getDimension() << ")";
  copyOnWrite();
  return (*getImplementation())[i][j];
}

const NumericalScalar & TimeSeries::at (const UnsignedLong i,
                                        const UnsignedLong j) const
{
  if (i >= getSize()) throw OutOfBoundException(HERE) << "i (" << i << ") is not less than size (" << getSize() << ")";
  if (j > getDimension()) throw OutOfBoundException(HERE) << "j (" << j << ") is greater than dimension (" << getDimension() << ")";
  return (*getImplementation())[i][j];
}

TSI_point TimeSeries::getValueAtIndex(const UnsignedLong index)
{
  copyOnWrite();
  return getImplementation()->getValueAtIndex(index);
}

TSI_const_point TimeSeries::getValueAtIndex(const UnsignedLong index) const
{
  return getImplementation()->getValueAtIndex(index);
}

void TimeSeries::setValueAtIndex(const UnsignedLong index,
                                 const NumericalPoint & val)
{
  copyOnWrite();
  getImplementation()->setValueAtIndex(index, val);
}

TSI_point TimeSeries::getValueAtNearestTime(const NumericalScalar timestamp)
{
  copyOnWrite();
  return getImplementation()->getValueAtNearestTime(timestamp);
}

TSI_const_point TimeSeries::getValueAtNearestTime(const NumericalScalar timestamp) const
{
  return getImplementation()->getValueAtNearestTime(timestamp);
}

void TimeSeries::setValueAtNearestTime(const NumericalScalar timestamp,
                                       const NumericalPoint & val)
{
  copyOnWrite();
  getImplementation()->setValueAtNearestTime(timestamp, val);
}

RegularGrid TimeSeries::getTimeGrid() const
{
  return getImplementation()->getTimeGrid();
}

const NumericalPoint TimeSeries::__getitem__ (const UnsignedLong index) const
{
  return this->at(index);
}

void TimeSeries::__setitem__ (const UnsignedLong index,
                              const NumericalPoint & val)
{
  copyOnWrite();
  this->at(index) = val;
}

/* Method __len__() is for Python */
UnsignedLong TimeSeries::__len__() const
{
  return getSize();
}

/* Method __contains__() is for Python */
Bool TimeSeries::__contains__(const NumericalPoint & val) const
{
  return getImplementation()->contains(val);
}



/* String converter */
String TimeSeries::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " name=" << getName()
               << " description=" << getDescription()
               << " implementation=" << getImplementation()->__repr__();
}

String TimeSeries::__str__(const String & offset) const
{
  return getImplementation()->__str__();
}


/* Description accessor */
void TimeSeries::setDescription(const Description & description)
{
  copyOnWrite();
  getImplementation()->setDescription(description);
}



/* Description accessor */
Description TimeSeries::getDescription() const
{
  return getImplementation()->getDescription();
}


/* Dimension accessor */
UnsignedLong TimeSeries::getDimension() const
{
  return getImplementation()->getDimension();
}


/* Size accessor */
UnsignedLong TimeSeries::getSize() const
{
  return getImplementation()->getSize();
}

void TimeSeries::erase(const UnsignedLong first,
                       const UnsignedLong last)
{
  copyOnWrite();
  getImplementation()->erase(first, last);
}


void TimeSeries::erase(const UnsignedLong index)
{
  copyOnWrite();
  getImplementation()->erase(index, index + 1);
}

void TimeSeries::erase(TimeSeriesImplementation::iterator first,
                       TimeSeriesImplementation::iterator last)
{
  copyOnWrite();
  getImplementation()->erase(first, last);
}

/* erase the whole sample */
void TimeSeries::clear()
{
  copyOnWrite();
  getImplementation()->clear();
}

/* Method add() appends an element to the collection */
void TimeSeries::add(const NumericalPoint & point)
{
  if ( (getSize() > 0) && (getDimension() != point.getDimension()) )
    throw InvalidArgumentException(HERE)
      << "Point has invalid dimension (dim=" << point.getDimension()
      << ") for time series (dim=" << getDimension() << ")";

  copyOnWrite();
  getImplementation()->add(point);
}

/* Method add() appends an element to the collection */
void TimeSeries::add(const NumericalSample & sample)
{
  if ( (getSize() > 0) && (getDimension() != sample.getDimension()) )
    throw InvalidArgumentException(HERE)
      << "Sample has invalid dimension (dim=" << sample.getDimension()
      << ") for time series (dim=" << getDimension() << ")";

  copyOnWrite();
  getImplementation()->add(sample);
}

/** Append another time series to the collection. The time grids must match (one follows the other) */
void TimeSeries::add(const TimeSeries & continuer)
{
  RegularGrid myTG        = getTimeGrid();
  RegularGrid continuerTG = continuer.getTimeGrid();
  if ( ! continuerTG.follows( myTG ) )
    throw InvalidArgumentException(HERE) << "The TimeGrid of the TimeSeries to be appended does not follow the original TimeGrid: original="
                                         << myTG
                                         << " - continuer="
                                         << continuerTG;

  if ( getDimension() != continuer.getDimension() )
    throw InvalidArgumentException(HERE) << "The dimension of the TimeSeries to be appended differs from the original dimension: original's dimension="
                                         << getDimension()
                                         << " - continuer's dimension="
                                         << continuer.getDimension();

  copyOnWrite();
  getImplementation()->add( *continuer.getImplementation() );
}


/* Return the values stored in the time series as a sample */
NumericalSample TimeSeries::getSample() const
{
  return getImplementation()->getSample();
}

NumericalSample TimeSeries::getNumericalSample() const
{
  return getImplementation()->getNumericalSample();
}

/* Compute the temporal mean of the time series */
NumericalPoint TimeSeries::getTemporalMean() const
{
  return getImplementation()->getTemporalMean();
}

/* Draw a marginal of the time series */
Graph TimeSeries::drawMarginal(const UnsignedLong index) const
{
  return getImplementation()->drawMarginal(index);
}


END_NAMESPACE_OPENTURNS
