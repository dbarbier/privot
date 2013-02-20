//                                               -*- C++ -*-
/**
 *  @file  t_MergeRandomAndConstantInput.cxx
 *  @brief The test file of class NumericalMathFunction for standard methods
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

      /** External code. This code has an input vector of dimension 4, namely (p0, p1, p2, p3)'. */
      NumericalMathFunction externalCode("poutre");
      UnsignedLong dim(externalCode.getInputDimension());

      /** The external code will be connected to 2 independent random variables X0 and X1 and one deterministic variable X2 with the following scheme:
          X2->p0
          X0->p1
          X1->p2
          X0->p3
          It means that (p0, p1, p2, p3)' = A.(X0, X1)' + b with:
          A = [0 0] b = [X2]
          [1 0]     [ 0]
          [0 1]     [ 0]
          [1 0]     [ 0]
          Here we build the linear function x -> A.x + b
      */
      UnsignedLong stochasticDimension(2);
      // UnsignedLong deterministicDimension(1);
      Matrix A(dim, stochasticDimension);
      A(1, 0) = 1;
      A(2, 1) = 1;
      A(3, 0) = 1;
      NumericalPoint b(dim, 0);
      NumericalScalar X2(50.0);
      b[0] = X2;

      NumericalMathFunction connect;
      NumericalPoint zero(stochasticDimension, 0);
      /** A LinearNumericalMathFunction will arrive soon... */
      connect.setEvaluationImplementation(new LinearNumericalMathEvaluationImplementation(zero, b, A.transpose()));
      connect.setGradientImplementation(new ConstantNumericalMathGradientImplementation(A.transpose()));
      connect.setHessianImplementation(new ConstantNumericalMathHessianImplementation(SymmetricTensor(stochasticDimension, dim)));
      /** We are now ready to build the resulting code externalCode(connect()) */
      NumericalMathFunction finalCode(externalCode, connect);

      /** Check if it worked */
      NumericalPoint x(connect.getInputDimension());
      x[0] = 5;
      x[1] = 10;
      fullprint << "finalCode(x)=" << finalCode(x) << std::endl;
      NumericalPoint xRef(dim);
      xRef[0] = X2;
      xRef[1] = x[0];
      xRef[2] = x[1];
      xRef[3] = x[0];
      fullprint << "ref=" << externalCode(xRef) << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
