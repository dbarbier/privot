//                                               -*- C++ -*-
/**
 *  @file  ARMACoefficients.cxx
 *  @brief ARMACoefficients class enables to stock coefficients of an ARMA process
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#include "PersistentObjectFactory.hxx"
#include "ARMACoefficients.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

TEMPLATE_CLASSNAMEINIT(PersistentCollection< ARMACoefficients >);
TEMPLATE_CLASSNAMEINIT(PersistentCollection< SquareMatrix >);
static Factory< PersistentCollection< SquareMatrix > > RegisteredFactory1("PersistentCollection<SquareMatrix>");




CLASSNAMEINIT(ARMACoefficients);
static Factory<ARMACoefficients> RegisteredFactory("ARMACoefficients");

/* Default constructor */
ARMACoefficients::ARMACoefficients(const UnsignedLong & size,
                                   const UnsignedLong & dimension)
  : PersistentCollection<SquareMatrix>(size, SquareMatrix(dimension) ),
    dimension_(dimension)
{
  if (dimension == 0) throw InvalidArgumentException(HERE) << "Error: dimension should be at least 1";
}


/* Standard constructor */
ARMACoefficients::ARMACoefficients(const SquareMatrixCollection & collection)
  : PersistentCollection<SquareMatrix>(0)
{
  // Adding elements one by one and checking coherance of dimension
  const UnsignedLong collectionSize(collection.getSize());
  if (collectionSize == 0) throw InvalidArgumentException(HERE) << "Error: cannot build an ARMACoefficients object based on an empty collection of matrices.";
  dimension_ = collection[0].getDimension();
  add(collection[0]);
  for (UnsignedLong i = 1; i < collectionSize; ++i)
    if (collection[i].getDimension() == dimension_)
      add(collection[i]);
}

/* Default constructor */
ARMACoefficients::ARMACoefficients(const NumericalPoint & scalarCoefficients)
  : PersistentCollection<SquareMatrix>(scalarCoefficients.getSize(), SquareMatrix(1))
{
  dimension_ = 1;
  for (UnsignedLong i = 0 ; i < getSize() ; ++i ) (*this)[i](0, 0) = scalarCoefficients[i];
}

/* constructor using polynomial */
ARMACoefficients::ARMACoefficients(const UniVariatePolynomial & polynomial)
  : PersistentCollection<SquareMatrix>(polynomial.getDegree() + 1, SquareMatrix(1))
{
  dimension_ = 1;
  const NumericalPoint coefficients(polynomial.getCoefficients());
  for (UnsignedLong i = 0 ; i < getSize() ; ++i ) (*this)[i](0, 0) = coefficients[i];
}

/* Virtual constructor  - clone*/
ARMACoefficients * ARMACoefficients::clone() const
{
  return new ARMACoefficients(*this);
}

/* String converter */
String ARMACoefficients::__repr__() const
{
  OSS oss;
  oss << "class=" << ARMACoefficients::GetClassName();
  for (UnsignedLong i = 0; i < getSize(); ++i) oss << ", shift=" << i << ", value=" << (*this)[i];
  return oss;
}

String ARMACoefficients::__str__(const String & offset) const
{
  OSS oss;
  for (UnsignedLong i = 0; i < getSize(); ++i) oss << offset << "shift = " << i << "\n" << offset << (*this)[i].__str__(offset) << "\n";
  return oss;
}

/* Dimension accessor */
UnsignedLong  ARMACoefficients::getDimension() const
{
  return dimension_;
}

/** Redefinition of add method : control of SquareMatrix sizes */
void ARMACoefficients::add(const SquareMatrix & matrix)
{
  if (matrix.getDimension() != dimension_)
    throw InvalidArgumentException(HERE) << "Could not add the coefficient. Incompatible dimension with the elements of collection";
  PersistentCollection<SquareMatrix>::add(matrix);
}


/* Method save() stores the object through the StorageManager */
void ARMACoefficients::save(Advocate & adv) const
{
  PersistentCollection<SquareMatrix>::save( adv );
}

/* Method load() reloads the object from the StorageManager */
void ARMACoefficients::load(Advocate & adv)
{
  PersistentCollection<SquareMatrix>::load( adv );
}

END_NAMESPACE_OPENTURNS

