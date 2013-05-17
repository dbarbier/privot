//                                               -*- C++ -*-
/**
 *  @file  t_Triangular_std.cxx
 *  @brief The test file of class Triangular for standard methods
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

class TestObject : public Triangular
{
public:
  explicit TestObject() : Triangular(-0.5, 1.5, 2.5) {}
  explicit TestObject(const String & name) : Triangular(-0.5, 1.5, 2.5)
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
      Triangular distribution(-0.5, 1.5, 2.5);
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

      // Define a point, left part of the support
      NumericalPoint point( distribution.getDimension(), 1.0 );
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
      NumericalPoint PDFgrFD(3);
      PDFgrFD[0] = (Triangular(distribution.getA() + eps, distribution.getM(), distribution.getB()).computePDF(point) -
                    Triangular(distribution.getA() - eps, distribution.getM(), distribution.getB()).computePDF(point)) / (2.0 * eps);
      PDFgrFD[1] = (Triangular(distribution.getA(), distribution.getM() + eps, distribution.getB()).computePDF(point) -
                    Triangular(distribution.getA(), distribution.getM() - eps, distribution.getB()).computePDF(point)) / (2.0 * eps);
      PDFgrFD[2] = (Triangular(distribution.getA(), distribution.getM(), distribution.getB() + eps).computePDF(point) -
                    Triangular(distribution.getA(), distribution.getM(), distribution.getB() - eps).computePDF(point)) / (2.0 * eps);
      fullprint << "pdf gradient (FD)=" << PDFgrFD << std::endl;
      NumericalPoint CDFgr = distribution.computeCDFGradient( point );
      fullprint << "cdf gradient     =" << CDFgr << std::endl;
      NumericalPoint CDFgrFD(3);
      CDFgrFD[0] = (Triangular(distribution.getA() + eps, distribution.getM(), distribution.getB()).computeCDF(point) -
                    Triangular(distribution.getA() - eps, distribution.getM(), distribution.getB()).computeCDF(point)) / (2.0 * eps);
      CDFgrFD[1] = (Triangular(distribution.getA(), distribution.getM() + eps, distribution.getB()).computeCDF(point) -
                    Triangular(distribution.getA(), distribution.getM() - eps, distribution.getB()).computeCDF(point)) / (2.0 * eps);
      CDFgrFD[2] = (Triangular(distribution.getA(), distribution.getM(), distribution.getB() + eps).computeCDF(point) -
                    Triangular(distribution.getA(), distribution.getM(), distribution.getB() - eps).computeCDF(point)) / (2.0 * eps);
      fullprint << "cdf gradient (FD)=" << CDFgrFD << std::endl;
      NumericalPoint quantile = distribution.computeQuantile( 0.25 );
      fullprint << "quantile=" << quantile << std::endl;
      fullprint << "cdf(quantile)=" << distribution.computeCDF(quantile) << std::endl;
      // Define a point, right part of the support
      point = NumericalPoint( distribution.getDimension(), 2.0 );
      fullprint << "Point= " << point << std::endl;

      // Show PDF and CDF of point
      DDF = distribution.computeDDF( point );
      fullprint << "ddf     =" << DDF << std::endl;
      fullprint << "ddf (FD)=" << NumericalPoint(1, (distribution.computePDF( point + NumericalPoint(1, eps) ) - distribution.computePDF( point  + NumericalPoint(1, -eps) )) / (2.0 * eps)) << std::endl;
      PDF = distribution.computePDF( point );
      fullprint << "pdf     =" << PDF << std::endl;
      fullprint << "pdf (FD)=" << (distribution.computeCDF( point + NumericalPoint(1, eps) ) - distribution.computeCDF( point  + NumericalPoint(1, -eps) )) / (2.0 * eps) << std::endl;
      CDF = distribution.computeCDF( point );
      fullprint << "cdf=" << CDF << std::endl;
      PDFgr = distribution.computePDFGradient( point );
      fullprint << "pdf gradient     =" << PDFgr << std::endl;
      PDFgrFD[0] = (Triangular(distribution.getA() + eps, distribution.getM(), distribution.getB()).computePDF(point) -
                    Triangular(distribution.getA() - eps, distribution.getM(), distribution.getB()).computePDF(point)) / (2.0 * eps);
      PDFgrFD[1] = (Triangular(distribution.getA(), distribution.getM() + eps, distribution.getB()).computePDF(point) -
                    Triangular(distribution.getA(), distribution.getM() - eps, distribution.getB()).computePDF(point)) / (2.0 * eps);
      PDFgrFD[2] = (Triangular(distribution.getA(), distribution.getM(), distribution.getB() + eps).computePDF(point) -
                    Triangular(distribution.getA(), distribution.getM(), distribution.getB() - eps).computePDF(point)) / (2.0 * eps);
      fullprint << "pdf gradient (FD)=" << PDFgrFD << std::endl;
      CDFgr = distribution.computeCDFGradient( point );
      fullprint << "cdf gradient     =" << CDFgr << std::endl;
      CDFgrFD[0] = (Triangular(distribution.getA() + eps, distribution.getM(), distribution.getB()).computeCDF(point) -
                    Triangular(distribution.getA() - eps, distribution.getM(), distribution.getB()).computeCDF(point)) / (2.0 * eps);
      CDFgrFD[1] = (Triangular(distribution.getA(), distribution.getM() + eps, distribution.getB()).computeCDF(point) -
                    Triangular(distribution.getA(), distribution.getM() - eps, distribution.getB()).computeCDF(point)) / (2.0 * eps);
      CDFgrFD[2] = (Triangular(distribution.getA(), distribution.getM(), distribution.getB() + eps).computeCDF(point) -
                    Triangular(distribution.getA(), distribution.getM(), distribution.getB() - eps).computeCDF(point)) / (2.0 * eps);
      fullprint << "cdf gradient (FD)=" << CDFgrFD << std::endl;
      quantile = distribution.computeQuantile( 0.95 );
      fullprint << "quantile=" << quantile << std::endl;
      fullprint << "cdf(quantile)=" << distribution.computeCDF(quantile) << std::endl;
      // Moments
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
      Triangular::NumericalPointWithDescriptionCollection parameters = distribution.getParametersCollection();
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