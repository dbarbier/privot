//                                               -*- C++ -*-
/**
 *  @file  DomainImplementation.hxx
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
 *  Id      DomainImplementation.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_DOMAINIMPLEMENTATION_HXX
#define OPENTURNS_DOMAINIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "NumericalPoint.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class DomainImplementation
 *
 * A class that holds a collection of domain
 */
class DomainImplementation
  : public PersistentObject
{
  CLASSNAME;

public:
  typedef Pointer<DomainImplementation>            Implementation;

  /** Default constructor */
  DomainImplementation(const UnsignedLong dimension = 1);

  /** Virtual constructor method */
  virtual DomainImplementation * clone() const;

  /** Check if the closed domain contains a given point */
  virtual Bool contains(const NumericalPoint & point) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Dimension accessors */
  UnsignedLong getDimension() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:
  /** The dimension of the DomainImplementation */
  UnsignedLong dimension_;

}; /* class DomainImplementation */

} /* Namespace OpenTURNS */

#endif /* OPENTURNS_DOMAINIMPLEMENTATION_HXX */
