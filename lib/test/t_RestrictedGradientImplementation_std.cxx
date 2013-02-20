//                                               -*- C++ -*-
/**
 * @file  t_RestrictedGradientImplementation_std.cxx
 * @brief The test file of class RestrictedGradientImplementation for standard methods
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
      Description inputNames(3);
      inputNames[0] = "x0";
      inputNames[1] = "x1";
      inputNames[2] = "x2";
      Description outputNames(1);
      outputNames[0] = "y0";
      Description formulas(1);
      formulas[0] = "x0^2+2*x1+3*x2^3";
      AnalyticalNumericalMathEvaluationImplementation evaluation(inputNames, outputNames, formulas);
      AnalyticalNumericalMathGradientImplementation gradient(evaluation);
      Indices restrictionIndices(2);
      restrictionIndices[0] = 1;
      restrictionIndices[1] = 2;
      NumericalPoint referencePoint(3);
      referencePoint[0] = -1.0;
      referencePoint[1] =  1.0;
      referencePoint[2] =  3.0;
      RestrictedGradientImplementation restriction(gradient, restrictionIndices, referencePoint);
      fullprint << "restricted gradient=" << restriction << std::endl;
      NumericalPoint point(2);
      point[0] =  4.0;
      point[1] = -4.0;
      fullprint << "value at " << point << "=" << restriction.gradient(point) << std::endl;
      point = referencePoint;
      point[1] =  4.0;
      point[2] = -4.0;
      fullprint << "reference value=" << gradient.gradient(point) << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }

  return ExitCode::Success;
}
