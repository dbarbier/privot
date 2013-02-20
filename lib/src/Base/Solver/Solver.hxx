//                                               -*- C++ -*-
/**
 *  @file  Solver.hxx
 *  @brief Interface class for a nonlinear scalar solver
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
 *  Id      Solver.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_SOLVER_HXX
#define OPENTURNS_SOLVER_HXX

#include "OTprivate.hxx"
#include "SolverImplementation.hxx"
#include "Pointer.hxx"
#include "TypedInterfaceObject.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Solver
 *
 * This class is an interface for the 1D nonlinear solvers
 */
class Solver :
  public TypedInterfaceObject<SolverImplementation>
{
  CLASSNAME;
public:

  typedef Pointer<SolverImplementation>           Implementation;

  /** Parameter constructor */
  Solver(const Implementation & p_implementation);

  /** Parameter from an implementation */
  Solver(const SolverImplementation & implementation);

  /** Parameter constructor */
  explicit Solver();

  /** Second parameter constructor */
  Solver(const NumericalScalar absoluteError,
         const NumericalScalar relativeError,
         const NumericalScalar residualError,
         const UnsignedLong maximumFunctionEvaluation);


  /** Comparison operator */
  Bool operator ==(const Solver & other) const;

  /** String converter */
  virtual String __repr__() const;

  /** Solve attempt to find one root to the equation function(x) = value in [infPoint, supPoint] */
  virtual NumericalScalar solve(const NumericalMathFunction & function,
                                const NumericalScalar value,
                                const NumericalScalar infPoint,
                                const NumericalScalar supPoint) const;

  /** Solve attempt to find one root to the equation function(x) = value in [infPoint, supPoint] given function(infPoint) and function(supPoint) */
  virtual NumericalScalar solve(const NumericalMathFunction & function,
                                const NumericalScalar value,
                                const NumericalScalar infPoint,
                                const NumericalScalar supPoint,
                                const NumericalScalar infValue,
                                const NumericalScalar supValue) const;

  /** Absolute error accessor */
  void setAbsoluteError(const NumericalScalar absoluteError);
  NumericalScalar getAbsoluteError() const;

  /** Relative error accessor */
  void setRelativeError(const NumericalScalar relativeError);
  NumericalScalar getRelativeError() const;

  /** Residual error accessor */
  void setResidualError(const NumericalScalar residualError);
  NumericalScalar getResidualError() const;

  /** Maximum function evaluation accessor */
  void setMaximumFunctionEvaluation(const UnsignedLong maximumFunctionEvaluation);
  UnsignedLong getMaximumFunctionEvaluation() const;


}; /* Class Solver */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SOLVER_HXX */
