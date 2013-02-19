//                                               -*- C++ -*-
/**
 *  @file  t_AbdoRackwitz_nonlinear.cxx
 *  @brief The test file of class AbdoRackwitz for solving a nonlinear problem
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

String printNumericalPoint(const NumericalPoint & point, const UnsignedLong digits)
{
  OSS oss;
  oss << "[";
  NumericalScalar eps(pow(0.1, digits));
  for (UnsignedLong i = 0; i < point.getDimension(); i++)
    {
      oss << std::fixed << std::setprecision(digits) << (i == 0 ? "" : ",") << Bulk<double>((fabs(point[i]) < eps) ? fabs(point[i]) : point[i]);
    }
  oss << "]";
  return oss;
}

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {
      NumericalMathFunction levelFunction("TestOptimNonLinear");
      // Activate the cache as we will use an analytical method
      levelFunction.enableCache();
      // Add a finite difference gradient to the function, as Abdo Rackwitz algorithm
      // needs it
      NonCenteredFiniteDifferenceGradient myGradient(1e-7, levelFunction.getEvaluationImplementation());
      fullprint << "myGradient=" << myGradient << std::endl;
      /** Substitute the gradient */
      levelFunction.setGradientImplementation(new NonCenteredFiniteDifferenceGradient(myGradient));
      AbdoRackwitzSpecificParameters specific;
      NumericalPoint startingPoint(4, 0.0);
      AbdoRackwitz myAlgorithm(specific, levelFunction);
      myAlgorithm.setStartingPoint(startingPoint);
      myAlgorithm.setLevelValue(-0.5);
      fullprint << "myAlgorithm = " << myAlgorithm << std::endl;
      myAlgorithm.run();
      NearestPointAlgorithmImplementationResult result(myAlgorithm.getResult());
      fullprint << "result = " << printNumericalPoint(result.getMinimizer(), 4) << std::endl;
      Graph convergence(result.getErrorHistory());
      convergence.draw("AbdoRackwitzConvergence");
      fullprint << "evaluation calls number=" << levelFunction.getEvaluationCallsNumber() << std::endl;
      fullprint << "gradient   calls number=" << levelFunction.getGradientCallsNumber() << std::endl;
      fullprint << "hessian    calls number=" << levelFunction.getHessianCallsNumber() << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }
  
  return ExitCode::Success;
}
