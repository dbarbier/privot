//                                               -*- C++ -*-
/**
 *  @file  TimeSeriesImplementation.hxx
 *  @brief The class TimeSeriesImplementation implements indexed by time
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
 *  @date   2011-05-24 19:30:41 +0200 (Tue, 24 May 2011)
 *  Id      TimeSeriesImplementation.hxx 1910 2011-05-24 17:30:41Z schueller
 */
#ifndef OPENTURNS_TIMESERIESIMPLEMENTATION_HXX
#define OPENTURNS_TIMESERIESIMPLEMENTATION_HXX

#include <stdint.h> // for uint64_t
#include <cmath>    // for nearbyint

#include "NumericalPoint.hxx"
#include "Description.hxx"
#include "Indices.hxx"
#include "PersistentCollection.hxx"
#include "Collection.hxx"
#include "NumericalSample.hxx"
#include "Graph.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class RegularGrid
 *
 * The RegularGrid class defines some regular time slice defined by three values:
 *  - either a start time, an interval (aka timeStep) and a number of steps
 *  - or a start time, a number of steps and an end time
 *
 * Note: the end time conforms to the STL standard of "one past the end", so
 * the following formula defines the end time:
 * end time = start time + ( timeStep * steps )
 */

class RegularGrid
  : public PersistentObject
{
  CLASSNAME;
  /** The start of the regular grid */
  NumericalScalar start_;
  /** The step of the regular grid */
  NumericalScalar step_;
  /** The number of stamps of the underlying regular grid */
  UnsignedLong n_;

public:


  RegularGrid()
    : PersistentObject(), start_(0.0), step_(1.0), n_(1)
  {}
  RegularGrid(const NumericalScalar start,
              const NumericalScalar step,
              const UnsignedLong n)
    : PersistentObject(), start_(start), step_(step), n_(n)
  {
    // Check if there is at least one point
    if (n < 1) throw InvalidArgumentException(HERE) << "Error: a regular grid must have at least one point.";
  }

  RegularGrid * clone() const
  {
    return new RegularGrid( *this );
  }
  inline Bool operator == (const RegularGrid & rhs) const
  {
    const RegularGrid & lhs = *this;
    return (lhs.start_ == rhs.start_) && (lhs.step_ == rhs.step_) && (lhs.n_ == rhs.n_);
  }
  inline NumericalScalar getStart() const
  {
    return start_;
  }
  /* This method computes the timestamp of the very next step past the time series (STL convention) */
  inline NumericalScalar getEnd() const
  {
    return start_ + step_ * n_;
  }
  inline NumericalScalar getStep() const
  {
    return step_;
  }
  inline UnsignedLong getN() const
  {
    return n_;
  }
  inline NumericalScalar getValue(const UnsignedLong i) const
  {
    if (i >= n_) throw InvalidArgumentException(HERE) << "Error: the given index i=" << i << "must be less than the number of ticks n=" << n_;
    return start_ + i * step_;
  }
  inline NumericalPoint getValues() const
  {
    NumericalPoint values(n_);
    for (UnsignedLong i = 0; i < n_; ++i) values[i] = start_ + i * step_;
    return values;
  }
  inline Bool follows(const RegularGrid & starter) const
  {
    const RegularGrid & continuer = *this;
    return (starter.getEnd() == continuer.getStart()) && (starter.getStep()) == continuer.getStep();
  }

  /**
   * String converter
   * This method shows human readable information on the
   * internal state of an TimeSeriesImplementation. It is used when streaming
   * the TimeSeriesImplementation or for user information.
   */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

}; // end class RegularGrid


#ifndef SWIG
class TimeSeriesImplementation;
class TSI_const_point;

/****************************************/


class TSI_point
{
  TimeSeriesImplementation * p_tsi_;
  UnsignedLong index_;
  UnsignedLong dimension_;
  UnsignedLong offset_;
  UnsignedLong size_;

  friend class TSI_const_point;
  friend std::ostream & operator << (std::ostream & os, const TSI_point & point);

public:
  typedef       NumericalScalar *       iterator;
  typedef const NumericalScalar * const_iterator;

public:
  TSI_point(TimeSeriesImplementation * p_tsi,
            UnsignedLong index, UnsignedLong offset, UnsignedLong size);

  TSI_point & operator = (const TSI_point & rhs);

  template <typename POINT>
  inline
  TSI_point & operator = (POINT rhs)
  {
#ifdef DEBUG_BOUNDCHECKING
    if ( size_ != rhs.getDimension() )
      throw InvalidArgumentException(HERE) << "Can't assign a point of size " << rhs.getDimension() << " to a point of size " << size_;
#endif
    if ( offset_ == 0 ) // We don't copy the timestamp, so the time series remains coherent
      std::copy( rhs.begin() + 1, rhs.end(), begin() + 1 );
    else
      std::copy( rhs.begin(), rhs.end(), begin() );
    return *this;
  }

  // TSI_point & operator = (const TSI_const_point & rhs);
  // TSI_point & operator = (const NumericalPoint & rhs);

  NumericalScalar & operator [] (const UnsignedLong i);
  const NumericalScalar & operator [] (const UnsignedLong i) const;
  NumericalScalar & at (const UnsignedLong i);
  const NumericalScalar & at (const UnsignedLong i) const;

  inline Collection<NumericalScalar> getCollection() const
  {
    return Collection<NumericalScalar>( begin(), end() );
  }
  inline operator NumericalPoint () const
  {
    return getCollection();
  }

  inline UnsignedLong getDimension() const
  {
    return size_;
  }

  inline iterator begin()
  {
    return &operator[](0);
  }
  inline iterator end()
  {
    return &operator[](size_);
  }
  inline const_iterator begin() const
  {
    return &operator[](0);
  }
  inline const_iterator end() const
  {
    return &operator[](size_);
  }

  TSI_point & operator += (const TSI_point & other);
  TSI_point & operator -= (const TSI_point & other);
  TSI_point & operator += (const NumericalPoint & other);
  TSI_point & operator -= (const NumericalPoint & other);
  TSI_point & operator *= (const NumericalScalar val);
};

bool operator == (const TSI_point & lhs, const TSI_point & rhs);
bool operator != (const TSI_point & lhs, const TSI_point & rhs);
bool operator <  (const TSI_point & lhs, const TSI_point & rhs);
bool operator >  (const TSI_point & lhs, const TSI_point & rhs);
bool operator <= (const TSI_point & lhs, const TSI_point & rhs);
bool operator >= (const TSI_point & lhs, const TSI_point & rhs);

inline
std::ostream & operator <<(std::ostream & os, const TSI_point & point)
{
  return os << NumericalPoint( point );
}

inline
OStream & operator << (OStream & OS, const TSI_point & point)
{
  OS.getStream() << NumericalPoint( point ).__repr__();
  return OS;
}


class TSI_const_point
{
  const TimeSeriesImplementation * p_tsi_;
  UnsignedLong index_;
  UnsignedLong dimension_;
  UnsignedLong offset_;
  UnsignedLong size_;

  friend std::ostream & operator << (std::ostream & os, const TSI_const_point & point);

public:
  typedef       NumericalScalar *       iterator;
  typedef const NumericalScalar * const_iterator;

public:
  TSI_const_point(const TimeSeriesImplementation * p_nsi,
                  UnsignedLong index, UnsignedLong offset, UnsignedLong size);
  TSI_const_point(const TSI_point & point);

  const NumericalScalar & operator [] (const UnsignedLong i) const;
  const NumericalScalar & at (const UnsignedLong i) const;

  inline Collection<NumericalScalar> getCollection() const
  {
    return Collection<NumericalScalar>( begin(), end() );
  }
  inline operator NumericalPoint () const
  {
    return getCollection();
  }

  inline UnsignedLong getDimension() const
  {
    return size_;
  }

  inline const_iterator begin() const
  {
    return &operator[](0);
  }
  inline const_iterator end() const
  {
    return &operator[](size_);
  }
};

bool operator == (const TSI_const_point & lhs, const TSI_const_point & rhs);
bool operator != (const TSI_const_point & lhs, const TSI_const_point & rhs);
bool operator <  (const TSI_const_point & lhs, const TSI_const_point & rhs);
bool operator >  (const TSI_const_point & lhs, const TSI_const_point & rhs);
bool operator <= (const TSI_const_point & lhs, const TSI_const_point & rhs);
bool operator >= (const TSI_const_point & lhs, const TSI_const_point & rhs);

inline
std::ostream & operator <<(std::ostream & os, const TSI_const_point & point)
{
  return os << NumericalPoint( point );
}

inline
OStream & operator << (OStream & OS, const TSI_const_point & point)
{
  OS.getStream() << NumericalPoint( point ).__repr__();
  return OS;
}

inline
NumericalPoint operator * (const TSI_const_point & point,
                           const NumericalScalar val)
{
  NumericalPoint res(point.getDimension(), 0.);
  for(UnsignedLong i = 0; i < point.getDimension(); ++i) res[i] = point[i] * val;
  return res;
}

inline
NumericalPoint operator * (const NumericalScalar val,
                           const TSI_const_point & point)
{
  return point * val;
}




/****************************************/


class TSI_iterator
{
  TimeSeriesImplementation * p_tsi_;
  UnsignedLong current_;
  UnsignedLong offset_;
  UnsignedLong size_;

public:
  typedef size_t difference_type;
  typedef std::random_access_iterator_tag iterator_category;
  typedef TSI_point value_type;
  typedef TSI_point * pointer;
  typedef TSI_point   reference;

  friend class TSI_const_iterator;
  friend bool operator == (const TSI_iterator & lhs, const TSI_iterator & rhs);
  friend bool operator <  (const TSI_iterator & lhs, const TSI_iterator & rhs);
  friend bool operator >  (const TSI_iterator & lhs, const TSI_iterator & rhs);
  friend difference_type operator - (const TSI_iterator & lhs, const TSI_iterator & rhs);

public:
  TSI_iterator() : p_tsi_(0), current_(0), offset_(0), size_(0) {}
  TSI_iterator(TimeSeriesImplementation & nsi, UnsignedLong index, UnsignedLong offset, UnsignedLong size)
    : p_tsi_(&nsi), current_(index), offset_(offset), size_(size) {}

  inline TSI_iterator & operator ++ ()
  {
    ++current_;
    return *this;
  }
  inline TSI_iterator & operator -- ()
  {
    --current_;
    return *this;
  }
  inline TSI_iterator   operator ++ (int)
  {
    TSI_iterator old(*this);
    ++current_;
    return old;
  }
  inline TSI_iterator   operator -- (int)
  {
    TSI_iterator old(*this);
    --current_;
    return old;
  }

  inline TSI_iterator & operator += (difference_type n)
  {
    current_ += n;
    return *this;
  }
  inline TSI_iterator & operator -= (difference_type n)
  {
    current_ -= n;
    return *this;
  }
  inline TSI_iterator   operator +  (difference_type n) const
  {
    TSI_iterator old(*this);
    old.current_ += n;
    return old;
  }
  inline TSI_iterator   operator -  (difference_type n) const
  {
    TSI_iterator old(*this);
    old.current_ -= n;
    return old;
  }

  inline TSI_point operator *  () const
  {
    return TSI_point(p_tsi_, current_, offset_, size_);
  }
};

inline
bool operator == (const TSI_iterator & lhs, const TSI_iterator & rhs)
{
  return (lhs.p_tsi_ == rhs.p_tsi_) && (lhs.current_ == rhs.current_) && (lhs.offset_ == rhs.offset_) && (lhs.size_ == rhs.size_);
}

inline
bool operator != (const TSI_iterator & lhs, const TSI_iterator & rhs)
{
  return ! (lhs == rhs);
}

inline
bool operator < (const TSI_iterator & lhs, const TSI_iterator & rhs)
{
  return (lhs.p_tsi_ == rhs.p_tsi_) && (lhs.current_ < rhs.current_) && (lhs.offset_ == rhs.offset_) && (lhs.size_ == rhs.size_);
}

inline
bool operator > (const TSI_iterator & lhs, const TSI_iterator & rhs)
{
  return (lhs.p_tsi_ == rhs.p_tsi_) && (lhs.current_ > rhs.current_) && (lhs.offset_ == rhs.offset_) && (lhs.size_ == rhs.size_);
}

inline
TSI_iterator::difference_type operator - (const TSI_iterator & lhs, const TSI_iterator & rhs)
{
  return (lhs.current_ - rhs.current_);
}




class TSI_const_iterator
{
  const TimeSeriesImplementation * p_tsi_;
  UnsignedLong current_;
  UnsignedLong offset_;
  UnsignedLong size_;

public:
  typedef size_t difference_type;
  typedef std::random_access_iterator_tag iterator_category;
  typedef TSI_const_point   value_type;
  typedef TSI_const_point * pointer;
  typedef TSI_const_point   reference;

  friend bool operator == (const TSI_const_iterator & lhs, const TSI_const_iterator & rhs);
  friend bool operator <  (const TSI_const_iterator & lhs, const TSI_const_iterator & rhs);
  friend bool operator >  (const TSI_const_iterator & lhs, const TSI_const_iterator & rhs);
  friend difference_type operator - (const TSI_const_iterator & lhs, const TSI_const_iterator & rhs);

public:
  TSI_const_iterator() : p_tsi_(0), current_(0), offset_(0), size_(0) {}
  TSI_const_iterator(const TimeSeriesImplementation & nsi, UnsignedLong index, UnsignedLong offset, UnsignedLong size)
    : p_tsi_(&nsi), current_(index), offset_(offset), size_(size) {}
  TSI_const_iterator(const TSI_iterator & iter) : p_tsi_(iter.p_tsi_), current_(iter.current_), offset_(iter.offset_), size_(iter.size_)  {}

  inline TSI_const_iterator & operator ++ ()
  {
    ++current_;
    return *this;
  }
  inline TSI_const_iterator & operator -- ()
  {
    --current_;
    return *this;
  }
  inline TSI_const_iterator   operator ++ (int)
  {
    TSI_const_iterator old(*this);
    ++current_;
    return old;
  }
  inline TSI_const_iterator   operator -- (int)
  {
    TSI_const_iterator old(*this);
    --current_;
    return old;
  }

  inline TSI_const_iterator & operator += (difference_type n)
  {
    current_ += n;
    return *this;
  }
  inline TSI_const_iterator & operator -= (difference_type n)
  {
    current_ -= n;
    return *this;
  }
  inline TSI_const_iterator   operator +  (difference_type n) const
  {
    TSI_const_iterator old(*this);
    old.current_ += n;
    return old;
  }
  inline TSI_const_iterator   operator -  (difference_type n) const
  {
    TSI_const_iterator old(*this);
    old.current_ -= n;
    return old;
  }

  inline TSI_const_point operator *  () const
  {
    return TSI_const_point(p_tsi_, current_, offset_, size_);
  }
};

inline
bool operator == (const TSI_const_iterator & lhs, const TSI_const_iterator & rhs)
{
  return (lhs.p_tsi_ == rhs.p_tsi_) && (lhs.current_ == rhs.current_) && (lhs.offset_ == rhs.offset_) && (lhs.size_ == rhs.size_);
}

inline
bool operator != (const TSI_const_iterator & lhs, const TSI_const_iterator & rhs)
{
  return ! (lhs == rhs);
}

inline
bool operator < (const TSI_const_iterator & lhs, const TSI_const_iterator & rhs)
{
  return (lhs.p_tsi_ == rhs.p_tsi_) && (lhs.current_ < rhs.current_) && (lhs.offset_ == rhs.offset_) && (lhs.size_ == rhs.size_);
}

inline
bool operator > (const TSI_const_iterator & lhs, const TSI_const_iterator & rhs)
{
  return (lhs.p_tsi_ == rhs.p_tsi_) && (lhs.current_ > rhs.current_) && (lhs.offset_ == rhs.offset_) && (lhs.size_ == rhs.size_);
}

inline
TSI_const_iterator::difference_type operator - (const TSI_const_iterator & lhs, const TSI_const_iterator & rhs)
{
  return (lhs.current_ - rhs.current_);
}

#endif


/****************************************/



/**
 * @class TimeSeriesImplementation
 */

class TimeSeriesImplementation
  : public PersistentObject
{
  CLASSNAME;
  friend class TSI_point;
  friend class TSI_const_point;
  friend class TSI_iterator;
  friend class TSI_const_iterator;
  friend class Factory<TimeSeriesImplementation>;
  friend class BuildMethodMap;
  friend class ExportMethodMap;
#ifndef SWIG
  friend Bool operator ==(const TimeSeriesImplementation & lhs, const TimeSeriesImplementation & rhs);
#endif

public:

  /* Some typedefs for easy reading */
  typedef TSI_iterator               iterator;
  typedef TSI_const_iterator         const_iterator;

  typedef Collection<UnsignedLong>   UnsignedLongCollection;

protected:

  /**
   * Default constructor is protected
   */
  TimeSeriesImplementation();

public:

  /** Standard constructor */
  TimeSeriesImplementation(const UnsignedLong n,
                           const UnsignedLong dim);

  /** Constructor from a TimeGrid and a dimension */
  TimeSeriesImplementation(const RegularGrid & tg,
                           const UnsignedLong dim);
  /** Constructor from a TimeGrid and a sample */
  TimeSeriesImplementation(const RegularGrid & tg,
                           const NumericalSample & sample);

#ifndef SWIG
  /** Constructor from a collection of NumericalPoint */
  TimeSeriesImplementation(const Collection<NumericalPoint> & coll);

  /** Partial copy constructor */
  // TimeSeriesImplementation(const TimeSeriesImplementation & other,
  //                       iterator first,
  //                       iterator last);

  inline iterator begin()
  {
    return iterator(*this, 0, 0, dimension_ + 1);
  }
  inline iterator end()
  {
    return iterator(*this, n_, 0, dimension_ + 1);
  }
  inline const_iterator begin() const
  {
    return const_iterator(*this, 0, 0, dimension_ + 1);
  }
  inline const_iterator end() const
  {
    return const_iterator(*this, n_, 0, dimension_ + 1);
  }

  void erase(const UnsignedLong first, const UnsignedLong last);
  void erase(iterator first, iterator last);
  void clear();

  inline TSI_point operator [] (const UnsignedLong index)
  {
    return TSI_point(this, index, 0, dimension_ + 1);
  }
  inline TSI_const_point operator [] (const UnsignedLong index) const
  {
    return TSI_const_point(this, index, 0, dimension_ + 1);
  }

  /** Accessor to values */
  inline TSI_point getValueAtIndex(const UnsignedLong index)
  {
    return TSI_point(this, index, 1, dimension_);
  }
  inline TSI_const_point getValueAtIndex(const UnsignedLong index) const
  {
    return TSI_const_point(this, index, 1, dimension_);
  }
  inline void setValueAtIndex(const UnsignedLong index, const NumericalPoint & val)
  {
    TSI_point(this, index, 1, dimension_) = val;
  }

  TSI_point getValueAtNearestTime(const NumericalScalar timestamp);
  TSI_const_point getValueAtNearestTime(const NumericalScalar timestamp) const;
  void setValueAtNearestTime(const NumericalScalar timestamp,
                             const NumericalPoint & val);

#else
  /** Accessor to values */
  NumericalPoint getValueAtIndex(const UnsignedLong index);
  NumericalPoint getValueAtIndex(const UnsignedLong index) const;
  void setValueAtIndex(const UnsignedLong index, const NumericalPoint & val);

  NumericalPoint getValueAtNearestTime(const NumericalScalar timestamp);
  NumericalPoint getValueAtNearestTime(const NumericalScalar timestamp) const;
  void setValueAtNearestTime(const NumericalScalar timestamp, const NumericalPoint & val);

#endif


  /** Virtual constructor */
  TimeSeriesImplementation * clone() const;

  /** Description Accessor */
  void setDescription(const Description & description);
  Description getDescription() const;

  inline RegularGrid getTimeGrid() const
  {
    if (n_ == 0) throw InternalException(HERE) << "Error: no time grid for empty time series.";
    return RegularGrid( start_, timeStep_, n_ );
  }

  /**
   * String converter
   * This method shows human readable information on the
   * internal state of an TimeSeriesImplementation. It is used when streaming
   * the TimeSeriesImplementation or for user information.
   */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  inline Bool __eq__(const TimeSeriesImplementation & rhs) const
  {
    return (*this == rhs);
  }

  /* Method contains() is for Python */
  Bool contains(const NumericalPoint & val) const;

  /** Size accessor */
  inline UnsignedLong getSize() const
  {
    return n_;
  }

  /** Dimension accessor */
  inline UnsignedLong getDimension() const
  {
    return dimension_;
  }

  /** Append an element to the collection */
  TimeSeriesImplementation & add(const NumericalPoint & point);

  /** Append a sample to the collection */
  TimeSeriesImplementation & add(const NumericalSample & sample);

  /** Append another time series to the collection. The time grids must match (one follows the other) */
  TimeSeriesImplementation & add(const TimeSeriesImplementation & continuer);

  /** Return the values stored in the time series as a sample */
  NumericalSample getSample() const;
  NumericalSample getNumericalSample() const;

  /** Return the time series as a sample, ie its values and time grid */
  NumericalSample asSample() const;
  NumericalSample asNumericalSample() const;

  /** Compute the temporal mean of the time series */
  NumericalPoint getTemporalMean() const;

  /** Draw a marginal of the timeSerie */
  Graph drawMarginal(const UnsignedLong index = 0) const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);


private:

  /** The start time of the time series */
  NumericalScalar start_;

  /** The interval of the underlying regular time grid */
  NumericalScalar timeStep_;

  /** The number of timestamps of the underlying regular time grid */
  UnsignedLong n_;

  /** The dimension of the time series */
  UnsignedLong dimension_;

  /** The collection of unwrapped points */
  PersistentCollection<NumericalScalar> data_;

  /** The description of all components */
  Description::Implementation p_description_;

}; /* class TimeSeriesImplementation */


/* Comparison function */
Bool operator ==(const TimeSeriesImplementation & lhs, const TimeSeriesImplementation & rhs);


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_TIMESERIESIMPLEMENTATION_HXX */
