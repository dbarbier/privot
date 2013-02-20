//                                               -*- C++ -*-
/**
 *  @file  NumericalPoint.hxx
 *  @brief NumericalPoint implements the classical mathematical point
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
 *  Id      NumericalPoint.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_NUMERICALPOINT_HXX
#define OPENTURNS_NUMERICALPOINT_HXX

#include <vector>
#include "PersistentCollection.hxx"
#include "Description.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class NumericalPoint
 *
 * NumericalPoint implements the classical mathematical point
 */

class NumericalPoint
  : public PersistentCollection<NumericalScalar>
{
  CLASSNAME;

public:

  typedef PersistentCollection<NumericalScalar> InternalType;
  typedef Collection<NumericalScalar>           NumericalScalarCollection;
  typedef NumericalPoint                              ImplementationType;

  /** Default constructor */
  NumericalPoint();

  /** Constructor with size */
  NumericalPoint(const UnsignedLong size,
                 const NumericalScalar value = 0.0);

  /** Constructor from a collection */
  NumericalPoint(const Collection<NumericalScalar> & coll);

  /** Virtual constructor */
  virtual NumericalPoint * clone() const;

  /** Description Accessor */
  virtual void setDescription(const Description & description) {}
  virtual Description getDescription() const
  {
    return Description( getDimension() );
  }

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Dimension accessor */
  inline UnsignedLong getDimension() const
  {
    return PersistentCollection<NumericalScalar>::getSize();
  }
  inline UnsignedLong getSize() const
  {
    return PersistentCollection<NumericalScalar>::getSize();
  }

  /** Collection accessor */
  inline const NumericalScalarCollection & getCollection() const
  {
    return *this;
  }

#ifndef SWIG
  /** Coordinate accessor */
  NumericalScalar & operator[](const UnsignedLong index);
  const NumericalScalar & operator[](const UnsignedLong index) const;

  /** Erase the elements between first and last */
  iterator erase(iterator first, iterator last);

  /** Erase the element pointed by position */
  iterator erase(iterator position);

  /** Erase the element pointed by position */
  iterator erase(UnsignedLong position);

  /** In-place addition operator */
  NumericalPoint & operator +=(const NumericalPoint & other);

  /** In-place substraction operator */
  NumericalPoint & operator -=(const NumericalPoint & other);
#endif

  /**  In-place product operator */
  NumericalPoint & operator *=(const NumericalScalar scalar);

  /**  In-place division operator */
  NumericalPoint & operator /=(const NumericalScalar scalar);

  /**  Norm */
  NumericalScalar norm() const;

  /**  Norm^2 */
  NumericalScalar norm2() const;

  /** Normalize the vector */
  NumericalPoint normalize() const;

  /** Square normalize the vector */
  NumericalPoint normalize2() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

private:

  //      /** The description of all components */
  //      Description:: p_description_;

}; /* class NumericalPoint */


#ifndef SWIG
/** Comparison operator */
Bool operator == (const NumericalPoint & lhs,
                  const NumericalPoint & rhs);

/** Ordering operator */
Bool operator < (const NumericalPoint & lhs,
                 const NumericalPoint & rhs);

/** Product operator */
NumericalPoint operator * (const NumericalScalar scalar,
                           const NumericalPoint & point);

/** Product operator */
NumericalPoint operator * (const NumericalPoint & point,
                           const NumericalScalar scalar);

/** Division operator */
NumericalPoint operator / (const NumericalPoint & point,
                           const NumericalScalar scalar);

/** Addition operator */
NumericalPoint operator + (const NumericalPoint & lhs,
                           const NumericalPoint & rhs);

/** Substraction operator */
NumericalPoint operator - (const NumericalPoint & lhs,
                           const NumericalPoint & rhs);

#endif

/** Dot product operator */
NumericalScalar dot(const NumericalPoint & lhs,
                    const NumericalPoint & rhs);



END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NUMERICALPOINT_HXX */
