//                                               -*- C++ -*-
/**
 *  @file  EnumerateFunctionImplementation.cxx
 *  @brief The bijective function to select polynomials in the orthogonal basis
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
 *  @author dutka
 *  @date   2008-05-21 11:21:38 +0200 (Wed, 21 May 2008)
 */
#include "EnumerateFunctionImplementation.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(EnumerateFunctionImplementation);


/* Parameter constructor */
EnumerateFunctionImplementation::EnumerateFunctionImplementation(const UnsignedLong dimension)
  : PersistentObject(),
    dimension_(dimension)
{
  if ( dimension == 0 )
    throw InvalidArgumentException ( HERE ) << "Cannot build an EnumerateFunction of dimension 0.";
}

/* String converter */
String EnumerateFunctionImplementation::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " dimension=" << dimension_;
}

/* The inverse of the association */
UnsignedLong EnumerateFunctionImplementation::inverse(const Indices & indices) const
{
  throw NotYetImplementedException( HERE );
}

/* Dimension accessor */
void EnumerateFunctionImplementation::setDimension(const UnsignedLong dimension)
{
  dimension_ = dimension;
}

UnsignedLong EnumerateFunctionImplementation::getDimension() const
{
  return dimension_;
}

/* Method save() stores the object through the StorageManager */
void EnumerateFunctionImplementation::save(Advocate & adv) const
{
  PersistentObject::save( adv );
  adv.saveAttribute( "dimension_", dimension_ );
}

/* Method load() reloads the object from the StorageManager */
void EnumerateFunctionImplementation::load(Advocate & adv)
{
  PersistentObject::load( adv );
  adv.loadAttribute( "dimension_", dimension_ );
}


END_NAMESPACE_OPENTURNS
