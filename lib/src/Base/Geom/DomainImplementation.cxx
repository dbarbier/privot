//                                               -*- C++ -*-
/**
 *  @file  DomainImplementation.cxx
 *  @brief DomainImplementation is defined as a domain of \mathbb{R}^d
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
#include "DomainImplementation.hxx"
#include "PersistentObjectFactory.hxx"
#include "Os.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(DomainImplementation);

static Factory<DomainImplementation> RegisteredFactory("DomainImplementation");


/* Default constructor */
DomainImplementation::DomainImplementation(UnsignedLong dimension):
  PersistentObject(),
  dimension_(dimension)
{
  // Nothing to do
}

/* Clone method */
DomainImplementation * DomainImplementation::clone() const
{
  return new DomainImplementation(*this);
}

/* String converter */
String DomainImplementation::__repr__() const
{
  return OSS() << "class = " << GetClassName()
               << " name = " << getName()
               << " dimension = " << dimension_;

}

String DomainImplementation::__str__(const String & offset) const
{
  return OSS () << offset << __repr__();
}

/* Check if the given point is inside of the closed interval */
Bool DomainImplementation::contains(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}

/**/
UnsignedLong DomainImplementation::getDimension() const
{
  return dimension_;
}

/* Method save() stores the object through the StorageManager */
void DomainImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute("dimension_", dimension_);
}

/* Method load() reloads the object from the StorageManager */
void DomainImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute("dimension_", dimension_);
}

} /* Namespace OpenTURNS */
