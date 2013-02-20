//                                               -*- C++ -*-
/**
 * @file  EventRandomVectorImplementation.hxx
 * @brief The class that implements an event random vector, i.e. a
 *        composite random vector built upon a boolean function which
 *        is described by a threshold and a comparison operator. The
 *        antecedent of the event must be a "classical" composite random
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

#ifndef OPENTURNS_EVENTRANDOMVECTORIMPLEMENTATION_HXX
#define OPENTURNS_EVENTRANDOMVECTORIMPLEMENTATION_HXX

#include "CompositeRandomVector.hxx"


BEGIN_NAMESPACE_OPENTURNS




/**
 * @class EventRandomVectorImplementation
 *
 *
 */
class EventRandomVectorImplementation
  : public CompositeRandomVector
{
  CLASSNAME;

public:


  /** Default constructor */
  EventRandomVectorImplementation();

  /** Constructor from RandomVector */
  EventRandomVectorImplementation(const RandomVectorImplementation & antecedent,
                                  const ComparisonOperator & op,
                                  const NumericalScalar threshold,
                                  const String & name = DefaultName);

  EventRandomVectorImplementation * clone() const;

  /** String converter */
  String __repr__() const;

  /** Dimension accessor */
  virtual UnsignedLong getDimension() const;

  /** Operator accessor */
  ComparisonOperator getOperator() const;

  /** Threshold accessor */
  NumericalScalar getThreshold() const;

  /* Here is the interface that all derived class may implement */

  /** Realization accessor */
  virtual NumericalPoint getRealization() const;

  /** Numerical sample accessor */
  virtual NumericalSample getSample(UnsignedLong size) const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:


private:

  friend class Factory<EventRandomVectorImplementation>;

  /** The comparison operator of the eventRandomVectorImplementation */
  ComparisonOperator operator_;

  /** The threshold of the eventRandomVectorImplementation */
  NumericalScalar threshold_;

}; /* class EventRandomVectorImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_EVENTRANDOMVECTORIMPLEMENTATION_HXX */
