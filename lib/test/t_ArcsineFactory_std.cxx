//                                               -*- C++ -*-
/**
 *  @file  t_ArcsineFactory_std.cxx
 *  @brief The test file of class Arcsine for standard methods
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
 *  @date   2009-09-30 17:51:57 +0200 (mer. 30 sept. 2009)
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
      Arcsine distribution(1., 2.5);
      UnsignedLong size(10000);
      NumericalSample sample(distribution.getSample(size));
      ArcsineFactory factory;
      Distribution estimatedDistribution(factory.build(sample));
      fullprint << "Distribution          =" << distribution << std::endl;
      fullprint << "Estimated distribution=" << estimatedDistribution << std::endl;
      estimatedDistribution = factory.build();
      fullprint << "Default distribution=" << estimatedDistribution << std::endl;
      estimatedDistribution = factory.build(distribution.getParametersCollection());
      fullprint << "Distribution from parameters=" << estimatedDistribution << std::endl;
      Arcsine estimatedArcsine(factory.buildAsArcsine(sample));
      fullprint << "Arcsine          =" << distribution << std::endl;
      fullprint << "Estimated Arcsine=" << estimatedArcsine << std::endl;
      estimatedArcsine = factory.buildAsArcsine();
      fullprint << "Default Arcsine=" << estimatedArcsine << std::endl;
      estimatedArcsine = factory.buildAsArcsine(distribution.getParametersCollection());
      fullprint << "Arcsine from parameters=" << estimatedArcsine << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
