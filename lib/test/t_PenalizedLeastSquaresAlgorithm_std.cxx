//                                               -*- C++ -*-
/**
 *  @file  t_PenalizedLeastSquaresAlgorithm_std.cxx
 *  @brief The test file of class PenalizedLeastSquaresAlgorithm for the standard methods
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
 *  @date   2007-08-01 16:47:51 +0200 (mer, 01 aoû 2007)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {
      UnsignedLong dimension(2);
      // Reference function
      Description inVar(dimension);
      inVar[0] = "x1";
      inVar[1] = "x2";
      Description outVar(1);
      outVar[0] = "y";
      Description formula(1);
      formula[0] = "x1^3+1.5*x2^3-x1*x2";
      NumericalMathFunction model(inVar, outVar, formula);

      // Basis upon which we will project the model
      PenalizedLeastSquaresAlgorithm::NumericalMathFunctionCollection basis(4);
      formula[0] = "x1";
      basis[0] = NumericalMathFunction(inVar, outVar, formula);
      formula[0] = "x2";
      basis[1] = NumericalMathFunction(inVar, outVar, formula);
      formula[0] = "x1^2";
      basis[2] = NumericalMathFunction(inVar, outVar, formula);
      formula[0] = "x2^2";
      basis[3] = NumericalMathFunction(inVar, outVar, formula);

      // Input sample
      UnsignedLong size(5);
      NumericalSample inputSample(size * size, dimension);
      NumericalPoint weight(inputSample.getSize());
      for (UnsignedLong i = 0; i < inputSample.getSize(); ++i)
        {
          inputSample[i][0] = NumericalScalar(i % size) / size;
          inputSample[i][1] = NumericalScalar (i / size) / size;
          weight[i] = (i % size + 1) * (i / size + 1);
        }
      NumericalScalar penalizationFactor(0.25);
      // Uniform weight, no penalization
      {
        PenalizedLeastSquaresAlgorithm algo(inputSample, model(inputSample), NumericalPoint(inputSample.getSize(), 1.0), basis);
        fullprint << "Uniform weight, no penalization" << std::endl;
        fullprint << "Coefficients=" << algo.getCoefficients() << std::endl;
        fullprint << "Residual=" << algo.getResidual() << std::endl;
        fullprint << "Relative error=" << algo.getRelativeError() << std::endl;
      }
      // Uniform weight, spherical penalization
      {
        PenalizedLeastSquaresAlgorithm algo(inputSample, model(inputSample), NumericalPoint(inputSample.getSize(), 1.0), basis, penalizationFactor);
        fullprint << "Uniform weight, spherical penalization" << std::endl;
        fullprint << "Coefficients=" << algo.getCoefficients() << std::endl;
        fullprint << "Residual=" << algo.getResidual() << std::endl;
      }
      // Non uniform weight, no penalization
      {
        PenalizedLeastSquaresAlgorithm algo(inputSample, model(inputSample), weight, basis);
        fullprint << "Non uniform weight, no penalization" << std::endl;
        fullprint << "Coefficients=" << algo.getCoefficients() << std::endl;
        fullprint << "Residual=" << algo.getResidual() << std::endl;
      }
      // Non uniform weight, spherical penalization
      {
        PenalizedLeastSquaresAlgorithm algo(inputSample, model(inputSample), weight, basis, penalizationFactor);
        fullprint << "Non uniform weight, spherical penalization" << std::endl;
        fullprint << "Coefficients=" << algo.getCoefficients() << std::endl;
        fullprint << "Residual=" << algo.getResidual() << std::endl;
      }
      // Non uniform weight, non spherical penalization
      {
        CovarianceMatrix penalizationMatrix(4);
        for (UnsignedLong i = 0; i < 4; ++i)
          penalizationMatrix(i, i) = 1.0;
        for (UnsignedLong i = 0; i < 3; ++i)
          penalizationMatrix(i, i + 1) = 1.0 / 8.0;
        PenalizedLeastSquaresAlgorithm algo(inputSample, model(inputSample), weight, basis, penalizationFactor, penalizationMatrix);
        fullprint << "Non uniform weight, non spherical penalization" << std::endl;
        fullprint << "Coefficients=" << algo.getCoefficients() << std::endl;
        fullprint << "Residual=" << algo.getResidual() << std::endl;
      }
    }

  catch (OT::NotSymmetricDefinitePositiveException & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }

  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }

  return ExitCode::Success;
}
