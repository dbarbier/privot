//                                               -*- C++ -*-
/**
 *  @file  NumericalPoint.cxx
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
 */
#include <cmath>
#include "NumericalPoint.hxx"
#include "Exception.hxx"
#include "StorageManager.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(NumericalPoint);

static Factory<PersistentCollection<NumericalScalar> > RegisteredFactory_alt1("PersistentCollection<NumericalScalar>");

static Factory<NumericalPoint> RegisteredFactory("NumericalPoint");

/* Default constructor */
NumericalPoint::NumericalPoint()
  : PersistentCollection<NumericalScalar>() //,
    // p_description_()
{
  // Nothing to do
}

/* Constructor with size */
NumericalPoint::NumericalPoint(const UnsignedLong size,
                               const NumericalScalar value)
  : PersistentCollection<NumericalScalar>(size, value)
{
  // Nothing to do
}



/* Constructor from a collection */
NumericalPoint::NumericalPoint(const Collection<NumericalScalar> & coll)
  : PersistentCollection<NumericalScalar>(coll)
{
  // Nothing to do
}


/* Constructor from indices */
NumericalPoint::NumericalPoint(const Indices & indices)
  : PersistentCollection<NumericalScalar>(indices.begin(), indices.end())
{
  // Nothing to do
}



/* Virtual constructor */
NumericalPoint * NumericalPoint::clone() const
{
  return new NumericalPoint(*this);
}


/* Coordinate accessor */
NumericalScalar & NumericalPoint::operator[](const UnsignedLong index)
{
  return PersistentCollection<NumericalScalar>::operator[](index);
}

/* Coordinate accessor */
const NumericalScalar & NumericalPoint::operator[](const UnsignedLong index) const
{
  return PersistentCollection<NumericalScalar>::operator[](index);
}


/* String converter */
String NumericalPoint::__repr__() const
{
  return OSS() << "class=" << NumericalPoint::GetClassName()
               << " name=" << getName()
               << " dimension=" << getDimension()
               << " values=" << PersistentCollection<NumericalScalar>::__repr__();
}

String NumericalPoint::__str__(const String & offset) const
{
  return PersistentCollection<NumericalScalar>::__str__();
}






/* Erase the elements between first and last */
NumericalPoint::iterator NumericalPoint::erase(iterator first, iterator last)
{
  return PersistentCollection<NumericalScalar>::erase(first, last);
}

/* Erase the element pointed by position */
NumericalPoint::iterator NumericalPoint::erase(iterator position)
{
  return PersistentCollection<NumericalScalar>::erase(position);
}

/* Erase the element pointed by position */
NumericalPoint::iterator NumericalPoint::erase(UnsignedLong position)
{
  return PersistentCollection<NumericalScalar>::erase(begin() + position);
}



/* Addition operator */
NumericalPoint operator + (const NumericalPoint & lhs, const NumericalPoint & rhs)
{
  if (lhs.getDimension() != rhs.getDimension())
    throw InvalidArgumentException(HERE)
      << "NumericalPoints of different dimensions cannot be added (LHS dimension = "
      << lhs.getDimension()
      << "; RHS dimension = "
      << rhs.getDimension();

  // We create a NumericalPoint of the same dimension as both points for holding the result
  NumericalPoint result(lhs);
  result += rhs;
  return result;
}



/* In-place addition operator */
NumericalPoint & NumericalPoint::operator +=(const NumericalPoint & other)
{
  if (getDimension() != other.getDimension())
    throw InvalidArgumentException(HERE)
      << "NumericalPoints of different dimensions cannot be added (LHS dimension = "
      << getDimension()
      << "; RHS dimension = "
      << other.getDimension();

  for (UnsignedLong i = 0; i < getDimension(); ++i) operator[](i) += other[i];
  return *this;
}



/* Substraction operator */
NumericalPoint operator - (const NumericalPoint & lhs, const NumericalPoint & rhs)
{
  if (lhs.getDimension() != rhs.getDimension())
    throw InvalidArgumentException(HERE)
      << "NumericalPoints of different dimensions cannot be substracted (LHS dimension = "
      << lhs.getDimension()
      << "; RHS dimension = "
      << rhs.getDimension();

  // We create a NumericalPoint of the same dimension as both points for holding the result
  NumericalPoint result(lhs);
  result -= rhs;
  return result;
}



/* In-place substraction operator */
NumericalPoint & NumericalPoint::operator -=(const NumericalPoint & other)
{
  if (getDimension() != other.getDimension())
    throw InvalidArgumentException(HERE)
      << "NumericalPoints of different dimensions cannot be substracted (LHS dimension = "
      << getDimension()
      << "; RHS dimension = " <<
      other.getDimension();

  for (UnsignedLong i = 0; i < getDimension(); ++i) (*this)[i] -= other[i];
  return *this;
}



/* Product operator */
NumericalPoint operator *(const NumericalPoint & point,
                          const NumericalScalar scalar)
{
  // We create a NumericalPoint of the same dimension as both points for holding the result
  NumericalPoint result(point.getDimension());
  for (UnsignedLong i = 0; i < point.getDimension(); ++i) result[i] = point[i] * scalar;
  return result;
}

NumericalPoint operator *(const NumericalScalar scalar,
                          const NumericalPoint & point)
{
  return point * scalar;
}

/*  In-place product operator */
NumericalPoint & NumericalPoint::operator *=(const NumericalScalar scalar)
{
  for (UnsignedLong i = 0; i < getDimension(); ++i) (*this)[i] *= scalar;
  return *this;
}

/* Division operator */
NumericalPoint operator /(const NumericalPoint & point,
                          const NumericalScalar scalar)
{
  if (scalar == 0.0) throw InvalidArgumentException(HERE) << "Error: cannot divide by 0.";
  // We create a NumericalPoint of the same dimension as both points for holding the result
  NumericalPoint result(point.getDimension());
  for (UnsignedLong i = 0; i < point.getDimension(); ++i) result[i] = point[i] / scalar;
  return result;
}

/*  In-place division operator */
NumericalPoint & NumericalPoint::operator /=(const NumericalScalar scalar)
{
  if (scalar == 0.0) throw InvalidArgumentException(HERE) << "Error: cannot divide by 0.";
  for (UnsignedLong i = 0; i < getDimension(); ++i) (*this)[i] /= scalar;
  return *this;
}


/* Dot product operator */
NumericalScalar dot(const NumericalPoint & lhs,
                    const NumericalPoint & rhs)
{
  if (lhs.getDimension() != rhs.getDimension())
    throw InvalidArgumentException(HERE)
      << "NumericalPoints of different dimensions cannot be added (LHS dimension = "
      << lhs.getDimension()
      << "; RHS dimension = "
      << rhs.getDimension();

  NumericalScalar dotProduct(0.);
  for (UnsignedLong i = 0; i < lhs.getDimension(); ++i) dotProduct += lhs[i] * rhs[i];
  return dotProduct;
}


/* Comparison operator */
Bool operator ==(const NumericalPoint & lhs,
                 const NumericalPoint & rhs)
{
  return static_cast<const PersistentCollection<NumericalScalar> >(lhs) == static_cast<const PersistentCollection<NumericalScalar> >(rhs);
}



/* Ordering operator */
Bool operator <(const NumericalPoint & lhs,
                const NumericalPoint & rhs)
{
  return static_cast<const PersistentCollection<NumericalScalar> >(lhs) < static_cast<const PersistentCollection<NumericalScalar> >(rhs);
}



/*  Norm */
NumericalScalar NumericalPoint::norm() const
{
  return sqrt( norm2() );
}


/*  Norm^2 */
NumericalScalar NumericalPoint::norm2() const
{
  return dot(*this, *this);
}

/*  Normalize */
NumericalPoint NumericalPoint::normalize() const
{
  const NumericalScalar theNorm(norm());
  if (theNorm == 0.0) throw InternalException(HERE) << "Error: cannot normalize a null vector";
  return (*this) / theNorm;
}

/*  Square normalize */
NumericalPoint NumericalPoint::normalize2() const
{
  const NumericalScalar theNorm2(norm2());
  if (theNorm2 == 0.0) throw InternalException(HERE) << "Error: cannot square normalize a null vector";
  NumericalPoint result(getDimension());
  for (UnsignedLong i = 0; i < getDimension(); ++i) result[i] = pow((*this)[i], 2) / theNorm2;
  return result;
}

/* Method save() stores the object through the StorageManager */
void NumericalPoint::save(Advocate & adv) const
{
  PersistentCollection<NumericalScalar>::save(adv);
}


/* Method load() reloads the object from the StorageManager */
void NumericalPoint::load(Advocate & adv)
{
  PersistentCollection<NumericalScalar>::load(adv);
}


END_NAMESPACE_OPENTURNS
