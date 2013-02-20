//                                               -*- C++ -*-
/**
 *  @file  DescriptionImplementation.hxx
 *  @brief DescriptionImplementation is the active part of Description
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
 *  Id      DescriptionImplementation.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_DESCRIPTIONIMPLEMENTATION_HXX
#define OPENTURNS_DESCRIPTIONIMPLEMENTATION_HXX

#include "PersistentCollection.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class DescriptionImplementation
 *
 * DescriptionImplementation is the active part of Description
 */

class DescriptionImplementation
  : public PersistentCollection<String>
{
  CLASSNAME;

public:


  /** Default constructor */
  DescriptionImplementation();

  /** Constructor with size */
  DescriptionImplementation(const UnsignedLong size);

  /** Constructor with default value */
  DescriptionImplementation(const String & value);

  /** Constructor with size and default value */
  DescriptionImplementation(const UnsignedLong size,
                            const String & value);

  /** Constructor from a collection */
  DescriptionImplementation(const Collection<String> & coll);

  /** Virtual constructor */
  virtual DescriptionImplementation * clone() const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

}; /* class DescriptionImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DESCRIPTIONIMPLEMENTATION_HXX */
