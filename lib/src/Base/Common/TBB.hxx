//                                               -*- C++ -*-
/**
 *  @file  TBB.hxx
 *  @brief This file supplies support for multithreading
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
 *  @date   2007-05-10 16:43:31 +0200 (Thu, 10 May 2007)
 *  Id      AtomicFunctions.hxx 434 2007-05-10 14:43:31Z dutka
 */
#ifndef OPENTURNS_TBB_HXX
#define OPENTURNS_TBB_HXX

#include "OTprivate.hxx"
#include "OTconfig.hxx"
#include <algorithm>

#ifdef HAVE_TBB
#  include "tbb/tbb.h"

#else /* HAVE_TBB */
#include "Exception.hxx"
// We redefine some TBB elements to simulate TBB availability through the code
// Those redefinitions are single threaded
namespace tbb
{

  template <typename T>
  class blocked_range
  {
  public:
    typedef T value_type;
    typedef std::size_t size_type;
    blocked_range(value_type from, value_type to, size_type gs = 1)
      : from_(from), to_(to), grainSize_(gs)
    {
#ifdef DEBUG_BOUNDCHECKING
      if (from_ > to_)
        throw OT::InvalidArgumentException(HERE) << "Range is malformed (from > to) with from=" << from_
                                                 << " and to=" << to;
      if (grainSize_ < 1)
        throw OT::InvalidArgumentException(HERE) << "Range is malformed (grainSize < 1) with grainSize=" << grainSize_;
#endif /* DEBUG_BOUNDCHECKING */
    }
    value_type begin() const
    {
      return from_;
    }
    value_type end() const
    {
      return to_;
    }
    size_type size() const
    {
      return size_t(to_ - from_);
    }
    size_type grainsize() const
    {
      return grainSize_;
    }
  private:
    value_type from_;
    value_type to_;
    size_type grainSize_;
  }; // end class blocked_range

  template <typename RANGE, typename BODY>
  void parallel_for( const RANGE & range, const BODY & body )
  {
    body( range );
  }

  template <typename RANGE, typename BODY>
  void parallel_reduce( const RANGE & range, BODY & body )
  {
    body( range );
  }

  template <typename ITERATOR>
  void parallel_sort( ITERATOR first, ITERATOR last )
  {
    std::stable_sort( first, last );
  }

} // namespace tbb
#endif /* HAVE_TBB */

BEGIN_NAMESPACE_OPENTURNS

class TBB
{
public:
#ifdef HAVE_TBB
  typedef tbb::split Split;
#else /* HAVE_TBB */
  struct Split {};
#endif /* HAVE_TBB */

  template <typename T>
  struct BlockedRange : public tbb::blocked_range<T>
  {
    typedef T value_type;
    typedef typename tbb::blocked_range<T>::size_type size_type;
    BlockedRange(value_type from, value_type to, size_type gs = 1) : tbb::blocked_range<T>(from, to, gs) {}
    BlockedRange(const tbb::blocked_range<T> & range) : tbb::blocked_range<T>(range) {}
  };

private:

  /** Start the machinery of multithreading */
  static void Initialization();

  /** End the machinery of multithreading */
  static void Release();

public:

  template <typename BODY>
  static inline
  void ParallelFor( UnsignedLong from, UnsignedLong to, const BODY & body, std::size_t gs = 1 )
  {
    tbb::parallel_for( tbb::blocked_range<UnsignedLong>( from, to, gs ), body );
  }

  template <typename BODY>
  static inline
  void ParallelReduce( UnsignedLong from, UnsignedLong to, BODY & body, std::size_t gs = 1)
  {
    tbb::parallel_reduce( tbb::blocked_range<UnsignedLong>( from, to, gs ), body );
  }

  template <typename ITERATOR>
  static inline
  void ParallelSort( ITERATOR first, ITERATOR last )
  {
    tbb::parallel_sort( first, last );
  }

private:

  friend struct TBB_init; /* friendship for static member initialization */
}; /* end class TBB */



/** This struct initializes all static members of TBB */
struct TBB_init
{
  TBB_init();
  ~TBB_init();
}; /* end class TBB_init */

static TBB_init __TBB_initializer;

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_TBB_HXX */
