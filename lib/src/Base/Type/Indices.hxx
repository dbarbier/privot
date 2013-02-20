//                                               -*- C++ -*-
/**
 *  @file  Indices.hxx
 *  @brief Set of indices defined as a Collection of UnsignedLong class
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
 *  Id      Indices.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_INDICES_HXX
#define OPENTURNS_INDICES_HXX

#include "OTprivate.hxx"
#include "PersistentCollection.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class Indices
 *
 * A class that holds a collection of indices
 */
class Indices :
  public PersistentCollection<UnsignedLong>
{
public:
  typedef PersistentCollection<UnsignedLong>   InternalType;

  /** Default constructor */
  Indices() : InternalType()
  {
    // Nothing to do
  }

  /** Constructor that pre-allocate size elements */
  explicit Indices(const UnsignedLong size) : InternalType(size)
  {
    // Nothing to do
  }

  /** Constructor that pre-allocate size elements with value */
  Indices(const UnsignedLong size,
          const UnsignedLong value) : InternalType(size, value)
  {
    // Nothing to do
  }

  /** Constructor from a range of elements */
  template <typename InputIterator>
  Indices(const InputIterator first,
          const InputIterator last) : InternalType(first, last)
  {
    // Nothing to do
  }

  /** Destructor */
  ~Indices() throw() {}

  /** Check that no value is repeated and no value exceed the given bound */
  Bool check(const UnsignedLong bound) const;

  /** Fill the indices with a linear progression, starting from start value by step stepsize */
  void fill(const UnsignedLong initialValue = 0,
            const UnsignedLong stepSize = 1);
#ifdef SWIG
  /** @copydoc Object::__repr__() const */
  virtual String __repr__() const;

  /** @copydoc Object::__str__() const */
  virtual String __str__(const String & offset = "") const;
#endif

}; /* class Indices */

} /* Namespace OpenTURNS */

#endif /* OPENTURNS_INDICES_HXX */
