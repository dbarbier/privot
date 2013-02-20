//                                               -*- C++ -*-
/**
 *  @file  Event.hxx
 *  @brief This class specializes the concept of RandomVector by constructing a Bernoulli
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
 *  Id      Event.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_EVENT_HXX
#define OPENTURNS_EVENT_HXX

#include "RandomVector.hxx"
#include "Pointer.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class Event
 *
 * The class that implements all random vectors
 */
class Event
  : public RandomVector
{
  CLASSNAME;

public:


  /** for save/load mecanism */
  Event();

  /** Constructor from RandomVector */
  Event(const RandomVector & antecedent,
        const ComparisonOperator & op,
        const NumericalScalar threshold,
        const String & name = DefaultName);

  /** Constructor from RandomVector */
  Event(const RandomVector & antecedent,
        const Domain & domain,
        const String & name = DefaultName);

  /** Constructor from RandomVector */
  Event(const Process &  process,
        const Domain & domain,
        const String & name = DefaultName);

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

private:

}; /* class Event */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_EVENT_HXX */
