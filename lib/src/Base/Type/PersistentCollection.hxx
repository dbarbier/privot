//                                               -*- C++ -*-
/**
 *  @file  PersistentCollection.hxx
 *  @brief PersistentCollection defines top-most collection strategies
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
 *  Id      PersistentCollection.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_PERSISTENTCOLLECTION_HXX
#define OPENTURNS_PERSISTENTCOLLECTION_HXX

#include "PersistentObject.hxx"
#include "StorageManager.hxx"
#include "Collection.hxx"

BEGIN_NAMESPACE_OPENTURNS


/**
 * @class PersistentCollection
 *
 * PersistentCollection defines top-most collection strategies
 */

/* Forward declaration */
template <class T> class PersistentCollection;


/** Stream operator */
template <class T> inline
OStream & operator << (OStream & OS,
                       const PersistentCollection<T> & collection)
{
  return OS << collection.__repr__();
}

template <class T> inline
std::ostream & operator << (std::ostream & os,
                            const PersistentCollection<T> & collection)
{
  return os << collection.__str__();
}





template <class T>
class PersistentCollection
  : public PersistentObject,
    public Collection<T>
{
  CLASSNAME;

public:

  typedef Collection<T>                                 InternalType;
  typedef typename InternalType::ElementType            ElementType;
  typedef typename InternalType::ValueType              ValueType;
  typedef typename InternalType::iterator               iterator;
  typedef typename InternalType::const_iterator         const_iterator;
  typedef typename InternalType::reverse_iterator       reverse_iterator;
  typedef typename InternalType::const_reverse_iterator const_reverse_iterator;



  /** Default constructor */
  PersistentCollection()
    : PersistentObject(),
      Collection<T>()
  {
    // Nothing to do
  }

  /** Constructor from a collection */
  PersistentCollection(const Collection<T> & collection)
    : PersistentObject(),
      Collection<T>(collection)
  {
    // Nothing to do
  }

  /** Constructor that pre-allocate size elements */
  PersistentCollection(const UnsignedLong size)
    : PersistentObject(),
      Collection<T>(size)
  {
    // Nothing to do
  }

  /** Constructor that pre-allocate size elements with value */
  PersistentCollection(const UnsignedLong size,
                       const T & value)
    : PersistentObject(),
      Collection<T>(size, value)
  {
    // Nothing to do
  }

  /** Constructor from a range of elements */
  template <typename InputIterator>
  PersistentCollection(InputIterator first,
                       InputIterator last)
    : PersistentObject(),
      Collection<T>(first, last)
  {
    // Nothing to do
  }


  /** Virtual constructor */
  virtual PersistentCollection * clone() const
  {
    return new PersistentCollection(*this);
  }


  /** String converter */
  inline
  String __repr__() const
  {
    return Collection<T>::__repr__();
  }

  inline
  String __str__(const String & offset = "") const
  {
    return Collection<T>::__str__();
  }

  /** Method save() stores the object through the StorageManager */
  inline
  void save(Advocate & adv) const
  {
    PersistentObject::save(adv);
    adv.saveAttribute( "size", this->getSize() );
    std::copy( Collection<T>::coll__.begin(),
               Collection<T>::coll__.end(),
               AdvocateIterator< T >( adv ) );
  }

  /** Method load() reloads the object from the StorageManager */
  inline
  void load(Advocate & adv)
  {
    PersistentObject::load(adv);
    UnsignedLong size = 0;
    adv.loadAttribute( "size", size );
    Collection<T>::coll__.resize(size);
    std::generate( Collection<T>::coll__.begin(),
                   Collection<T>::coll__.end(),
                   AdvocateIterator< T >( adv ) );
  }


}; /* class PersistentCollection */


/** Comparison operator */
template <class T> inline
Bool operator == (const PersistentCollection<T> & lhs,
                  const PersistentCollection<T> & rhs)
{
  return static_cast<const Collection<T>& >(lhs) == static_cast<const Collection<T>& >(rhs);
}


/** Ordering operator */
template <class T> inline
Bool operator < (const PersistentCollection<T> & lhs,
                 const PersistentCollection<T> & rhs)
{
  return static_cast<const Collection<T>& >(lhs) < static_cast<const Collection<T>& >(rhs);
}


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PERSISTENTCOLLECTION_HXX */
