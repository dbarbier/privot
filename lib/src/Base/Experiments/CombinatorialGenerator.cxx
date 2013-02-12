//                                               -*- C++ -*-
/**
 *  @file  CombinatorialGenerator.cxx
 *  @brief Abstract top-level view of an combinatorialGenerator plane
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
 */
#include "CombinatorialGenerator.hxx"
#include "Tuples.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(CombinatorialGenerator);

/* Default constructor */
CombinatorialGenerator::CombinatorialGenerator()
  : TypedInterfaceObject<CombinatorialGeneratorImplementation>(Tuples().clone())
{
  // Nothing to do
}

/* Constructor from implementation pointer */
CombinatorialGenerator::CombinatorialGenerator(const Implementation & p_implementation)
  : TypedInterfaceObject<CombinatorialGeneratorImplementation>(p_implementation)
{
  // Nothing to do
}

/* Constructor from implementation */
CombinatorialGenerator::CombinatorialGenerator(const CombinatorialGeneratorImplementation & implementation)
  : TypedInterfaceObject<CombinatorialGeneratorImplementation>(implementation.clone())
{
  // Nothing to do
}

/* String converter */
String CombinatorialGenerator::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName ()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}

void CombinatorialGenerator::setImplementation(const Implementation & p_implementation)
{
  copyOnWrite();
  getImplementation() = p_implementation;
}

/* Sample generation */
CombinatorialGenerator::IndicesCollection CombinatorialGenerator::generate()
{
  return getImplementation()->generate();
}

END_NAMESPACE_OPENTURNS
