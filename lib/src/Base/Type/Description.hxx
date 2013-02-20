//                                               -*- C++ -*-
/**
 *  @file  Description.hxx
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
 *  Id      Description.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_DESCRIPTION_HXX
#define OPENTURNS_DESCRIPTION_HXX

#include "TypedCollectionInterfaceObject.hxx"
#include "DescriptionImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @typedef Description
 *
 * Description is a collection of string for human usage
 */

class Description :
  public TypedCollectionInterfaceObject<DescriptionImplementation>
{
  CLASSNAME;
public:

  /** Default constructor */
  Description();

  /** Constructor with size */
  Description(const UnsignedLong size);

  /** Constructor with value */
  Description(const String & value);

  /** Constructor with size and default value */
  Description(const UnsignedLong size,
              const String & value);

  /** Constructor from a collection */
  Description(const Collection<String> & coll);

  /** Contructor from implementation */
  Description(const DescriptionImplementation & implementation);

  /** Destructor */
  virtual ~Description();

#ifdef SWIG
  /** @copydoc Object::__repr__() const */
  virtual String __repr__() const;

  /** @copydoc Object::__str__() const */
  virtual String __str__(const String & offset = "") const;
#endif

  void sort();
}; /* class Description */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DESCRIPTION_HXX */
