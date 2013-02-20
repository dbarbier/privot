//                                               -*- C++ -*-
/**
 *  @file  t_NumericalMathFunction_dual_linear_combination.cxx
 *  @brief The test file of class NumericalMathFunction for dual_linear combinations
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
 *  @date   2007-05-10 16:43:31 +0200 (jeu, 10 mai 2007)
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
      DualLinearCombinationEvaluationImplementation::NumericalMathFunctionCollection functions(2);
      functions[0] = NumericalMathFunction(inVar, outVar, formula);
      formula[0] = "exp(-x1 * x2 + x3) / cos(1.0 + x2 * x3 - x1)";
      functions[1] = NumericalMathFunction(inVar, outVar, formula);
      // Second, build the weights
      NumericalSample coefficients(0, 2);
      NumericalPoint p(2);
      p[0] = 1.5;
      p[1] = 2.5;
      coefficients.add(p);
      p[0] = -3.5;
      p[1] = 0.5;
      coefficients.add(p);
      // Third, build the function
      NumericalMathFunction myFunction(functions, coefficients);
      NumericalPoint inPoint(3);
      inPoint[0] = 1.2;
      inPoint[1] = 2.3;
      inPoint[2] = 3.4;
      fullprint << "myFunction=" << myFunction << std::endl;
      fullprint << "Value at " << inPoint << "=" << myFunction(inPoint) << std::endl;
      fullprint << "Gradient at " << inPoint << "=" << myFunction.gradient(inPoint) << std::endl;
      PlatformInfo::SetNumericalPrecision(5);
      fullprint << "Hessian at " << inPoint << "=" << myFunction.hessian(inPoint) << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
