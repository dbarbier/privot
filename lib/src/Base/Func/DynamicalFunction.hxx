//                                               -*- C++ -*-
/**
 *  @file  DynamicalFunction.hxx
 *  @brief The class that implements dynamical functions
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
 *  @date   2011-05-24 19:30:41 +0200 (mar. 24 mai 2011)
 *  Id      DynamicalFunction.hxx 1910 2011-05-24 17:30:41Z schueller
 */
#ifndef OPENTURNS_DYNAMICALFUNCTION_HXX
#define OPENTURNS_DYNAMICALFUNCTION_HXX

#include "TypedInterfaceObject.hxx"
#include "DynamicalFunctionImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class DynamicalFunction
 * @brief Simulates a dynamical function
 * @ingroup Function
 *
 * The class that simulates a dynamical function, it means
 * a function acting on a set of vectors indexed by the time.
 * This class is just an interface to actual implementation
 * objects that can be hot-replaced during computation.
 * Each implementation object refers to a specific evaluation
 * @see DynamicalFunctionImplementation
 */
class DynamicalFunction
  : public TypedInterfaceObject<DynamicalFunctionImplementation>
{
  CLASSNAME;
public:

  /* Some typedefs for easy reading */

  /** Default constructor */
  DynamicalFunction();

  /** Constructor from DynamicalFunctionImplementation */
  DynamicalFunction(const DynamicalFunctionImplementation & implementation);

#ifndef SWIG
  /** Constructor from implementation */
  DynamicalFunction(const Implementation & p_implementation);

  /** Constructor from implementation pointer */
  DynamicalFunction(DynamicalFunctionImplementation * p_implementation);
#endif

  /** Comparison operator */
  Bool operator ==(const DynamicalFunction & other) const;

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

  /** Operator () */
  TimeSeries operator() (const TimeSeries & inTS) const;
  ProcessSample operator() (const ProcessSample & inPS) const;

  /** Get the i-th marginal function */
  DynamicalFunction getMarginal(const UnsignedLong i) const;

  /** Get the function corresponding to indices components */
  DynamicalFunction getMarginal(const Indices & indices) const;

  /** Accessor for input point dimension */
  UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  UnsignedLong getOutputDimension() const;

  /** Input description Accessor, i.e. the names of the input parameters */
  Description getInputDescription() const;

  /** Output description Accessor, i.e. the names of the Output parameters */
  Description getOutputDescription() const;

  /** Number of calls to the evaluation */
  UnsignedLong getCallsNumber() const;

}; /* class DynamicalFunction */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DYNAMICALFUNCTION_HXX */
