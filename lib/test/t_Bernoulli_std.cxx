//                                               -*- C++ -*-
/**
 *  @file  t_Bernoulli_std.cxx
 *  @brief The test file of class Bernoulli for standard methods
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
 *  @date   2010-02-04 16:44:49 +0100 (jeu. 04 févr. 2010)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

class TestObject : public Bernoulli
{
public:
  explicit TestObject() : Bernoulli(0.7) {}
  explicit TestObject(const String & name) : Bernoulli(0.7)
  {
    setName(name);
  }
  virtual ~TestObject() {}
};


int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);
  setRandomGenerator();

  try
    {
      // Test basic functionnalities
      checkClassWithClassName<TestObject>();

      // Test some extra functionnalities
      checkNameFeature<TestObject>();

      // Instanciate one distribution object
      Bernoulli distribution(0.7);
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
      NumericalPoint point( distribution.getDimension(), 0.0 );
      fullprint << "Point= " << point << std::endl;

      // Show PDF and CDF of point
      NumericalScalar LPDF = distribution.computeLogPDF( point );
      fullprint << "log pdf=" << LPDF << std::endl;
      NumericalScalar PDF = distribution.computePDF( point );
      fullprint << "pdf     =" << PDF << std::endl;
      fullprint << "pdf (FD)=" << (distribution.computeCDF( point + NumericalPoint(1, 0) ) - distribution.computeCDF( point  + NumericalPoint(1, -1) )) << std::endl;
      NumericalScalar CDF = distribution.computeCDF( point );
      fullprint << "cdf=" << CDF << std::endl;
      NumericalScalar CCDF = distribution.computeComplementaryCDF( point );
      fullprint << "ccdf=" << CCDF << std::endl;
      NumericalComplex CF = distribution.computeCharacteristicFunction( 0.5 );
      fullprint << "characteristic function=" << CF << std::endl;
      NumericalComplex LCF = distribution.computeLogCharacteristicFunction( 0.5 );
      fullprint << "log characteristic function=" << LCF << std::endl;
      NumericalComplex GF = distribution.computeGeneratingFunction( NumericalComplex(0.3, 0.7) );
      fullprint << "generating function=" << GF << std::endl;
      NumericalComplex LGF = distribution.computeLogGeneratingFunction( NumericalComplex(0.3, 0.7) );
      fullprint << "log generating function=" << LGF << std::endl;
      NumericalPoint quantile = distribution.computeQuantile( 0.95 );
      fullprint << "quantile=" << quantile << std::endl;
      fullprint << "cdf(quantile)=" << distribution.computeCDF(quantile) << std::endl;
      NumericalPoint mean = distribution.getMean();
      fullprint << "mean=" << mean << std::endl;
      NumericalPoint standardDeviation = distribution.getStandardDeviation();
      fullprint << "standard deviation=" << standardDeviation << std::endl;
      NumericalPoint skewness = distribution.getSkewness();
      fullprint << "skewness=" << skewness << std::endl;
      NumericalPoint kurtosis = distribution.getKurtosis();
      fullprint << "kurtosis=" << kurtosis << std::endl;
      CovarianceMatrix covariance = distribution.getCovariance();
      fullprint << "covariance=" << covariance << std::endl;
      Bernoulli::NumericalPointWithDescriptionCollection parameters = distribution.getParametersCollection();
      fullprint << "parameters=" << parameters << std::endl;
      for (UnsignedLong i = 0; i < 6; ++i) fullprint << "standard moment n=" << i << ", value=" << distribution.getStandardMoment(i) << std::endl;
      fullprint << "Standard representative=" << distribution.getStandardRepresentative()->__str__() << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
