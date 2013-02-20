//                                               -*- C++ -*-
/**
 *  @file  t_NumericalMathFunction_exec.cxx
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

      // Test basic functionnalities
      //checkClassWithClassName<ComputedNumericalMathEvaluationImplementation>();

      // Test some extra functionnalities
      //checkNameFeature<ComputedNumericalMathEvaluationImplementation>();


      /** Instance creation */
      NumericalMathFunction deviation("poutre");

      NumericalPoint inPoint(4);
      inPoint[0] = 210.e9;
      inPoint[1] = 1000;
      inPoint[2] = 1.5;
      inPoint[3] = 2.e-6;

      NumericalPoint outPoint = deviation( inPoint );

      fullprint << "deviation = " << outPoint << std::endl;

      // Test the cache behavior
      deviation.enableCache();
      fullprint << "calls = " << deviation.getEvaluationCallsNumber() << " hits = " << deviation.getCacheHits() << std::endl;
      outPoint = deviation( inPoint );
      fullprint << "deviation = " << outPoint << std::endl;
      fullprint << "calls = " << deviation.getEvaluationCallsNumber() << " hits = " << deviation.getCacheHits() << std::endl;
      outPoint = deviation( inPoint );
      fullprint << "deviation = " << outPoint << std::endl;
      fullprint << "calls = " << deviation.getEvaluationCallsNumber() << " hits = " << deviation.getCacheHits() << std::endl;


    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
