//                                               -*- C++ -*-
/**
 *  @file  ComparisonOperatorImplementation.cxx
 *  @brief Abstract top-level class for all ComparisonOperatorImplementation
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
#include "ComparisonOperatorImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(ComparisonOperatorImplementation);

/* Default constructor */
ComparisonOperatorImplementation::ComparisonOperatorImplementation()
  : PersistentObject()
{
  // Nothing to do
}

/* String converter */
String ComparisonOperatorImplementation::__repr__() const
{
  return OSS() << "class=" << ComparisonOperatorImplementation::GetClassName()
               << " name=" << getName();
}


/* Here is the interface that all derived class must implement */

/* Method save() stores the object through the StorageManager */
void ComparisonOperatorImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void ComparisonOperatorImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
}


END_NAMESPACE_OPENTURNS
