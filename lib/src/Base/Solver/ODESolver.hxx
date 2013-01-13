//                                               -*- C++ -*-
/**
 *  @file  ODESolver.hxx
 *  @brief Cross-validation algorithm interface
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
 *  Id      ODESolver.hxx 1649 2010-11-09 12:44:00Z dutka
 */
#ifndef OPENTURNS_ODESOLVER_HXX
#define OPENTURNS_ODESOLVER_HXX

#include "TypedInterfaceObject.hxx"
#include "ODESolverImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

class ODESolverImplementation;

/**
 * @class ODESolver
 *
 * Cross-validation algorithm interface
 */
class ODESolver
  : public TypedInterfaceObject<ODESolverImplementation>
{
  CLASSNAME;

public:

  typedef TypedInterfaceObject<ODESolverImplementation>::Implementation  Implementation;

  /** Default constructor */
  ODESolver();

  /** Parameter constructor */
  ODESolver(const DynamicalFunction & transitionFunction);

  /** Constructor from implementation */
  ODESolver(const Implementation & p_implementation);

  /** Constructor from implementation */
  ODESolver(const ODESolverImplementation & implementation);

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Perform cross-validation */
  NumericalSample solve(const NumericalPoint & initialState,
                        const NumericalPoint & timeGrid) const;

  /** Transition function accessor */
  void setTransitionFunction(const DynamicalFunction & transitionFunction);
  DynamicalFunction getTransitionFunction() const;

}; /* class ODESolver */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ODESOLVER_HXX */
