//                                               -*- C++ -*-
/**
 *  @file  FilteringWindowsImplementation.cxx
 *  @brief
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include "FilteringWindows.hxx"
#include "PersistentObjectFactory.hxx"
#include "Hamming.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(FilteringWindows);

/* Constructor with parameters */
FilteringWindows::FilteringWindows()
  : TypedInterfaceObject<FilteringWindowsImplementation>(new Hamming())
{
  // Nothing to do
}

/* Parameters constructor */
FilteringWindows::FilteringWindows(const FilteringWindowsImplementation & implementation,
                                   const String & name)
  : TypedInterfaceObject<FilteringWindowsImplementation>(implementation.clone())
{
  getImplementation()->setName(name);
}

/* Constructor from implementation */
FilteringWindows::FilteringWindows(const Implementation & p_implementation,
                                   const String & name)
  : TypedInterfaceObject<FilteringWindowsImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* Constructor from implementation pointer */
FilteringWindows::FilteringWindows(FilteringWindowsImplementation * p_implementation,
                                   const String & name)
  : TypedInterfaceObject<FilteringWindowsImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* functor - evaluation of filter using () */
NumericalScalar FilteringWindows::operator()(const NumericalScalar t) const
{
  return getImplementation()->operator()(t);
}

/* String converter */
String FilteringWindows::__repr__() const
{
  OSS oss;
  oss << "class=" << FilteringWindows::GetClassName()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}

/* String converter */
String FilteringWindows::__str__(const String & offset) const
{
  OSS oss;
  oss << "class=" << FilteringWindows::GetClassName()
      << " implementation=" << getImplementation()->__str__(offset);
  return oss;
}

END_NAMESPACE_OPENTURNS
