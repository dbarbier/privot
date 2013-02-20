//                                               -*- C++ -*-
/**
 *  @file  t_RandomMixture_std.cxx
 *  @brief The test file of class RandomMixture for standard methods
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
 *  @author lebrun
 *  @date   2008-07-03 08:31:34 +0200 (jeu, 03 jui 2008)
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
      // Create a collection of test-cases and the associated references
      UnsignedLong numberOfTests(3);
      Collection< Collection< Distribution > > testCases(numberOfTests);
      Collection< Distribution > references(numberOfTests);
      testCases[0] = Collection<Distribution>(2);
      testCases[0][0] = Uniform(-1.0, 3.0);
      testCases[0][1] = Uniform(-1.0, 3.0);
      references[0] = Triangular(-2.0, 2.0, 6.0);
      testCases[1] = Collection<Distribution>(3);
      testCases[1][0] = Normal();
      testCases[1][1] = Normal(1.0, 2.0);
      testCases[1][2] = Normal(-2.0, 2.0);
      references[1] = Normal(-1.0, 3.0);
      testCases[2] = Collection<Distribution>(3);
      testCases[2][0] = Exponential();
      testCases[2][1] = Exponential();
      testCases[2][2] = Exponential();
      references[2] = Gamma(3.0, 1.0, 0.0);
      fullprint << "testCases=" << testCases << std::endl;
      fullprint << "references=" << references << std::endl;
      for (UnsignedLong testIndex = 0; testIndex < testCases.getSize(); ++testIndex)
        {
          // Instanciate one distribution object
          RandomMixture distribution(testCases[testIndex]);
          distribution.setBlockMin(5);
          distribution.setBlockMax(20);
          Distribution distributionReference(references[testIndex]);
          fullprint << "Distribution " << distribution << std::endl;
          std::cout << "Distribution " << distribution << std::endl;

          // Is this distribution elliptical ?
          fullprint << "Elliptical = " << (distribution.isElliptical() ? "true" : "false") << std::endl;

          // Is this distribution continuous ?
          fullprint << "Continuous = " << (distribution.isContinuous() ? "true" : "false") << std::endl;

          // Test for realization of distribution
          NumericalPoint oneRealization = distribution.getRealization();
          fullprint << "oneRealization=" << oneRealization << std::endl;

          // Test for sampling
          UnsignedLong size = 10000;
          NumericalSample oneSample = distribution.getSample( size );
          fullprint << "oneSample first=" << oneSample[0] << " last=" << oneSample[size - 1] << std::endl;
          fullprint << "mean=" << oneSample.computeMean() << std::endl;
          fullprint << "covariance=" << oneSample.computeCovariance() << std::endl;

          // Define a point
          NumericalPoint point(distribution.getDimension(), 0.5);
          fullprint << "Point= " << point << std::endl;

          // Show PDF and CDF of point
          NumericalScalar eps(1e-5);
          NumericalPoint DDF = distribution.computeDDF(point);
          fullprint << "ddf      =" << DDF << std::endl;
          fullprint << "ddf (ref)=" << distributionReference.computeDDF(point) << std::endl;
          NumericalScalar PDF = distribution.computePDF(point);
          fullprint << "pdf      =" << PDF << std::endl;
          fullprint << "pdf  (FD)=" << (distribution.computeCDF( point + NumericalPoint(1, eps) ) - distribution.computeCDF( point  + NumericalPoint(1, -eps) )) / (2.0 * eps) << std::endl;
          fullprint << "pdf (ref)=" << distributionReference.computePDF(point) << std::endl;
          NumericalScalar CDF = distribution.computeCDF( point );
          fullprint << "cdf      =" << CDF << std::endl;
          fullprint << "cdf (ref)=" << distributionReference.computeCDF(point) << std::endl;
          NumericalComplex CF = distribution.computeCharacteristicFunction( point[0] );
          fullprint << "characteristic function=" << CF << std::endl;
          NumericalComplex LCF = distribution.computeLogCharacteristicFunction( point[0] );
          fullprint << "log characteristic function=" << LCF << std::endl;
          NumericalPoint quantile = distribution.computeQuantile( 0.95 );
          fullprint << "quantile      =" << quantile << std::endl;
          fullprint << "quantile (ref)=" << distributionReference.computeQuantile(0.95) << std::endl;
          fullprint << "cdf(quantile)=" << distribution.computeCDF(quantile) << std::endl;
          NumericalPoint mean = distribution.getMean();
          fullprint << "mean      =" << mean << std::endl;
          fullprint << "mean (ref)=" << distributionReference.getMean() << std::endl;
          NumericalPoint standardDeviation = distribution.getStandardDeviation();
          fullprint << "standard deviation      =" << standardDeviation << std::endl;
          fullprint << "standard deviation (ref)=" << distributionReference.getStandardDeviation() << std::endl;
          NumericalPoint skewness = distribution.getSkewness();
          fullprint << "skewness      =" << skewness << std::endl;
          fullprint << "skewness (ref)=" << distributionReference.getSkewness() << std::endl;
          NumericalPoint kurtosis = distribution.getKurtosis();
          fullprint << "kurtosis      =" << kurtosis << std::endl;
          fullprint << "kurtosis (ref)=" << distributionReference.getKurtosis() << std::endl;
          CovarianceMatrix covariance = distribution.getCovariance();
          fullprint << "covariance      =" << covariance << std::endl;
          fullprint << "covariance (ref)=" << distributionReference.getCovariance() << std::endl;
          RandomMixture::NumericalPointWithDescriptionCollection parameters = distribution.getParametersCollection();
          fullprint << "parameters=" << parameters << std::endl;
          /*    distribution.setIntegrationNodesNumber(6);
                for (UnsignedLong i = 0; i < 6; ++i) fullprint << "standard moment n=" << i << ", value=" << distribution.getStandardMoment(i) << std::endl;*/
          fullprint << "Standard representative=" << distribution.getStandardRepresentative()->__str__() << std::endl;

          // Specific to this distribution
          NumericalPoint weights(distribution.getWeights());
          fullprint << "weights=" << weights << std::endl;
          distribution.setWeights(2.0 * weights);
          fullprint << "new weights=" << distribution.getWeights() << std::endl;
          fullprint << "blockMin=" << distribution.getBlockMin() << std::endl;
          fullprint << "blockMax=" << distribution.getBlockMax() << std::endl;
          fullprint << "maxSize=" << distribution.getMaxSize() << std::endl;
          fullprint << "alpha=" << distribution.getAlpha() << std::endl;
          fullprint << "beta=" << distribution.getBeta() << std::endl;
        }
      // Tests of the simplification mechanism
      NumericalPoint weights(0);
      Collection< Distribution > coll(0);
      coll.add(Dirac(0.5));
      weights.add(1.0);
      coll.add(Normal(1.0, 2.0));
      weights.add(2.0);
      coll.add(Normal(2.0, 1.0));
      weights.add(-3.0);
      coll.add(Uniform(-2.0, 2.0));
      weights.add(-1.0);
      coll.add(Exponential(2.0, -3.0));
      weights.add(1.5);
      RandomMixture rm(coll, weights);
      coll.add(rm);
      weights.add(-2.5);
      coll.add(Gamma(3.0, 4.0, -2.0));
      weights.add(2.5);
      RandomMixture distribution(coll, weights);
      fullprint << "distribution=" << distribution << std::endl;
      fullprint << "distribution=" << distribution.__str__() << std::endl;
      for (UnsignedLong i = 0; i < 10; ++i)
	{
	  NumericalScalar x(distribution.getMean()[0] + (-3.0 + 6.0 * i / 9.0) + distribution.getStandardDeviation()[0]);
	  fullprint << "pdf(" << x << ")=" << distribution.computePDF(x) << std::endl;
	}
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
