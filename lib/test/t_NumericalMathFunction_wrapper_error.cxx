//                                               -*- C++ -*-
/**
 *  @file  t_NumericalMathFunction_wrapper_error.cxx
 *  @brief The test file of class NumericalMathFunction for wrapper error
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
 *  @date   2009-05-28 14:47:53 +0200 (Thu, 28 May 2009)
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

      // Test basic functionnalities
      //checkClassWithClassName<ComputedNumericalMathEvaluationImplementation>();

      // Test some extra functionnalities
      //checkNameFeature<ComputedNumericalMathEvaluationImplementation>();


      /** Instance creation */
      NumericalMathFunction deviation("poutre");

      NumericalPoint inPoint(4);
      inPoint[0] = 210.e9; // E
      inPoint[1] = 1000;   // F
      inPoint[2] = 1.5;    // L
      inPoint[3] = 0.   ;  // I is zero so an error should be raised

      try
        {
          NumericalPoint outPoint = deviation( inPoint );

          // We should never go here
          throw TestFailed( "ERROR: test should have failed. NumericalMathFunction did not failed as expected." );

        }
      catch (InternalException & ex)
        {
          std::cerr << "Got exception: " << ex << std::endl;
        }

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
