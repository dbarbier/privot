//                                               -*- C++ -*-
/**
 * @file  EventDomainImplementation.hxx
 * @brief The class that implements an event domain based on a composite random vector
 *        built upon a boolean function which is described by a domain appartenance.
 *        The antecedent of the event must be a "classical" composite random
 *        vector in the context of OpenTURNS.
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
 * @author schueller
 * @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */

#ifndef OPENTURNS_EVENTDOMAINIMPLEMENTATION_HXX
#define OPENTURNS_EVENTDOMAINIMPLEMENTATION_HXX

#include "CompositeRandomVector.hxx"
#include "Domain.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class EventDomainImplementation
 *
 *
 */
class EventDomainImplementation
  : public CompositeRandomVector
{
  CLASSNAME;

public:

  /** Default constructor */
  EventDomainImplementation();

  /** Constructor from RandomVector */
  EventDomainImplementation(const RandomVectorImplementation & antecedent,
                            const Domain & domain,
                            const String & name = DefaultName);

  EventDomainImplementation * clone() const;

  /** String converter */
  String __repr__() const;

  /** Dimension accessor */
  virtual UnsignedLong getDimension() const;

  /** Domain accessor */
  Domain getDomain() const;

  /* Here is the interface that all derived class may implement */

  /** Realization accessor */
  virtual NumericalPoint getRealization() const;

  /** Numerical sample accessor */
  virtual NumericalSample getSample(const UnsignedLong size) const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:


private:

  friend class Factory<EventDomainImplementation>;

  /** The domain of the eventDomainImplementation */
  Domain domain_;


}; /* class EventDomainImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_EVENTDOMAINIMPLEMENTATION_HXX */
