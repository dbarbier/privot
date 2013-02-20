//                                               -*- C++ -*-
/**
 *  @file  t_IndependentCopula_std.cxx
 *  @brief The test file of class IndependentCopula for standard methods
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

class TestObject : public IndependentCopula
{
public:
  explicit TestObject() : IndependentCopula(3) {}
  explicit TestObject(const String & name) : IndependentCopula(3)
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
      //    checkClassWithClassName<TestObject>();

      // Test some extra functionnalities
      //    checkNameFeature<TestObject>();

      // Instanciate one distribution object
      UnsignedLong dim(3);
      IndependentCopula copula(dim);
      fullprint << "Copula " << copula << std::endl;
      std::cout << "Copula " << copula << std::endl;

      // Is this copula elliptical ?
      fullprint << "Elliptical distribution= " << (copula.isElliptical() ? "true" : "false") << std::endl;

      // Is this copula continuous ?
      fullprint << "Continuous = " << (copula.isContinuous() ? "true" : "false") << std::endl;

      // Is this copula elliptical ?
      fullprint << "Elliptical = " << (copula.hasEllipticalCopula() ? "true" : "false") << std::endl;

      // Is this copula independant ?
      fullprint << "Independent = " << (copula.hasIndependentCopula() ? "true" : "false") << std::endl;

      // Test for realization of copula
      NumericalPoint oneRealization = copula.getRealization();
      fullprint << "oneRealization=" << oneRealization << std::endl;

      // Test for sampling
      UnsignedLong size = 10000;
      NumericalSample oneSample = copula.getSample( size );
      fullprint << "oneSample first=" << oneSample[0] << " last=" << oneSample[size - 1] << std::endl;
      fullprint << "mean=" << oneSample.computeMean() << std::endl;
      fullprint << "covariance=" << oneSample.computeCovariance() << std::endl;

      // Define a point
      NumericalPoint point( copula.getDimension(), 0.6 );
      fullprint << "Point= " << point << std::endl;

      // Show DDF, PDF and CDF of point
      NumericalPoint  pointDDF = copula.computeDDF( point );
      NumericalScalar pointPDF = copula.computePDF( point );
      NumericalScalar pointCDF = copula.computeCDF( point );
      fullprint << "point= " << point
                << " ddf=" << pointDDF
                << " ddf (FD)=" << copula.ContinuousDistribution::computeDDF(point)
                << " pdf=" << pointPDF
                << " cdf=" << pointCDF
                << std::endl;
      NumericalPoint PDFgr = copula.computePDFGradient( point );
      fullprint << "pdf gradient     =" << PDFgr << std::endl;
      NumericalPoint CDFgr = copula.computeCDFGradient( point );
      fullprint << "cdf gradient     =" << CDFgr << std::endl;
      NumericalPoint quantile = copula.computeQuantile( 0.95 );
      fullprint << "quantile=" << quantile << std::endl;
      fullprint << "cdf(quantile)=" << copula.computeCDF(quantile) << std::endl;
      NumericalPoint mean = copula.getMean();
      fullprint << "mean=" << mean << std::endl;
      CovarianceMatrix covariance = copula.getCovariance();
      fullprint << "covariance=" << covariance << std::endl;
      IndependentCopula::NumericalPointWithDescriptionCollection parameters = copula.getParametersCollection();
      fullprint << "parameters=" << parameters << std::endl;

      // Specific to this copula

      // Extract the marginals
      for (UnsignedLong i = 0; i < dim; i++)
        {
          Copula margin(copula.getMarginal(i));
          fullprint << "margin=" << margin << std::endl;
          fullprint << "margin PDF=" << margin.computePDF(NumericalPoint(1, 0.25)) << std::endl;
          fullprint << "margin CDF=" << margin.computeCDF(NumericalPoint(1, 0.25)) << std::endl;
          fullprint << "margin quantile=" << margin.computeQuantile(0.95) << std::endl;
          fullprint << "margin realization=" << margin.getRealization() << std::endl;
        }

      // Extract a 2-D marginal
      Indices indices(2, 0);
      indices[0] = 1;
      indices[1] = 0;
      fullprint << "indices=" << indices << std::endl;
      Copula margins(copula.getMarginal(indices));
      fullprint << "margins=" << margins << std::endl;
      fullprint << "margins PDF=" << margins.computePDF(NumericalPoint(2, 0.25)) << std::endl;
      fullprint << "margins CDF=" << margins.computeCDF(NumericalPoint(2, 0.25)) << std::endl;
      quantile = margins.computeQuantile(0.95);
      fullprint << "margins quantile=" << quantile << std::endl;
      fullprint << "margins CDF(quantile)=" << margins.computeCDF(quantile) << std::endl;
      fullprint << "margins realization=" << margins.getRealization() << std::endl;


    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
