//                                               -*- C++ -*-
/**
 *  @file  t_BetaFactory_std.cxx
 *  @brief The test file of class Beta for standard methods
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
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
      Beta distribution(0.2, 0.6, -1.0, 2.0);
      UnsignedLong size(10000);
      NumericalSample sample(distribution.getSample(size));
      BetaFactory factory;
      CovarianceMatrix covariance;
      // Distribution estimatedDistribution(factory.build(sample, covariance));
      Distribution estimatedDistribution(factory.build(sample));
      fullprint << "Distribution          =" << distribution << std::endl;
      fullprint << "Estimated distribution=" << estimatedDistribution << std::endl;
      // fullprint << "Covariance=" << covariance << std::endl;
      distribution = Beta(0.5, 1.3, -1.0, 2.0);
      sample = distribution.getSample(size);
      // estimatedDistribution = factory.build(sample, covariance);
      estimatedDistribution = factory.build(sample);
      fullprint << "Distribution          =" << distribution << std::endl;
      fullprint << "Estimated distribution=" << estimatedDistribution << std::endl;
      // fullprint << "Covariance=" << covariance << std::endl;
      distribution = Beta(0.5, 2.3, -1.0, 2.0);
      sample = distribution.getSample(size);
      // estimatedDistribution = factory.build(sample, covariance);
      estimatedDistribution = factory.build(sample);
      fullprint << "Distribution          =" << distribution << std::endl;
      fullprint << "Estimated distribution=" << estimatedDistribution << std::endl;
      // fullprint << "Covariance=" << covariance << std::endl;
      distribution = Beta(1.5, 4.3, -1.0, 2.0);
      sample = distribution.getSample(size);
      // estimatedDistribution = factory.build(sample, covariance);
      estimatedDistribution = factory.build(sample);
      fullprint << "Distribution          =" << distribution << std::endl;
      fullprint << "Estimated distribution=" << estimatedDistribution << std::endl;
      // fullprint << "Covariance=" << covariance << std::endl;
      estimatedDistribution = factory.build();
      fullprint << "Default distribution=" << estimatedDistribution << std::endl;
      estimatedDistribution = factory.build(distribution.getParametersCollection());
      fullprint << "Distribution from parameters=" << estimatedDistribution << std::endl;
      Beta estimatedBeta(factory.buildAsBeta(sample));
      fullprint << "Beta          =" << distribution << std::endl;
      fullprint << "Estimated Beta=" << estimatedBeta << std::endl;
      estimatedBeta = factory.buildAsBeta();
      fullprint << "Default Beta=" << estimatedBeta << std::endl;
      estimatedBeta = factory.buildAsBeta(distribution.getParametersCollection());
      fullprint << "Beta from parameters=" << estimatedBeta << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}