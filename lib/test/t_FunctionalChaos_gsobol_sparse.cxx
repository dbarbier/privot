//                                               -*- C++ -*-
/**
 *  @file  t_LeastSquaresMetaModelSelection_std.cxx
 *  @brief The test file of class LeastSquaresMetaModelSelection for standard methods
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
 *  @date   2010-02-04 16:44:49 +0100 (Thu, 04 Feb 2010)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);
  //   Log::Show( Log::Flags() | Log::INFO );

  // Problem parameters
  UnsignedLong dimension(8);

  // Reference analytical values
  NumericalScalar covTh(1.0);
  NumericalPoint a(dimension);
  a[0] = 1.0;
  a[1] = 2.0;
  a[2] = 5.0;
  a[3] = 10.0;
  a[4] = 20.0;
  a[5] = 50.0;
  a[6] = 100.0;
  a[7] = 500.0;

  // Create the gSobol function
  Description inputVariables(dimension);
  Description outputVariables(1);
  outputVariables[0] = "y";
  Description formula(1);
  formula[0] = "1.0";
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      covTh *= 1.0 + 1.0 / (3.0 * pow(1.0 + a[i], 2.0));
      inputVariables[i] = (OSS() << "xi" << i);
      formula[0] = (OSS() << formula[0] << " * ((abs(4.0 * xi" << i << " - 2.0) + " << a[i] << ") / (1.0 + " << a[i] << "))");
    }
  --covTh;

  NumericalMathFunction model(inputVariables, outputVariables, formula);

  // Create the input distribution
  Collection<Distribution> marginalX(dimension);
  for ( UnsignedLong i = 0; i < dimension; ++ i )
    {
      marginalX[i] = Uniform( 0.0, 1.0 );
    }
  ComposedDistribution distribution(marginalX);

  // Create the orthogonal basis
  Collection<OrthogonalUniVariatePolynomialFamily> polynomialCollection(dimension);
  for ( UnsignedLong i = 0; i < dimension; ++ i )
    {
      polynomialCollection[i] = LegendreFactory();
    }
  const NumericalScalar q(0.4);
  HyperbolicAnisotropicEnumerateFunction enumerateFunction(dimension, q);
  OrthogonalProductPolynomialFactory productBasis(polynomialCollection, enumerateFunction);

  // design experiment
  UnsignedLong samplingSize( 100 );

  // build basis
  UnsignedLong basisSize( 917 );

  // run algorithm
  Collection<FittingAlgorithm> listFittingAlgorithm;
  listFittingAlgorithm.add(KFold());
  listFittingAlgorithm.add(CorrectedLeaveOneOut());

  for ( UnsignedLong fittingAlgorithmIndex = 0; fittingAlgorithmIndex < listFittingAlgorithm.getSize(); ++ fittingAlgorithmIndex )
    {
      FixedStrategy adaptiveStrategy( productBasis, basisSize );
      FittingAlgorithm fittingAlgorithm( listFittingAlgorithm[fittingAlgorithmIndex] );
      LeastSquaresStrategy projectionStrategy( LowDiscrepancyExperiment(SobolSequence(dimension), samplingSize), LeastSquaresMetaModelSelectionFactory ( LAR(), fittingAlgorithm) );
      FunctionalChaosAlgorithm algo(model, distribution, adaptiveStrategy, projectionStrategy);
      RandomGenerator::SetSeed(0);
      algo.run();

      FunctionalChaosResult result(algo.getResult());
      fullprint << "coeffs = " << result.getCoefficients() << std::endl;
      fullprint << "residuals = " << result.getResiduals() << std::endl;
      fullprint << "relative errors = " << result.getRelativeErrors() << std::endl;
    }




  return ExitCode::Success;
}
