//                                               -*- C++ -*-
/**
 *  @file  t_LinearCombinationHessianImplementation_std.cxx
 *  @brief The test file of class LinearCombinationHessianImplementation for standard methods
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
 *  @author dutka
 *  @date   2007-08-01 16:47:51 +0200 (mer, 01 aoû 2007)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {
      // First, build two functions from R^3->R
      Description inVar(3);
      inVar[0] = "x1";
      inVar[1] = "x2";
      inVar[2] = "x3";
      Description outVar(1);
      outVar[0] = "y";
      Description formula(1);
      formula[0] = "x1^3 * sin(x2 + 2.5 * x3) - (x1 + x2)^2 / (1.0 + x3^2)";
      LinearCombinationEvaluationImplementation::NumericalMathFunctionCollection functions(2);
      functions[0] = NumericalMathFunction(inVar, outVar, formula);
      formula[0] = "exp(-x1 * x2 + x3) / cos(1.0 + x2 * x3 - x1)";
      functions[1] = NumericalMathFunction(inVar, outVar, formula);
      // Second, build the weights
      NumericalPoint coefficients(2);
      coefficients[0] = 0.3;
      coefficients[1] = 2.9;
      LinearCombinationHessianImplementation myHessian(LinearCombinationEvaluationImplementation(functions, coefficients));
      NumericalPoint inPoint(3);
      inPoint[0] = 1.2;
      inPoint[1] = 2.3;
      inPoint[2] = 3.4;
      fullprint << "myHessian=" << myHessian << std::endl;
      // Reduce the ouput precision because we use finite difference so only 5 digits are significant
      PlatformInfo::SetNumericalPrecision(5);
      fullprint << "Value at " << inPoint << "=" << myHessian.hessian(inPoint) << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }

  return ExitCode::Success;
}
