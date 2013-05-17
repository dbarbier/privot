//                                               -*- C++ -*-
/**
 *  @file  t_UniformFactory_std.cxx
 *  @brief The test file of class Uniform for standard methods
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
  setRandomGenerator();

  try
    {
      NumericalSample sample(3, 2);
      sample[0][0] = 1.0;
      sample[0][1] = 1.5;
      sample[1][0] = 2.0;
      sample[1][1] = 2.5;
      sample[2][0] = 3.0;
      sample[2][1] = 3.5;
      UserDefinedFactory factory;
      CovarianceMatrix covariance;
      // Distribution estimatedDistribution(factory.build(sample, covariance));
      Distribution estimatedDistribution(factory.build(sample));
      fullprint << "Sample                =" << sample << std::endl;
      fullprint << "Estimated distribution=" << estimatedDistribution << std::endl;
      // fullprint << "Covariance=" << covariance << std::endl;
      estimatedDistribution = factory.build();
      fullprint << "Default distribution=" << estimatedDistribution << std::endl;
      UserDefined estimatedUserDefined(factory.buildAsUserDefined(sample));
      fullprint << "Sample                =" << sample << std::endl;
      fullprint << "Estimated UserDefined=" << estimatedUserDefined << std::endl;
      estimatedUserDefined = factory.buildAsUserDefined();
      fullprint << "Default UserDefined=" << estimatedUserDefined << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}