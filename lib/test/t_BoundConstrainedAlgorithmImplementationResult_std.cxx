//                                               -*- C++ -*-
/**
 *  @file  t_BoundConstrainedAlgorithmImplementationResult_std.cxx
 *  @brief The test file of BoundConstrainedAlgorithmImplementationResult class
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
 *  @date   2009-01-08 13:33:24 +0100 (jeu. 08 janv. 2009)
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
      UnsignedLong dim(4);
      NumericalPoint optimizer(dim, 1.0);
      NumericalScalar optimalValue(5.0);
      BoundConstrainedAlgorithmImplementationResult::OptimizationProblem optimization(BoundConstrainedAlgorithmImplementationResult::MINIMIZATION);
      UnsignedLong evaluationsNumber(10);
      NumericalScalar absoluteError(1e-6);
      NumericalScalar relativeError(1e-7);
      NumericalScalar objectiveError(1e-8);
      NumericalScalar constraintError(1e-10);
      BoundConstrainedAlgorithmImplementationResult result(optimizer, optimalValue, optimization, evaluationsNumber, absoluteError, relativeError, objectiveError, constraintError);
      fullprint << "result=" << result << std::endl;
      fullprint << "minimizer=" << result.getOptimizer() << std::endl;
      fullprint << "optimal value=" << result.getOptimalValue() << std::endl;
      fullprint << "optimization problem=" << (result.getOptimizationProblem() == BoundConstrainedAlgorithmImplementationResult::MINIMIZATION ? "minimization" : "maximization") << std::endl;
      fullprint << "evaluations number=" << result.getEvaluationsNumber() << std::endl;
      fullprint << "absolute error=" << result.getAbsoluteError() << std::endl;
      fullprint << "relativeError=" << result.getRelativeError() << std::endl;
      fullprint << "objective error=" << result.getObjectiveError() << std::endl;
      fullprint << "constraint error=" << result.getConstraintError() << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
