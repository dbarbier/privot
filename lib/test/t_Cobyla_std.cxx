//                                               -*- C++ -*-
/**
 *  @file  t_Cobyla_std.cxx
 *  @brief The test file of class Cobyla for standard methods
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

      try
        {
          // Test function operator ()
          NumericalMathFunction levelFunction("TestOptimLinear");
          CobylaSpecificParameters specific;
          NumericalPoint startingPoint(4, 1.0);
          Cobyla myAlgorithm(specific, levelFunction);
          myAlgorithm.setStartingPoint(startingPoint);
          myAlgorithm.setLevelValue(3.0);
          myAlgorithm.setMaximumIterationsNumber(100);
          myAlgorithm.setMaximumAbsoluteError(1.0e-10);
          myAlgorithm.setMaximumRelativeError(1.0e-10);
          myAlgorithm.setMaximumResidualError(1.0e-10);
          myAlgorithm.setMaximumConstraintError(1.0e-10);
          fullprint << "myAlgorithm = " << myAlgorithm << std::endl;
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
