//                                               -*- C++ -*-
/**
 *  @file  t_LAR_std.cxx
 *  @brief The test file of class LAR for standard methods
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
  UnsignedLong dimension(3);
  NumericalScalar a(7.0);
  NumericalScalar b(0.1);
  // Reference analytical values
  NumericalScalar covTh((pow(b, 2) * pow(M_PI, 8)) / 18.0 + (b * pow(M_PI, 4)) / 5.0 + (pow(a, 2)) / 8.0 + 1.0 / 2.0);
  NumericalPoint sob_1(3);
  sob_1[0] = (b * pow(M_PI, 4) / 5.0 + pow(b, 2) * pow(M_PI, 8) / 50.0 + 1.0 / 2.0) / covTh;
  sob_1[1] = (pow(a, 2) / 8.0) / covTh;
  sob_1[2] = 0.0;
  NumericalPoint sob_2(3);
  sob_2[0] = 0.0;
  sob_2[1] = (pow(b, 2) * pow(M_PI, 8) / 18.0 - pow(b, 2) * pow(M_PI, 8) / 50.0) / covTh;
  sob_2[2] = 0.0;
  NumericalPoint sob_3(1, 0.0);
  NumericalPoint sob_T1(3);
  sob_T1[0] = sob_1[0] + sob_2[0] + sob_2[1] + sob_3[0];
  sob_T1[1] = sob_1[1] + sob_2[0] + sob_2[2] + sob_3[0];
  sob_T1[2] = sob_1[2] + sob_2[1] + sob_2[2] + sob_3[0];
  NumericalPoint sob_T2(3);
  sob_T2[0] = sob_2[0] + sob_2[1] + sob_3[0];
  sob_T2[1] = sob_2[0] + sob_2[2] + sob_3[0];
  sob_T2[2] = sob_2[1] + sob_2[2] + sob_3[0];
  NumericalPoint sob_T3(sob_3);
  // Create the Ishigami function
  Description inputVariables(dimension);
  inputVariables[0] = "xi1";
  inputVariables[1] = "xi2";
  inputVariables[2] = "xi3";
  Description outputVariables(1);
  outputVariables[0] = "y";
  Description formula(1);
  formula[0] = (OSS() << "sin(xi1) + (" << a << ") * (sin(xi2)) ^ 2 + (" << b << ") * xi3^4 * sin(xi1)");
  NumericalMathFunction model(inputVariables, outputVariables, formula);

  // Create the input distribution
  Collection<Distribution> marginalX(dimension);
  marginalX[0] = Uniform(-M_PI, M_PI);
  marginalX[1] = Uniform(-M_PI, M_PI);
  marginalX[2] = Uniform(-M_PI, M_PI);
  ComposedDistribution distribution(marginalX);

  // Create the orthogonal basis
  Collection<OrthogonalUniVariatePolynomialFamily> polynomialCollection(dimension);
  polynomialCollection[0] = LegendreFactory();
  polynomialCollection[1] = LegendreFactory();
  polynomialCollection[2] = LegendreFactory();
  LinearEnumerateFunction enumerateFunction(dimension);
  OrthogonalProductPolynomialFactory productBasis(polynomialCollection, enumerateFunction);

  // design experiment
  UnsignedLong samplingSize(75);
  Experiment experiment(LowDiscrepancyExperiment(SobolSequence(dimension), distribution, samplingSize));

  // build the isoprobabilistic transformation
  Collection<Distribution> marginalZ;
  for (UnsignedLong i = 0; i < dimension; ++ i )
    {
      marginalZ.add(productBasis.getMeasure().getMarginal(i));
    }
  MarginalTransformationEvaluation evaluationT(MarginalTransformationEvaluation(marginalX, marginalZ));
  MarginalTransformationGradient gradientT(evaluationT);
  MarginalTransformationHessian hessianT(evaluationT);
  NumericalMathFunction xToU(evaluationT.clone(), gradientT.clone(), hessianT.clone());

  // generate samples
  NumericalSample x( experiment.generate() );
  NumericalSample u( xToU(x) );
  NumericalSample y( model(x) );

  // build basis
  UnsignedLong degree = 10;
  UnsignedLong basisSize = enumerateFunction.getStrataCumulatedCardinal(degree);
  Basis psi;
  for ( UnsignedLong i = 0; i < basisSize; ++ i )
    {
      psi.add(productBasis.build(i));
    }

  // run algorithm
  LAR factory;
  factory.setVerbose(true);
  fullprint << "factory = " << factory << std::endl;

  BasisSequence seq = factory.build( u, y, psi );

  UnsignedLong first = 20;
  if ( seq.getSize() >= first )
    fullprint << "first " << first << " indices = " << seq[first - 1] << std::endl;

  return ExitCode::Success;
}
