//                                               -*- C++ -*-
/**
 *  @file  Domain.hxx
 *  @brief Domain is the interface of DomainImplementation
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
 *  Id      Domain.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_DOMAIN_HXX
#define OPENTURNS_DOMAIN_HXX

#include "TypedInterfaceObject.hxx"
#include "DomainImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Domain
 *
 * A class that holds a domain
 */
class Domain
  : public TypedInterfaceObject<DomainImplementation>
{
  CLASSNAME;

public:

  /** Default constructor */
  Domain();

  /** Copy-Standard constructors */
  Domain(const DomainImplementation & implementation);

  /** Standard constructor based on Interval(a,b) */
  Domain(const NumericalPoint & a,
         const NumericalPoint & b);

  /** Dimension accessors */
  virtual UnsignedLong getDimension() const;

  /** Check if the closed domain contains a given point */
  Bool contains(const NumericalPoint & point) const;

}; /* class Domain */

} /* Namespace OpenTURNS */

#endif /* OPENTURNS_DOMAIN_HXX */
