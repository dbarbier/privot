//                                               -*- C++ -*-
/**
 *  @file  t_NearestPointChecker_std.cxx
 *  @brief The test file of class NearestPointChecker for standard methods
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
          // Function
          UnsignedLong sampleSize(20);
          NumericalMathFunction levelFunction("TestOptimLinear");
          LessOrEqual myOperator;
          NumericalScalar threshold(2.0);
          NumericalSample mySample(0, levelFunction.getInputDimension());
          NumericalScalar random(0.1);
          for(UnsignedLong index = 0; index < sampleSize; index++)
            {
              NumericalPoint point(levelFunction.getInputDimension());
              NumericalScalar norm(0.0);
              for(UnsignedLong coordinate = 0; coordinate < levelFunction.getInputDimension(); coordinate++)
                {
                  point[coordinate] = sqrt(-2.0 * log(random));
                  random = fmod(random + sqrt(2.0), 1.0);
                  point[coordinate] *= cos(2.0 * atan(1.0) * random);
                  norm += point[coordinate] * point[coordinate];
                }
              for(UnsignedLong coordinate = 0; coordinate < levelFunction.getInputDimension(); coordinate++)
                {
                  point[coordinate] /= sqrt(norm);
                }
              mySample.add(point);
            }
          NearestPointChecker myNearestPointChecker(levelFunction, myOperator, threshold, mySample);
          for(UnsignedLong index = 0; index < sampleSize; index++)
            {
              fullprint << mySample[index] << std::endl;
            }
          myNearestPointChecker.run();
          fullprint << "myNearestPointChecker = " << myNearestPointChecker << std::endl;
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
