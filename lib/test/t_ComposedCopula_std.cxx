//                                               -*- C++ -*-
/**
 *  @file  t_ComposedCopula_std.cxx
 *  @brief The test file of class ComposedCopula for standard methods
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
      // Instanciate one distribution object
      CorrelationMatrix R(3);
      R(0, 1) = 0.5;
      R(0, 2) = 0.25;
      ComposedCopula::CopulaCollection collection(3);
      collection[0] = FrankCopula(3.0);
      collection[1] = NormalCopula(R);
      collection[2] = ClaytonCopula(2.0);
      ComposedCopula copula(collection);
      fullprint << "Copula " << copula << std::endl;
      std::cout << "Copula " << copula << std::endl;

      // Is this copula elliptical ?
      fullprint << "Elliptical distribution= " << (copula.isElliptical() ? "true" : "false") << std::endl;

      // Is this copula continuous ?
      fullprint << "Continuous = " << (copula.isContinuous() ? "true" : "false") << std::endl;

      // Is this copula elliptical ?
      fullprint << "Elliptical = " << (copula.hasEllipticalCopula() ? "true" : "false") << std::endl;

      // Is this copula independent ?
      fullprint << "Independent = " << (copula.hasIndependentCopula() ? "true" : "false") << std::endl;

      // Test for realization of copula
      NumericalPoint oneRealization = copula.getRealization();
      fullprint << "oneRealization=" << oneRealization << std::endl;

      // Test for sampling
      UnsignedLong size = 10000;
      NumericalSample oneSample = copula.getSample( size );
      fullprint << "oneSample first=" << oneSample[0] << " last=" << oneSample[size - 1] << std::endl;
      fullprint << "mean=" << oneSample.computeMean() << std::endl;
      UnsignedLong precision(PlatformInfo::GetNumericalPrecision());
      PlatformInfo::SetNumericalPrecision(5);
      fullprint << "covariance=" << oneSample.computeCovariance() << std::endl;
      PlatformInfo::SetNumericalPrecision(precision);

      // Define a point
      NumericalPoint point( copula.getDimension(), 0.6 );
      fullprint << "Point= " << point << std::endl;

      // Show PDF and CDF of point
      NumericalScalar eps(1e-5);
      NumericalPoint DDF = copula.computeDDF( point );
      fullprint << "ddf     =" << DDF << std::endl;
      NumericalPoint ddfFD(copula.getDimension());
      fullprint << "ddf (FD)=" << copula.ContinuousDistribution::computeDDF(point) << std::endl;
      NumericalScalar PDF = copula.computePDF( point );
      fullprint << "pdf     =" << PDF << std::endl;
      NumericalScalar CDF = copula.computeCDF( point );
      fullprint << "cdf=" << CDF << std::endl;
      //    NumericalPoint PDFgr = copula.computePDFGradient( point );
      //    fullprint << "pdf gradient     =" << PDFgr << std::endl;
      //    NumericalPoint CDFgr = copula.computeCDFGradient( point );
      //    fullprint << "cdf gradient     =" << CDFgr << std::endl;
      NumericalPoint quantile = copula.computeQuantile( 0.95 );
      fullprint << "quantile=" << quantile << std::endl;
      fullprint << "cdf(quantile)=" << copula.computeCDF(quantile) << std::endl;
      NumericalPoint mean = copula.getMean();
      fullprint << "mean=" << mean << std::endl;
      CovarianceMatrix covariance = copula.getCovariance();
      precision = PlatformInfo::GetNumericalPrecision();
      PlatformInfo::SetNumericalPrecision(5);
      fullprint << "covariance=" << covariance << std::endl;
      PlatformInfo::SetNumericalPrecision(precision);
      ComposedCopula::NumericalPointWithDescriptionCollection parameters = copula.getParametersCollection();
      fullprint << "parameters=" << parameters << std::endl;

      // Specific to this copula

      // Extract a 5-D marginal
      UnsignedLong dim(5);
      point = NumericalPoint(dim, 0.25);
      Indices indices(dim, 0);
      indices[0] = 1;
      indices[1] = 2;
      indices[2] = 3;
      indices[3] = 5;
      indices[4] = 6;
      fullprint << "indices=" << indices << std::endl;
      Copula margins(copula.getMarginal(indices));
      fullprint << "margins=" << margins << std::endl;
      fullprint << "margins PDF=" << margins.computePDF(point) << std::endl;
      fullprint << "margins CDF=" << margins.computeCDF(point) << std::endl;
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
