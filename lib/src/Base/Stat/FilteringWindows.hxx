//                                               -*- C++ -*-
/**
 *  @file  FilteringWindows.hxx
 *  @brief This class is enables to build a second order model
 *  This is the interface class
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
 *  Id      FilteringWindows.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_FILTERINGWINDOWS_HXX
#define OPENTURNS_FILTERINGWINDOWS_HXX

#include "FilteringWindowsImplementation.hxx"
#include "PersistentObject.hxx"
#include "TypedInterfaceObject.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class FilteringWindowsImplementation
 */

class FilteringWindows
  : public TypedInterfaceObject<FilteringWindowsImplementation>
{

  CLASSNAME;

public:

  /** Default onstructor */
  FilteringWindows();

  /** Copy constructors */
  FilteringWindows(const FilteringWindowsImplementation & implementation,
                   const String & name = DefaultName);

  /** Constructor from implementation */
  FilteringWindows(const Implementation & p_implementation,
                   const String & name = DefaultName);

#ifndef SWIG
  /** Constructor from implementation pointer */
  FilteringWindows(FilteringWindowsImplementation * p_implementation,
                   const String & name = DefaultName);

#endif

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** functor - evaluation of filter using () */
  NumericalScalar operator()(const NumericalScalar t) const;

} ; /* class FilteringWindows */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_FILTERINGWINDOWS_HXX */
