//                                               -*- C++ -*-
/**
 *  @file  Interval.hxx
 *  @brief Interval is defined as the cartesian product of n 1D intervalls ]low_1, up_1]x...x]low_n,up_n]
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 *  Id      Interval.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_INTERVAL_HXX
#define OPENTURNS_INTERVAL_HXX

#include "PersistentObject.hxx"
#include "Collection.hxx"
#include "PersistentCollection.hxx"
#include "NumericalPoint.hxx"
#include "DomainImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Interval
 *
 * A class that holds a collection of interval
 */
class Interval
  : public DomainImplementation
{
  CLASSNAME;

public:
  typedef Collection<UnsignedLong>           BoolCollection;
  typedef PersistentCollection<UnsignedLong> BoolPersistentCollection;


  /** Default constructor */
  explicit Interval(const UnsignedLong dimension = 1);

  /** Parameters constructor, simplified for 1D case */
  Interval(const NumericalScalar lowerBound,
           const NumericalScalar upperBound);

  /** Parameters constructor */
  Interval(const NumericalPoint & lowerBound,
           const NumericalPoint & upperBound);

  /** Parameters constructor */
  Interval(const NumericalPoint & lowerBound,
           const NumericalPoint & upperBound,
           const BoolCollection & finiteLowerBound,
           const BoolCollection & finiteUpperBound);

  /** Virtual constructor method */
  Interval * clone() const;

  /** Check if the interval is empty, i.e. if we have lowerBound >= upperBound for at least one component */
  Bool isEmpty() const;

  /** Check if the interval is numerically empty, i.e. if we have lowerBound >= upperBound for at least one component */
  Bool isNumericallyEmpty() const;

  /** Check if the given point is inside of the closed interval */
  Bool contains(const NumericalPoint & point) const;

  /** Check if the given point is numerically inside of the closed interval, i.e. using only the bounds part of the interval */
  Bool numericallyContains(const NumericalPoint & point) const;

  /** Get the numerical volume of the interval */
  NumericalScalar getNumericalVolume() const;

  /** Get the dimension of the interval */
  UnsignedLong getDimension() const;

  /** Returns the interval equals to the intersection between the interval and another one */
  Interval intersect(const Interval & other) const;

  /** Returns the interval equals to the union between the interval and another one */
  Interval join(const Interval & other) const;

  /** Addition operator */
  Interval operator +(const Interval & rhs) const;

  /** Substraction operator */
  Interval operator -(const Interval & rhs) const;

  /** In-place addition operator */
  Interval & operator +=(const Interval & other);

  /** In-place substraction operator */
  Interval & operator -=(const Interval & other);

  /** Product operator */
  Interval operator *(const NumericalScalar scalar) const;

  /**  In-place product operator */
  Interval & operator *=(const NumericalScalar scalar);

  /** Comparison operator */
  Bool operator == (const Interval & rhs) const ;

  /** Lower bound accessor */
  NumericalPoint getLowerBound() const;
  void setLowerBound(const NumericalPoint & lowerBound);

  /** Upper bound accessor */
  NumericalPoint getUpperBound() const;
  void setUpperBound(const NumericalPoint & upperBound);

  /** Lower bound flag accessor */
  BoolCollection getFiniteLowerBound() const;
  void setFiniteLowerBound(const BoolCollection & finiteLowerBound);

  /** Upper bound flag accessor */
  BoolCollection getFiniteUpperBound() const;
  void setFiniteUpperBound(const BoolCollection & finiteUpperBound);

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

private:

  // An n-D intervall is defined as the cartesian product of n 1D intervalls ]low_1, up_1]x...x]low_n,up_n]
  NumericalPoint lowerBound_;
  NumericalPoint upperBound_;

  // Flags that tell if the bounds are finite or not.
  // The bound is finite if the flag is true, else it is equal to -inf if the associated value is < 0, +inf else
  BoolPersistentCollection finiteLowerBound_;
  BoolPersistentCollection finiteUpperBound_;

}; /* class Interval */

} /* Namespace OpenTURNS */

#endif /* OPENTURNS_INTERVAL_HXX */
