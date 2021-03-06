//                                               -*- C++ -*-
/**
 *  @file  EnumerateFunction.cxx
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
#include "EnumerateFunction.hxx"
#include "LinearEnumerateFunction.hxx"
#include "HyperbolicAnisotropicEnumerateFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(EnumerateFunction);

/* Constructor with parameters */
EnumerateFunction::EnumerateFunction(const EnumerateFunctionImplementation & implementation)
  : TypedInterfaceObject<EnumerateFunctionImplementation>(implementation.clone())
{
  // Nothing to do
}

/* Constructor with parameters */
EnumerateFunction::EnumerateFunction(Implementation & p_implementation)
  : TypedInterfaceObject<EnumerateFunctionImplementation>(p_implementation)
{
  // Nothing to do
}

/* Parameter constructor */
EnumerateFunction::EnumerateFunction(const UnsignedLong dimension)
  : TypedInterfaceObject<EnumerateFunctionImplementation>(new LinearEnumerateFunction(dimension))
{
  // Nothing to do
}

/* Parameter constructor */
EnumerateFunction::EnumerateFunction(const UnsignedLong dimension,
                                     const NumericalScalar q)
  : TypedInterfaceObject<EnumerateFunctionImplementation>(new HyperbolicAnisotropicEnumerateFunction(dimension, q))
{
  // Nothing to do
}

/* Parameter constructor */
EnumerateFunction::EnumerateFunction(const NumericalPoint & weight,
                                     const NumericalScalar q)
  : TypedInterfaceObject<EnumerateFunctionImplementation>(new HyperbolicAnisotropicEnumerateFunction(weight, q))
{
  // Nothing to do
}

/* String converter */
String EnumerateFunction::__repr__() const
{
  return getImplementation()->__repr__();
}

String EnumerateFunction::__str__(const String & offset) const
{
  return getImplementation()->__str__(offset);
}

/* The bijective association between an integer and a set of indices */
Indices EnumerateFunction::operator() (const UnsignedLong index) const
{
  return getImplementation()->operator()( index );
}

/* The inverse of the association */
UnsignedLong EnumerateFunction::inverse(const Indices & indices) const
{
  return getImplementation()->inverse( indices );
}

/* The cardinal of the given strata */
UnsignedLong EnumerateFunction::getStrataCardinal(const UnsignedLong strataIndex) const
{
  return getImplementation()->getStrataCardinal( strataIndex );
}

/* The cardinal of the cumulated strata above or equal to the given strata */
UnsignedLong EnumerateFunction::getStrataCumulatedCardinal(const UnsignedLong strataIndex) const
{
  return getImplementation()->getStrataCumulatedCardinal( strataIndex );
}

/* Dimension accessor */
void EnumerateFunction::setDimension(const UnsignedLong dimension)
{
  copyOnWrite();
  getImplementation()->setDimension( dimension );
}

UnsignedLong EnumerateFunction::getDimension() const
{
  return getImplementation()->getDimension();
}

END_NAMESPACE_OPENTURNS
