//                                               -*- C++ -*-
/**
 *  @file  Domain.cxx
 *  @brief Domain is the interface of DomainImplementation.cxx
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
#include "Domain.hxx"
#include "PersistentObjectFactory.hxx"
#include "Interval.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Domain);

/* Default constructor */
Domain::Domain():
  TypedInterfaceObject<DomainImplementation>(new Interval())
{
  // Nothing to do
}

Domain::Domain(const NumericalPoint & a,
               const NumericalPoint & b):
  TypedInterfaceObject<DomainImplementation>(new Interval(a, b))
{
  // Nothing to do
}

/* Default constructor */
Domain::Domain(const DomainImplementation & implementation):
  TypedInterfaceObject<DomainImplementation>(implementation.clone())
{
  // Nothing to do
}

UnsignedLong Domain::getDimension() const
{
  return getImplementation()->getDimension();
}

/* Check if the given point is inside of the closed interval */
Bool Domain::contains(const NumericalPoint & point) const
{
  return getImplementation()->contains(point);
}

END_NAMESPACE_OPENTURNS
