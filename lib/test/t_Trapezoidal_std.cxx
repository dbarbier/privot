//                                               -*- C++ -*-
/**
 *  @file  t_Trapezoidal_std.cxx
 *  @brief The test file of class Trapezoidal for standard methods
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

class TestObject : public Trapezoidal
{
public:
  explicit TestObject() : Trapezoidal(1.0, 2.0, 3.0, 4.0) {}
  explicit TestObject(const String & name) : Trapezoidal(1.0, 2.0, 3.0, 4.0)
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
      Trapezoidal distribution(1.0, 1.2, 3.0, 14.0);
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
      fullprint << "skewness=" << oneSample.computeSkewnessPerComponent() << std::endl;
      fullprint << "kurtosis=" << oneSample.computeKurtosisPerComponent() << std::endl;

      // Define a point
      NumericalPoint point(distribution.getDimension(), 1.1);
      fullprint << "Point= " << point << std::endl;

      // Show PDF and CDF of point
      NumericalScalar eps(1e-5);
      NumericalPoint DDF = distribution.computeDDF( point );
      fullprint << "ddf     =" << DDF << std::endl;
      fullprint << "ddf (FD)=" << distribution.ContinuousDistribution::computeDDF(point) << std::endl;
      NumericalScalar LPDF = distribution.computeLogPDF( point );
      fullprint << "log pdf=" << LPDF << std::endl;
      NumericalScalar PDF = distribution.computePDF( point );
      fullprint << "pdf     =" << PDF << std::endl;
      fullprint << "pdf (FD)=" << (distribution.computeCDF( point + NumericalPoint(1, eps) ) - distribution.computeCDF( point  + NumericalPoint(1, -eps) )) / (2.0 * eps) << std::endl;
      NumericalScalar CDF = distribution.computeCDF( point );
      fullprint << "cdf=" << CDF << std::endl;
      NumericalScalar CCDF = distribution.computeComplementaryCDF( point );
      fullprint << "ccdf=" << CCDF << std::endl;
      NumericalComplex CF = distribution.computeCharacteristicFunction( point[0] );
      fullprint << "characteristic function=" << CF << std::endl;
      NumericalComplex LCF = distribution.computeLogCharacteristicFunction( point[0] );
      fullprint << "log characteristic function=" << LCF << std::endl;
      NumericalPoint PDFgr = distribution.computePDFGradient( point );
      fullprint << "pdf gradient     =" << PDFgr << std::endl;
      NumericalPoint PDFgrFD(4);
      PDFgrFD[0] = (Trapezoidal(distribution.getA() + eps, distribution.getB(), distribution.getC(), distribution.getD()).computePDF(point) -
                    Trapezoidal(distribution.getA() - eps, distribution.getB(), distribution.getC(), distribution.getD()).computePDF(point)) / (2.0 * eps);
      PDFgrFD[1] = (Trapezoidal(distribution.getA(), distribution.getB() + eps, distribution.getC(), distribution.getD()).computePDF(point) -
                    Trapezoidal(distribution.getA(), distribution.getB() - eps, distribution.getC(), distribution.getD()).computePDF(point)) / (2.0 * eps);
      PDFgrFD[2] = (Trapezoidal(distribution.getA(), distribution.getB(), distribution.getC() + eps, distribution.getD()).computePDF(point) -
                    Trapezoidal(distribution.getA(), distribution.getB(), distribution.getC() - eps, distribution.getD()).computePDF(point)) / (2.0 * eps);
      PDFgrFD[3] = (Trapezoidal(distribution.getA(), distribution.getB(), distribution.getC(), distribution.getD() + eps).computePDF(point) -
                    Trapezoidal(distribution.getA(), distribution.getB(), distribution.getC(), distribution.getD() - eps).computePDF(point)) / (2.0 * eps);
      fullprint << "pdf gradient (FD)=" << PDFgrFD << std::endl;
      NumericalPoint CDFgr = distribution.computeCDFGradient( point );
      fullprint << "cdf gradient     =" << CDFgr << std::endl;
      NumericalPoint CDFgrFD(4);
      CDFgrFD[0] = (Trapezoidal(distribution.getA() + eps, distribution.getB(), distribution.getC(), distribution.getD()).computeCDF(point) -
                    Trapezoidal(distribution.getA() - eps, distribution.getB(), distribution.getC(), distribution.getD()).computeCDF(point)) / (2.0 * eps);
      CDFgrFD[1] = (Trapezoidal(distribution.getA(), distribution.getB() + eps, distribution.getC(), distribution.getD()).computeCDF(point) -
                    Trapezoidal(distribution.getA(), distribution.getB() - eps, distribution.getC(), distribution.getD()).computeCDF(point)) / (2.0 * eps);
      CDFgrFD[2] = (Trapezoidal(distribution.getA(), distribution.getB(), distribution.getC() + eps, distribution.getD()).computeCDF(point) -
                    Trapezoidal(distribution.getA(), distribution.getB(), distribution.getC() - eps, distribution.getD()).computeCDF(point)) / (2.0 * eps);
      CDFgrFD[3] = (Trapezoidal(distribution.getA(), distribution.getB(), distribution.getC(), distribution.getD() + eps).computeCDF(point) -
                    Trapezoidal(distribution.getA(), distribution.getB(), distribution.getC(), distribution.getD() - eps).computeCDF(point)) / (2.0 * eps);
      fullprint << "cdf gradient (FD)=" << CDFgrFD << std::endl;
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
      Trapezoidal::NumericalPointWithDescriptionCollection parameters = distribution.getParametersCollection();
      fullprint << "parameters=" << parameters << std::endl;
      for (UnsignedLong i = 0; i < 6; ++i) fullprint << "standard moment n=" << i << ", value=" << distribution.getStandardMoment(i) << std::endl;
      fullprint << "Standard representative=" << distribution.getStandardRepresentative()->__str__() << std::endl;

      NumericalScalar roughness = distribution.getRoughness();
      fullprint << "roughness=" << roughness << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
