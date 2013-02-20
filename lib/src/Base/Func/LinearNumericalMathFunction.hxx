//                                               -*- C++ -*-
/**
 *  @file  LinearNumericalMathFunction.hxx
 *  @brief The class that implements linear numerical math functions.
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
 *  Id      LinearNumericalMathFunction.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_LINEARNUMERICALMATHFUNCTION_HXX
#define OPENTURNS_LINEARNUMERICALMATHFUNCTION_HXX

#include "NumericalMathFunction.hxx"
#include "NumericalPoint.hxx"
#include "Matrix.hxx"
#include "LinearNumericalMathEvaluationImplementation.hxx"
#include "ConstantNumericalMathGradientImplementation.hxx"
#include "ConstantNumericalMathHessianImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class LinearNumericalMathFunction
 *
 * The class that simulates a linear numerical math function,
 * its gradient and its hessian. This class is just an interface
 * to actual implementation objects that can be hot-replaced
 * during computation. Each implementation object refers to
 * the function, the gradient or the hessian.
 */
class LinearNumericalMathFunction
  : public NumericalMathFunction
{
  CLASSNAME;
public:

  /* Some typedefs for easy reading */

  /** Default constructor */
  LinearNumericalMathFunction(const NumericalPoint & center,
                              const NumericalPoint & constant,
                              const Matrix & linear);


  /** Comparison operator */
  Bool operator ==(const LinearNumericalMathFunction & other) const;

  /** String converter */
  virtual String __repr__() const;

}; /* class LinearNumericalMathFunction */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARNUMERICALMATHFUNCTION_HXX */
