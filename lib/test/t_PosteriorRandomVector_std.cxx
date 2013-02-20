//                                               -*- C++ -*-
/**
 *  @file  t_RandomWalkMetropolisHastings_std.cxx
 *  @brief The test file of class RandomWalkMetropolisHastings
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
#include <iostream>
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;
typedef ComposedDistribution::DistributionCollection DistributionCollection;
typedef RandomWalkMetropolisHastings::CalibrationStrategyCollection CalibrationStrategyCollection;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {

      // observations
      UnsignedLong size = 10;
      Normal realDist(31., 1.2);

      NumericalSample data(realDist.getSample(size));

      // calibration parameters
      CalibrationStrategyCollection calibrationColl(2);

      // proposal distribution
      DistributionCollection proposalColl;
      Uniform mean_proposal(-2.0, 2.0);
      Uniform std_proposal(-2.0, 2.0);
      proposalColl.add(mean_proposal);
      proposalColl.add(std_proposal);

      // prior distribution
      NumericalScalar mu0 = 25.;

      NumericalScalar sigma0 = 0.1;
      Normal mean_prior(mu0, sigma0);
      Dirac std_prior(2.0); // standard dev is known
      DistributionCollection priorColl;
      priorColl.add(mean_prior);
      priorColl.add(std_prior);
      Distribution prior = ComposedDistribution( priorColl );

      // choose the initial state within the prior
      NumericalPoint initialState(prior.getRealization());

      // conditional distribution
      Distribution conditional = Normal();

      // create a metropolis-hastings sampler
      RandomWalkMetropolisHastings sampler(prior, conditional, data, initialState, proposalColl);
      sampler.setVerbose(true);
      sampler.setThinning(100);
      sampler.setBurnIn(1000);
      sampler.setCalibrationStrategyPerComponent(calibrationColl);

      // create a PosteriorRandomVector
      PosteriorRandomVector randomVector(sampler);
      std::cout << "randomVector=" << randomVector << std::endl;

      // get the dimension
      UnsignedLong dimension = randomVector.getDimension();
      std::cout << "dimension=" << dimension << std::endl;

      // get a realization
      NumericalPoint realization(randomVector.getRealization());
      std::cout << "realization=" << realization << std::endl;

      // get a sample
      NumericalSample sample(randomVector.getSample(10));
      std::cout << "sample=" << sample << std::endl;


    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
