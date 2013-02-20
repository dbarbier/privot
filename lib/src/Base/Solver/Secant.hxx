//                                               -*- C++ -*-
/**
 *  @file  Secant.hxx
 *  @brief Implementation class of the scalar nonlinear solver based on
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
 *  Id      Secant.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_SECANT_HXX
#define OPENTURNS_SECANT_HXX

#include "OTprivate.hxx"
#include "SolverImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Secant
 *
 * This class is an interface for the 1D nonlinear solverImplementations
 */
class Secant :
  public SolverImplementation
{
  CLASSNAME;
public:

  /** Parameter constructor */
  Secant(const NumericalScalar absoluteError = ResourceMap::GetAsNumericalScalar( "SolverImplementation-DefaultAbsoluteError" ),
         const NumericalScalar relativeError = ResourceMap::GetAsNumericalScalar( "SolverImplementation-DefaultRelativeError" ),
         const NumericalScalar residualError = ResourceMap::GetAsNumericalScalar( "SolverImplementation-DefaultResidualError" ),
         const UnsignedLong maximumFunctionEvaluation = ResourceMap::GetAsUnsignedLong( "SolverImplementation-DefaultMaximumFunctionEvaluation" ));


  /** Virtual constructor */
  virtual Secant * clone() const;

  /** String converter */
  String __repr__() const;

  /** Solve attempt to find one root to the equation function(x) = value in [infPoint, supPoint] given function(infPoint) and function(supPoint) with the Secant method */
  using SolverImplementation::solve;
  NumericalScalar solve(const NumericalMathFunction & function,
                        const NumericalScalar value,
                        const NumericalScalar infPoint,
                        const NumericalScalar supPoint,
                        const NumericalScalar infValue,
                        const NumericalScalar supValue) const;

private:

}; /* Class Secant */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SECANT_HXX */
