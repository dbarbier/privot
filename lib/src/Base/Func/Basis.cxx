//                                               -*- C++ -*-
/**
 *  @file  Basis.cxx
 *  @brief A collection of numerical math function
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
 *  @date   2010-11-09 13:44:00 +0100 (Tue, 09 Nov 2010)
 */

#include "PersistentObjectFactory.hxx"
#include "Basis.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Basis);

static Factory<Basis> RegisteredFactory("Basis");

/* Default constructor */
Basis::Basis()
  : PersistentCollection<NumericalMathFunction>()
{
  // Nothing to do
}

/* Constructor from a collection */
Basis::Basis(const Collection<NumericalMathFunction> & coll)
  : PersistentCollection<NumericalMathFunction>(coll)
{
  // Nothing to do
}

/* String converter */
String Basis::__repr__() const
{
  return OSS() << "class=" << GetClassName()
               << " coll=" << PersistentCollection<NumericalMathFunction>::__repr__();
}

String Basis::__str__(const String & offset) const
{
  return OSS(false) << "Basis( " << PersistentCollection<NumericalMathFunction>::__str__( offset ) << " )";
}

/* Dimension accessor */
UnsignedLong Basis::getDimension() const
{
  return (getSize() > 0) ? (*this)[0].getInputDimension() : 0;
}

/* Method save() stores the object through the StorageManager */
void Basis::save(Advocate & adv) const
{
  PersistentCollection<NumericalMathFunction>::save( adv );
}

/* Method load() reloads the object from the StorageManager */
void Basis::load(Advocate & adv)
{
  PersistentCollection<NumericalMathFunction>::load( adv );
}

END_NAMESPACE_OPENTURNS

