//                                               -*- C++ -*-
/**
 *  @file  OrthogonalFunctionFactory.cxx
 *  @brief This is an abstract class for orthogonal basis
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
#include "OrthogonalFunctionFactory.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(OrthogonalFunctionFactory);

static Factory<OrthogonalFunctionFactory> RegisteredFactory("OrthogonalFunctionFactory");


/* Default constructor */
OrthogonalFunctionFactory::OrthogonalFunctionFactory()
  : PersistentObject(),
    measure_()
{
  // Nothing to do
}


/* Virtual constructor */
OrthogonalFunctionFactory * OrthogonalFunctionFactory::clone() const
{
  return new OrthogonalFunctionFactory(*this);
}


/* Build the NumericalMathFunction of the given index */
NumericalMathFunction OrthogonalFunctionFactory::build(const UnsignedLong index) const
{
  throw NotYetImplementedException(HERE);
}


/* Return the measure upon which the basis is orthogonal */
Distribution OrthogonalFunctionFactory::getMeasure() const
{
  return measure_;
}

/* Return the enumerate function that translate unidimensional indices nto multidimensional indices */
EnumerateFunction OrthogonalFunctionFactory::getEnumerateFunction() const
{
  throw NotYetImplementedException(HERE);
}


/* String converter */
String OrthogonalFunctionFactory::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " measure=" << measure_;
}

/* Method save() stores the object through the StorageManager */
void OrthogonalFunctionFactory::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "measure_", measure_ );
  adv.saveAttribute( "enumerateFunction_", enumerateFunction_ );
}


/* Method load() reloads the object from the StorageManager */
void OrthogonalFunctionFactory::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "measure_", measure_ );
  adv.loadAttribute( "enumerateFunction_", enumerateFunction_ );
}


END_NAMESPACE_OPENTURNS
