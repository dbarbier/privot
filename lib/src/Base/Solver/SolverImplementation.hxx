//                                               -*- C++ -*-
/**
 *  @file  SolverImplementation.hxx
 *  @brief Implementation class for a nonlinear scalar solver
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
 *  Id      SolverImplementation.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_SOLVERIMPLEMENTATION_HXX
#define OPENTURNS_SOLVERIMPLEMENTATION_HXX

#include "OTprivate.hxx"
#include "NumericalMathFunction.hxx"
#include "PersistentObject.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class SolverImplementation
 *
 * This class is an interface for the 1D nonlinear solverImplementations
 */
class SolverImplementation :
  public PersistentObject
{
  CLASSNAME;
public:

  /** Parameter constructor */
  SolverImplementation(const NumericalScalar absoluteError = ResourceMap::GetAsNumericalScalar( "SolverImplementation-DefaultAbsoluteError" ),
                       const NumericalScalar relativeError = ResourceMap::GetAsNumericalScalar( "SolverImplementation-DefaultRelativeError" ),
                       const NumericalScalar residualError = ResourceMap::GetAsNumericalScalar( "SolverImplementation-DefaultResidualError" ),
                       const UnsignedLong maximumFunctionEvaluation = ResourceMap::GetAsUnsignedLong( "SolverImplementation-DefaultMaximumFunctionEvaluation" ));


  /** Virtual constructor */
  virtual SolverImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const SolverImplementation & other) const;

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

protected:


  /** Used function evaluation statistics */
  mutable UnsignedLong maximumFunctionEvaluation_;
  mutable UnsignedLong usedFunctionEvaluation_;

private:
  NumericalScalar absoluteError_;
  NumericalScalar relativeError_;
  NumericalScalar residualError_;

}; /* Class SolverImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SOLVERIMPLEMENTATION_HXX */
