//                                               -*- C++ -*-
/**
 *  @file  t_SQP_nonlinear.cxx
 *  @brief The test file of class SQP for solving a nonlinear problem
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

      try
        {
          // Test function operator ()
          NumericalMathFunction levelFunction("TestOptimNonLinear");
          // Add a finite difference gradient to the function, as SQP algorithm
          // needs it
          CenteredFiniteDifferenceGradient myGradient(1e-7, levelFunction.getEvaluationImplementation());
          /** Substitute the gradient */
          levelFunction.setGradientImplementation(new CenteredFiniteDifferenceGradient(myGradient));

          // Add a finite difference hessian to the function, as SQP algorithm
          // needs it
          CenteredFiniteDifferenceHessian myHessian(1e-3, levelFunction.getEvaluationImplementation());
          /** Substitute the hessian */
          levelFunction.setHessianImplementation(new CenteredFiniteDifferenceHessian(myHessian));
          SQPSpecificParameters specific;
          NumericalPoint startingPoint(4, 0.0);
          SQP mySQPAlgorithm(specific, levelFunction);
          mySQPAlgorithm.setStartingPoint(startingPoint);
          mySQPAlgorithm.setLevelValue(3.0);
          fullprint << "mySQPAlgorithm=" << mySQPAlgorithm << std::endl;
          mySQPAlgorithm.run();
          fullprint << "result=" << printNumericalPoint(mySQPAlgorithm.getResult().getMinimizer(), 4) << std::endl;
        }
      catch (NoWrapperFileFoundException & ex)
        {
          throw TestFailed(ex.__repr__());
        }


    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }

  return ExitCode::Success;
}
