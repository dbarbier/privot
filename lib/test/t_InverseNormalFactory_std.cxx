//                                               -*- C++ -*-
/**
 *  @file  t_InverseNormalFactory_std.cxx
 *  @brief The test file of class InverseNormal for standard methods
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
 *  @date   2011-04-11 12:32:27 +0200 (lun. 11 avril 2011)
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
      InverseNormal distribution(2.5, 1.5);
      UnsignedLong size(10000);
      NumericalSample sample(distribution.getSample(size));
      InverseNormalFactory factory;
      CovarianceMatrix covariance;
      // Distribution estimatedDistribution(factory.build(sample, covariance));
      Distribution estimatedDistribution(factory.build(sample));
      fullprint << "Distribution          =" << distribution << std::endl;
      fullprint << "Estimated distribution=" << estimatedDistribution << std::endl;
      // fullprint << "Covariance=" << covariance << std::endl;
      estimatedDistribution = factory.build();
      fullprint << "Default distribution=" << estimatedDistribution << std::endl;
      estimatedDistribution = factory.build(distribution.getParametersCollection());
      fullprint << "Distribution from parameters=" << estimatedDistribution << std::endl;
      InverseNormal estimatedInverseNormal(factory.buildAsInverseNormal(sample));
      fullprint << "InverseNormal          =" << distribution << std::endl;
      fullprint << "Estimated inverseNormal=" << estimatedInverseNormal << std::endl;
      estimatedInverseNormal = factory.buildAsInverseNormal();
      fullprint << "Default inverseNormal=" << estimatedInverseNormal << std::endl;
      estimatedInverseNormal = factory.buildAsInverseNormal(distribution.getParametersCollection());
      fullprint << "InverseNormal from parameters=" << estimatedInverseNormal << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
