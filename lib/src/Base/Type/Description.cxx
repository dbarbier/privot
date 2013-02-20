//                                               -*- C++ -*-
/**
 *  @file  Description.cxx
 *  @brief Description is a collection of string for human usage
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
#include "Description.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Description);

/* Default constructor */
Description::Description()
  : TypedCollectionInterfaceObject<DescriptionImplementation>(new DescriptionImplementation)
{
  // Nothing to do
}


/* Constructor with size */
Description::Description(const UnsignedLong size)
  : TypedCollectionInterfaceObject<DescriptionImplementation>(new DescriptionImplementation(size))
{
  // Nothing to do
}


/* Constructor with value */
Description::Description(const String & value)
  : TypedCollectionInterfaceObject<DescriptionImplementation>(new DescriptionImplementation(value))
{
  // Nothing to do
}


/* Constructor with size and default value */
Description::Description(const UnsignedLong size,
                         const String & value)
  : TypedCollectionInterfaceObject<DescriptionImplementation>(new DescriptionImplementation(size, value))
{
  // Nothing to do
}

/* Constructor with size and default value */
Description::Description(const Collection<String> & coll)
  : TypedCollectionInterfaceObject<DescriptionImplementation>(new DescriptionImplementation(coll))
{
  // Nothing to do
}

/* Constructor from implementation */
Description::Description(const DescriptionImplementation & implementation)
  : TypedCollectionInterfaceObject<DescriptionImplementation>(implementation.clone())
{
  // Nothing to do
}

/* Destructor */
Description::~Description()
{
  // Nothing to do
}


void Description::sort()
{
  copyOnWrite();
  std::sort(begin(), end());
}

END_NAMESPACE_OPENTURNS



