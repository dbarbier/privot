//                                               -*- C++ -*-
/**
 *  @file  TimeSeriesImplementation.cxx
 *  @brief The class TimeSeriesImplementation implements blank free samples
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
 *  @author lebrun
 *  @date   2011-08-01 17:35:56 +0200 (Mon, 01 Aug 2011)
 */
#include <limits>        // std::numeric_limits
#include <map>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <cstdio>        // std::fopen, std::errno
#include <cstring>       // std::strerror

#include "OTconfig.hxx"
#include "TimeSeriesImplementation.hxx"
#include "StorageManager.hxx"
#include "PersistentObjectFactory.hxx"
#include "Log.hxx"
#include "Exception.hxx"
#include "ResourceMap.hxx"
#include "Path.hxx"
#include "Curve.hxx"
#include "Os.hxx"
#include "TBB.hxx"

BEGIN_NAMESPACE_OPENTURNS

static Factory<RegularGrid> RegisteredFactoryTG("RegularGrid");

CLASSNAMEINIT(RegularGrid);

/*
 * String converter
 * This method shows human readable information on the
 * internal state of an TimeSeriesImplementation. It is used when streaming
 * the TimeSeriesImplementation or for user information.
 */
String RegularGrid::__repr__() const
{
  return OSS() << "class=RegularGrid name=" << getName()
               << " start=" << start_
               << " step=" << step_
               << " n=" << n_;
}

String RegularGrid::__str__(const String & offset) const
{
  return OSS() << offset
               << "RegularGrid(start=" << start_
               << ", step=" << step_
               << ", n=" << n_
               << ")";
}

/* Method save() stores the object through the StorageManager */
void RegularGrid::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "start_", start_);
  adv.saveAttribute( "step_", step_);
  adv.saveAttribute( "n_", n_);
}


/* Method load() reloads the object from the StorageManager */
void RegularGrid::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "start_", start_);
  adv.loadAttribute( "step_", step_);
  adv.loadAttribute( "n_", n_);
}





/* TSI_point saves the foolowing attributes:
 *  - p_tsi : a pointer to the related TimeSeriesImplementation
 *  - index : the index of the point in *p_tsi (0 <= index < p_tsi->n_)
 *  - dimension : the dimension of *p_tsi (ie, the actual dimension is size)
 *  - offset : some shift value used to manage either time, values or time+values points (generally 0 ou 1)
 *  - size : some length used to manage either time, values or time+values points (generally dimension-offset)
 */
TSI_point::TSI_point(TimeSeriesImplementation * p_tsi, UnsignedLong index, UnsignedLong offset, UnsignedLong size)
  : p_tsi_(p_tsi), index_(index), dimension_(p_tsi->dimension_), offset_(offset), size_(size)
{
#ifdef DEBUG_BOUNDCHECKING
  if ( (1 + dimension_) < (offset_ + size_) )
    throw InvalidArgumentException(HERE) << "Inconsistency in values passed to TSI_point: 1 + dimension=" << dimension_
                                         << " is less than offset=" << offset_ << " + size=" << size_;
#endif
}

TSI_point & TSI_point::operator = (const TSI_point & rhs)
{
  if (this != &rhs)
    {
#ifdef DEBUG_BOUNDCHECKING
      if (size_ != rhs.size_)
        throw InvalidArgumentException(HERE) << "Can't assign a point of size " << rhs.size_ << " to a point of size " << size_;
#endif
      if ( offset_ == 0 ) // We don't copy the timestamp, so the time series remains coherent
        std::copy( rhs.begin() + 1, rhs.end(), begin() + 1 );
      else
        std::copy( rhs.begin(), rhs.end(), begin() );
    }
  return *this;
}

//       TSI_point & TSI_point::operator = (const TSI_const_point & rhs)
//       {
// #ifdef DEBUG_BOUNDCHECKING
//        if ( size_ != rhs.getDimension() )
//          throw InvalidArgumentException(HERE) << "Can't assign a point of size " << rhs.getDimension() << " to a point of size " << size_;
// #endif
//        if ( offset_ == 0 ) // We don't copy the timestamp, so the time series remains coherent
//          std::copy( rhs.begin() + 1, rhs.end(), begin() + 1 );
//        else
//          std::copy( rhs.begin(), rhs.end(), begin() );
//        return *this;
//       }

//       TSI_point & TSI_point::operator = (const NumericalPoint & rhs)
//       {
// #ifdef DEBUG_BOUNDCHECKING
//        if ( size_ != rhs.getDimension() )
//          throw InvalidArgumentException(HERE) << "Can't assign a point of size " << rhs.getDimension() << " to a point of size " << size_;
// #endif
//        if ( offset_ == 0 ) // We don't copy the timestamp, so the time series remains coherent
//          std::copy( rhs.begin() + 1, rhs.end(), begin() + 1 );
//        else
//          std::copy( rhs.begin(), rhs.end(), begin() );
//         return *this;
//       }

NumericalScalar & TSI_point::operator [] (UnsignedLong i)
{
  return p_tsi_->data_[index_ * (1 + dimension_) + offset_ + i];
}

const NumericalScalar & TSI_point::operator [] (UnsignedLong i) const
{
  return p_tsi_->data_[index_ * (1 + dimension_) + offset_ + i];
}

NumericalScalar & TSI_point::at (UnsignedLong i)
{
  return p_tsi_->data_.at(index_ * (1 + dimension_) + offset_ + i);
}

const NumericalScalar & TSI_point::at (UnsignedLong i) const
{
  return p_tsi_->data_.at(index_ * (1 + dimension_) + offset_ + i);
}

TSI_point & TSI_point::operator += (const TSI_point & other)
{
#ifdef DEBUG_BOUNDCHECKING
  if (getDimension() != other.getDimension())
    throw InvalidArgumentException(HERE)
      << "Points of different sizes cannot be added (LHS size = "
      << size_
      << "; RHS size = "
      << other.size_;
#endif
  for (UnsignedLong i = 0; i < size_; ++i) (*this)[i] += other[i];
  return *this;
}

TSI_point & TSI_point::operator -= (const TSI_point & other)
{
#ifdef DEBUG_BOUNDCHECKING
  if (getDimension() != other.getDimension())
    throw InvalidArgumentException(HERE)
      << "Points of different sizes cannot be substracted (LHS size = "
      << size_
      << "; RHS size = "
      << other.size_;
#endif
  for (UnsignedLong i = 0; i < size_; ++i) (*this)[i] -= other[i];
  return *this;
}

TSI_point & TSI_point::operator += (const NumericalPoint & other)
{
#ifdef DEBUG_BOUNDCHECKING
  if (getDimension() != other.getDimension())
    throw InvalidArgumentException(HERE)
      << "Points of different sizes cannot be added (LHS size = "
      << size_
      << "; RHS size = "
      << other.getDimension();
#endif
  for (UnsignedLong i = 0; i < size_; ++i) (*this)[i] += other[i];
  return *this;
}

TSI_point & TSI_point::operator -= (const NumericalPoint & other)
{
#ifdef DEBUG_BOUNDCHECKING
  if (getDimension() != other.getDimension())
    throw InvalidArgumentException(HERE)
      << "NumericalPoints of different dimensions cannot be substracted (LHS size = "
      << size_
      << "; RHS size = "
      << other.getDimension();
#endif
  for (UnsignedLong i = 0; i < size_; ++i) (*this)[i] -= other[i];
  return *this;
}

TSI_point & TSI_point::operator *= (const NumericalScalar val)
{
  for(UnsignedLong i = 0; i < size_; ++i) (*this)[i] *= val;
  return *this;
}


bool operator == (const TSI_point & lhs, const TSI_point & rhs)
{
  return (lhs.getDimension() == rhs.getDimension()) &&
    std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

bool operator != (const TSI_point & lhs, const TSI_point & rhs)
{
  return ! (lhs == rhs);
}

bool operator < (const TSI_point & lhs, const TSI_point & rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                      rhs.begin(), rhs.end(),
                                      std::less<NumericalScalar>());
}

bool operator > (const TSI_point & lhs, const TSI_point & rhs)
{
  return !( lhs <= rhs );
}

bool operator <= (const TSI_point & lhs, const TSI_point & rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                      rhs.begin(), rhs.end(),
                                      std::less_equal<NumericalScalar>());
}

bool operator >= (const TSI_point & lhs, const TSI_point & rhs)
{
  return !( lhs < rhs );
}





TSI_const_point::TSI_const_point(const TimeSeriesImplementation * p_tsi, UnsignedLong index, UnsignedLong offset, UnsignedLong size)
  : p_tsi_(p_tsi), index_(index), dimension_(p_tsi->dimension_), offset_(offset), size_(size)
{
#ifdef DEBUG_BOUNDCHECKING
  if ( (1 + dimension_) < (offset_ + size_) )
    throw InvalidArgumentException(HERE) << "Inconsistency in values passed to TSI_point: 1 + dimension=" << dimension_
                                         << " is less than offset=" << offset_ << " + size=" << size_;
#endif
}

TSI_const_point::TSI_const_point(const TSI_point & point)
  : p_tsi_(point.p_tsi_), index_(point.index_), dimension_(point.dimension_), offset_(point.offset_), size_(point.size_) {}

bool operator == (const TSI_const_point & lhs, const TSI_const_point & rhs)
{
  return (lhs.getDimension() == rhs.getDimension()) &&
    std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

const NumericalScalar & TSI_const_point::operator [] (const UnsignedLong i) const
{
  return p_tsi_->data_[index_ * (1 + dimension_) + offset_ + i];
}

const NumericalScalar & TSI_const_point::at (const UnsignedLong i) const
{
  return p_tsi_->data_.at(index_ * (1 + dimension_) + offset_ + i);
}

bool operator != (const TSI_const_point & lhs, const TSI_const_point & rhs)
{
  return ! (lhs == rhs);
}

bool operator < (const TSI_const_point & lhs, const TSI_const_point & rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                      rhs.begin(), rhs.end(),
                                      std::less<NumericalScalar>());
}

bool operator > (const TSI_const_point & lhs, const TSI_const_point & rhs)
{
  return !( lhs <= rhs );
}

bool operator <= (const TSI_const_point & lhs, const TSI_const_point & rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                      rhs.begin(), rhs.end(),
                                      std::less_equal<NumericalScalar>());
}

bool operator >= (const TSI_const_point & lhs, const TSI_const_point & rhs)
{
  return !( lhs < rhs );
}








static Factory<TimeSeriesImplementation> RegisteredFactoryTSI("TimeSeriesImplementation");





CLASSNAMEINIT(TimeSeriesImplementation);



/* Default constructor is private */
TimeSeriesImplementation::TimeSeriesImplementation()
  : PersistentObject(),
    start_(0.0),
    timeStep_(1.0),
    n_(0),
    dimension_(0),
    data_(n_ * (1 + dimension_), 0.0),
    p_description_()
{
  // Nothing to do
}

/* Standard constructor */
TimeSeriesImplementation::TimeSeriesImplementation(const UnsignedLong n,
                                                   const UnsignedLong dim)
  : PersistentObject(),
    start_(0.0),
    timeStep_(1.0),
    n_(n),
    dimension_(dim),
    data_(n_ * (1 + dimension_), 0.0),
    p_description_()
{
  // We fill the timestamps in the time series
  for (UnsignedLong i = 0; i < n_; ++i) (*this)[i][0] = start_ + timeStep_ * i;
}

/* Constructor from a TimeGrid and a dimension */
TimeSeriesImplementation::TimeSeriesImplementation(const RegularGrid & tg,
                                                   const UnsignedLong dim)
  : PersistentObject(),
    start_( tg.getStart() ),
    timeStep_( tg.getStep() ),
    n_( tg.getN() ),
    dimension_(dim),
    data_(n_ * (1 + dimension_), 0.0),
    p_description_()
{
  // We fill the timestamps in the time series
  for (UnsignedLong i = 0; i < n_; ++i) (*this)[i][0] = tg.getValue(i);
}

/* Constructor from a TimeGrid and a sample */
TimeSeriesImplementation::TimeSeriesImplementation(const RegularGrid & tg,
                                                   const NumericalSample & sample)
  : PersistentObject(),
    start_( tg.getStart() ),
    timeStep_( tg.getStep() ),
    n_( tg.getN() ),
    dimension_(sample.getDimension()),
    data_(n_ * (1 + dimension_), 0.0),
    p_description_()
{
  if (n_ != sample.getSize()) throw InvalidArgumentException(HERE) << "Can't create a TimeSeries with a TimeGrid with " << tg.getN()
                                                                   << " steps and a sample of size " << sample.getSize();
  // We fill the timestamps in the time series
  for (UnsignedLong i = 0; i < n_; ++i)
    {
      (*this)[i][0] = tg.getValue(i);
      getValueAtIndex(i) = sample[i];
    }
}


// /* Constructor from a NumericalPoint */
// TimeSeriesImplementation::TimeSeriesImplementation(UnsignedLong size,
//                                                         const NumericalPoint & point)
//   : PersistentObject(),
//     size_(size),
//     dimension_(point.getDimension()),
//     data_(size_ * dimension_, 0.0),
//     p_description_()
// {
//   for (UnsignedLong i = 0; i < size_; ++i)
//     for (UnsignedLong j = 0; j < dimension_; ++j)
//       data_[i * dimension_ + j] = point[j];
// }


/* Constructor from a collection of NumericalPoint */
TimeSeriesImplementation::TimeSeriesImplementation(const Collection<NumericalPoint> & coll)
  : PersistentObject(),
    start_(0.0),
    timeStep_(1.0),
    n_( coll.getSize() ),
    dimension_((coll.getSize() > 1) ? coll[0].getDimension() - 1 : 0),
    data_(n_ * (1 + dimension_), 0.0),
    p_description_()
{
  if ((coll.getSize() > 1) && (coll[0].getDimension() < 1)) throw InvalidArgumentException(HERE) << "Can't create a TimeSeries from a collection with dimension < 1";
  std::copy( coll.begin(), coll.end(), begin() );
}

// /* Partial copy constructor */
// TimeSeriesImplementation::TimeSeriesImplementation(const TimeSeriesImplementation & other, iterator first, iterator last)
//   : PersistentObject(),
//     size_(last - first),
//     dimension_(other.getDimension()),
//     data_(size_ * dimension_, 0.0),
//     p_description_(other.p_description_)
// {
//   std::copy( first, last, begin() );
// }


/* Virtual constructor */
TimeSeriesImplementation * TimeSeriesImplementation::clone() const
{
  return new TimeSeriesImplementation(*this);
}

static inline
UnsignedLong getIndexAtNearestTime(const NumericalScalar timeStamp,
                                   const NumericalScalar start,
                                   const NumericalScalar timeStep,
                                   const UnsignedLong n)
{
  switch (n)
    {
    case 0:
      throw InvalidArgumentException(HERE) << "Can't find nearest timestamp in an empty TimeSeries";

    case 1:
      return 0;

    default:
      if ( timeStamp < start ) return 0;

      NumericalScalar stop(start + timeStep * ( n - 1 ));
      if ( timeStamp > stop ) return n - 1;

      UnsignedLong interpolatedN(static_cast<UnsignedLong>( nearbyint( ( timeStamp - start ) / timeStep ) ));
      return interpolatedN;
    }
}

TSI_point TimeSeriesImplementation::getValueAtNearestTime(const NumericalScalar timestamp)
{
  return getValueAtIndex( getIndexAtNearestTime( timestamp, start_, timeStep_, n_ ) );
}

TSI_const_point TimeSeriesImplementation::getValueAtNearestTime(const NumericalScalar timestamp) const
{
  return getValueAtIndex( getIndexAtNearestTime( timestamp, start_, timeStep_, n_ ) );
}

void TimeSeriesImplementation::setValueAtNearestTime(const NumericalScalar timestamp, const NumericalPoint & val)
{
  setValueAtIndex( getIndexAtNearestTime( timestamp, start_, timeStep_, n_ ), val );
}




/* Description Accessor */
void TimeSeriesImplementation::setDescription(const Description & description)
{
  if (description.getSize() != (1 + getDimension())) throw InvalidArgumentException(HERE) << "Error: the given description does not match the sample dimension.";
  p_description_ = description.getImplementation();
}


/* Description Accessor */
Description TimeSeriesImplementation::getDescription() const
{
  return p_description_.isNull() ? Description(1 + getDimension()) : *p_description_;
}

/* Comparison function */
Bool operator ==(const TimeSeriesImplementation & lhs, const TimeSeriesImplementation & rhs)
{
  return (lhs.n_ == rhs.n_) && (lhs.dimension_ == rhs.dimension_) &&
    std::equal( lhs.begin(), lhs.end(), rhs.begin() );
}

void TimeSeriesImplementation::erase(iterator first, iterator last)
{
  PersistentCollection<NumericalScalar>::iterator f = data_.begin() + (first - begin()) * (1 + dimension_);
  PersistentCollection<NumericalScalar>::iterator l = data_.begin() + (last  - begin()) * (1 + dimension_);
  data_.erase( f, l );
  n_ -= last - first;
}

void TimeSeriesImplementation::erase(const UnsignedLong first, const UnsignedLong last)
{
  PersistentCollection<NumericalScalar>::iterator f = data_.begin() + first * (1 + dimension_);
  PersistentCollection<NumericalScalar>::iterator l = data_.begin() + last  * (1 + dimension_);
  data_.erase( f, l );
  n_ -= last - first;
}

void TimeSeriesImplementation::clear()
{
  data_.clear();
  n_ = 0;
}

/* Method __contains__() is for Python */
Bool TimeSeriesImplementation::contains(const NumericalPoint & val) const
{
  for (UnsignedLong i = 0; i < n_; ++i) if ( getValueAtIndex(i) == val ) return true;
  return false;
}

/* String converter */
String TimeSeriesImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << TimeSeriesImplementation::GetClassName()
      << " name=" << getName()
      << " start=" << start_
      << " timeStep=" << timeStep_
      << " n=" << n_
      << " dimension=" << dimension_
      << " data=[";
  const char * sep = "";
  for(const_iterator it = begin(); it != end(); ++it, sep = ",") oss << sep << *it;
  oss << "]";
  return oss;
}

String TimeSeriesImplementation::__str__(const String & offset) const
{
  // First, print the description if it is not empty.
  // If you use the getDescription() method you get a default value
  // for the description that is not stored in the sample, producing a spurious output
  const Bool printDescription = !p_description_.isNull() && (p_description_->getSize() == (1 + dimension_));

  size_t twidth = 0; // column title max width
  size_t lwidth = 0; // LHS number max width
  size_t rwidth = 0; // RHS number max width
  size_t iwidth = 0; // index max width

  if (printDescription)
    {
      for( UnsignedLong j = 0; j < (1 + dimension_); ++j )
        twidth = std::max( twidth, (*p_description_)[j].size() );
    }

  for( UnsignedLong i = 0; i < n_; ++i )
    for( UnsignedLong j = 0; j < (1 + dimension_); ++j )
      {
        String st = OSS() << data_[i * (1 + dimension_) + j];
        size_t dotpos = st.find( '.' );
        lwidth = std::max( lwidth, (dotpos != String::npos) ? dotpos             : st.size() );
        rwidth = std::max( rwidth, (dotpos != String::npos) ? st.size() - dotpos : 0         );
      }

  if (twidth > lwidth + rwidth)
    rwidth = twidth - lwidth;
  else
    twidth = lwidth + rwidth;

  {
    // Computing the size of the last index (max width of the indexes)
    String sti = OSS() << n_ - 1;
    iwidth = sti.size();
  }

  OSS oss;
  // Print the column title
  if (printDescription)
    {
      oss << offset << String( iwidth , ' ' ) << "   [ ";
      const char * sep = "";
      for( UnsignedLong j = 0; j < (1 + dimension_); ++j, sep = " " )
        {
          oss << sep << (*p_description_)[j] << String( twidth - (*p_description_)[j].size(), ' ' );
        }
      oss << " ]\n";
    }

  const char * newline = "";
  for( UnsignedLong i = 0; i < n_; ++i, newline = "\n" )
    {
      String sti = OSS() << i;
      oss << newline << offset << String( iwidth - sti.size(), ' ' ) << sti << " : [ ";
      const char * sep = "";
      for( UnsignedLong j = 0; j < (1 + dimension_); ++j, sep = " " )
        {
          String st = OSS() << data_[i * (1 + dimension_) + j];
          size_t dotpos = st.find( '.' );
          oss << sep << String( lwidth - ((dotpos != String::npos) ? dotpos : st.size()), ' ' )
              << st
              << String( rwidth - ((dotpos != String::npos) ? st.size() - dotpos : 0), ' ' );
        }
      oss << " ]";
    }
  return oss;
}


/* Appends an element to the collection */
TimeSeriesImplementation & TimeSeriesImplementation::add(const NumericalPoint & point)
{
  const UnsignedLong oldSize(n_);
  ++n_;
  data_.resize( n_ * ( 1 + dimension_) );
  data_[oldSize * (1 + dimension_)] = start_ + timeStep_ * oldSize;
  memcpy( &data_[oldSize * (1 + dimension_) + 1], &point[0], dimension_ * sizeof(NumericalScalar) );
  return *this;
}


/* Appends a sample to the collection */
TimeSeriesImplementation & TimeSeriesImplementation::add(const NumericalSample & sample)
{
  const UnsignedLong oldSize(n_);
  UnsignedLong sampleSize(sample.getSize());
  n_ += sampleSize;
  data_.resize( n_ * ( 1 + dimension_) );
  for (UnsignedLong i = 0; i < sampleSize; ++i)
    {
      data_[(oldSize + i) * (1 + dimension_)] = start_ + timeStep_ * (oldSize + i);
      memcpy( &data_[(oldSize + i)  * (1 + dimension_) + 1], &sample[i][0], dimension_ * sizeof(NumericalScalar) );
    }
  return *this;
}

/* Append another time series to the collection. The time grids must match (one follows the other) */
TimeSeriesImplementation & TimeSeriesImplementation::add(const TimeSeriesImplementation & continuer)
{
  RegularGrid continuerTG(continuer.getTimeGrid());
  const UnsignedLong oldSize(n_);
  n_ += continuerTG.getN();
  data_.resize( n_ * ( 1 + dimension_) );
  memcpy( &data_[oldSize  * (1 + dimension_)], &continuer.data_[0], continuerTG.getN() * (1 + dimension_) * sizeof(NumericalScalar) );
  return *this;
}

/* Compute the temporal mean of the time series */
NumericalPoint TimeSeriesImplementation::getTemporalMean() const
{
  NumericalPoint mean( dimension_, 0.0 );
  // TODO: parallelization
  for (UnsignedLong i = 0; i < n_; ++i)
    mean += getValueAtIndex(i);
  return mean * (1.0 / n_);
}

/* Return the values stored in the time series as a sample */
NumericalSample TimeSeriesImplementation::getSample() const
{
  NumericalSampleImplementation out(n_, dimension_);
  for (UnsignedLong i = 0; i < n_; ++i)
    out[i] = getValueAtIndex(i);
  return out;
}

NumericalSample TimeSeriesImplementation::getNumericalSample() const
{
  LOGUSER(OSS() << "In class TimeSeries, method getNumericalSample is deprecated in favor of getSample.");
  return getSample();
}

/* Return the time series as a sample, ie its values and time grid */
NumericalSample TimeSeriesImplementation::asSample() const
{
  // Waiting for a better implementation...
  NumericalSampleImplementation out(n_, dimension_ + 1);
  for (UnsignedLong i = 0; i < n_; ++i)
    out[i] = (*this)[i];
  return out;
}

NumericalSample TimeSeriesImplementation::asNumericalSample() const
{
  LOGUSER(OSS() << "In class TimeSeries, method asNumericalSample is deprecated in favor of asSample.");
  return asSample();
}

/* Draw a marginal of the TimeSeries */
Graph TimeSeriesImplementation::drawMarginal(const UnsignedLong index) const
{
  if (index > getDimension() - 1 ) throw InvalidArgumentException(HERE) << "Error : indice should be between [0, " << getDimension() - 1 << "]";
  // Discretization of the x axis
  const String title(OSS() << getName() << " - " << index << " marginal" );
  NumericalSample dataTX(n_, 2);
  UnsignedLong shift(0);
  for (UnsignedLong i = 0; i < n_; ++i)
    {
      dataTX(i, 0) = data_[shift];
      dataTX(i, 1) = data_[shift + index + 1];
      shift += dimension_ + 1;
    }
  const Curve curveSerie(dataTX);
  Graph graph(title, "Time", "Values", true, "topright");
  graph.add(curveSerie);
  return graph;
}


/* Method save() stores the object through the StorageManager */
void TimeSeriesImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "start_", start_);
  adv.saveAttribute( "timeStep_", timeStep_);
  adv.saveAttribute( "n_", n_);
  adv.saveAttribute( "dimension_", dimension_);
  adv.saveAttribute( "data_", data_);
  if (!p_description_.isNull())
    adv.saveAttribute( "description_", *p_description_ );
}


/* Method load() reloads the object from the StorageManager */
void TimeSeriesImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "start_", start_);
  adv.loadAttribute( "timeStep_", timeStep_);
  adv.loadAttribute( "n_", n_);
  adv.loadAttribute( "dimension_", dimension_);
  adv.loadAttribute( "data_", data_);
  Description description;
  adv.loadAttribute( "description_", description );
  if (description.getSize() != 0) setDescription(description);
}

END_NAMESPACE_OPENTURNS
