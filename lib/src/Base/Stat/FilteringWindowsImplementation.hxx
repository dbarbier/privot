//                                               -*- C++ -*-
/**
 *  @file  FilteringWindowsImplementation.hxx
 *  @brief This class is enables to build a second order model
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
 *  Id      FilteringWindowsImplementation.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_FILTERINGWINDOWSIMPLEMENTATION_HXX
#define OPENTURNS_FILTERINGWINDOWSIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "Pointer.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class FilteringWindowsImplementation
 */

class FilteringWindowsImplementation
  : public PersistentObject
{

  CLASSNAME;

public:

  typedef Pointer<FilteringWindowsImplementation>    Implementation;

  /** Default constructor without parameters */
  FilteringWindowsImplementation(const String & name = DefaultName);

  /** Virtual copy constructor */
  virtual FilteringWindowsImplementation * clone() const;

  virtual NumericalScalar operator()(const NumericalScalar t) const;

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

} ; /* class FilteringWindowsImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_FILTERINWINDOWSIMPLEMENTATION_HXX */
