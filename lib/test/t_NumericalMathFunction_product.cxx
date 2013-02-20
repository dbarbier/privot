//                                               -*- C++ -*-
/**
 *  @file  t_NumericalMathFunction_product.cxx
 *  @brief The test file of class NumericalMathFunction for linear combinations
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
 *  @author: $LastChangedBy: dutka $
 *  @date:   $LastChangedDate: 2007-05-10 16:43:31 +0200 (jeu, 10 mai 2007) $
 *  Id:      $Id: t_NumericalMathFunction_product.cxx 434 2007-05-10 14:43:31Z dutka $
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
      NumericalMathFunction f1(inVar, outVar, formula);
      formula[0] = "exp(-x1 * x2 + x3) / cos(1.0 + x2 * x3 - x1)";
      formula.add("sin(x2+x3*x1^2)");
      outVar.add("z");
      NumericalMathFunction f2(inVar, outVar, formula);
      // Second, build the function
      NumericalMathFunction myFunction(f1 * f2);
      NumericalPoint inPoint(3);
      inPoint[0] = 1.2;
      inPoint[1] = 2.3;
      inPoint[2] = 3.4;
      fullprint << "myFunction=" << myFunction << std::endl;
      fullprint << "myFunction=" << myFunction.__str__() << std::endl;
      const NumericalPoint value(myFunction(inPoint));
      fullprint << "Value at " << inPoint << "=\n" << value << std::endl;
      fullprint << "Value at " << inPoint << "=\n" << value.__str__() << std::endl;
      const Matrix gradient(myFunction.gradient(inPoint));
      fullprint << "Gradient at " << inPoint << "=\n" << gradient << std::endl;
      fullprint << "Gradient at " << inPoint << "=\n" << gradient.__str__() << std::endl;
      const SymmetricTensor hessian(myFunction.hessian(inPoint));
      fullprint << "Hessian at " << inPoint << "=\n" << hessian << std::endl;
      fullprint << "Hessian at " << inPoint << "=\n" << hessian.__str__() << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
