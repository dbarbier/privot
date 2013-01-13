//                                               -*- C++ -*-
/**
 *  @file  ODESolverImplementation.hxx
 *  @brief Cross-validation algorithm implementation
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
 *  @date   2010-11-09 13:44:00 +0100 (Tue, 09 Nov 2010)
 *  Id      ODESolverImplementation.hxx 1649 2010-11-09 12:44:00Z dutka
 */
#ifndef OPENTURNS_ODESOLVERIMPLEMENTATION_HXX
#define OPENTURNS_ODESOLVERIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "DynamicalFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ODESolverImplementation
 *
 * Cross-validation algorithm implementation
 */
class ODESolverImplementation
  : public PersistentObject
{
  CLASSNAME;

public:

  /** Default constructor */
  ODESolverImplementation();

  /** Parameter constructor */
  ODESolverImplementation(const DynamicalFunction & transitionFunction);

  /** Virtual constructor */
  ODESolverImplementation * clone() const = 0;

  /** String converter */
  virtual String __repr__() const;

  /** Perform cross-validation */
  virtual NumericalSample solve(const NumericalPoint & initialState,
                                const NumericalPoint & timeGrid) const = 0;

  /** Transition function accessor */
  void setTransitionFunction(const DynamicalFunction & transitionFunction);
  DynamicalFunction getTransitionFunction() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

  /** Transition function */
  DynamicalFunction transitionFunction_;

}; /* class ODESolverImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ODESOLVERIMPLEMENTATION_HXX */
