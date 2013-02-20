//                                               -*- C++ -*-
/**
 *  @file  t_FORM_sensitivity.cxx
 *  @brief The test file of FORM class
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

String printNumericalPoint(const NumericalPoint & point, const UnsignedLong digits)
{
  OSS oss;
  oss << "[";
  NumericalScalar eps(pow(0.1, digits));
  for (UnsignedLong i = 0; i < point.getDimension(); i++)
    {
      oss << std::fixed << std::setprecision(digits) << (i == 0 ? "" : ",") << Bulk<double>((fabs(point[i]) < eps) ? fabs(point[i]) : point[i]);
    }
  oss << "]";
  return oss;
}

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {

      /* We create a numerical math function */
      /** Analytical construction */
      Description inputFunc(2);
      inputFunc[0] = "x0";
      inputFunc[1] = "x1";
      Description outputFunc(1);
      outputFunc[0] = "y0";
      Description formulas(outputFunc.getSize());
      formulas[0] = "-(6+x0^2-x1)";
      fullprint << "formulas=" << formulas << std::endl;
      NumericalMathFunction myFunction(inputFunc, outputFunc, formulas);

      UnsignedLong dim(myFunction.getInputDimension());
      /* We create a normal distribution point of dimension 1 */
      NumericalPoint mean(dim, 0.0);
      mean[0] = 5.0; // x0
      mean[1] = 2.1; // x1
      NumericalPoint sigma(dim, 0.0);
      sigma[0] = 3.3; // x0
      sigma[1] = 3.0; // x1
      IdentityMatrix R(dim);
      ComposedDistribution::DistributionCollection testDistributions(2);
      testDistributions[0] = Normal(mean, sigma, R);
      ComposedDistribution::DistributionCollection marginals(2);
      marginals[0] = testDistributions[0].getMarginal(0);
      marginals[1] = testDistributions[0].getMarginal(1);
      testDistributions[1] = ComposedDistribution(marginals);
      for (UnsignedLong i = 0; i < 2; i ++)
        {
          Distribution myDistribution(testDistributions[i]);
          /* we name the components of the distribution */
          Description componentDescription(dim);
          componentDescription[0] = "Marginal 1";
          componentDescription[1] = "Marginal 2";
          myDistribution.setDescription(componentDescription);

          /* We create a 'usual' RandomVector from the Distribution */
          RandomVector vect(myDistribution);

          /* We create a composite random vector */
          RandomVector output(myFunction, vect);
          Description outputDescription(dim);
          outputDescription[0] = "Interest Variable 1";
          output.setDescription(outputDescription);

          /* We create an Event from this RandomVector */
          Event myEvent(output, Greater(), 0.0, "Event 1");

          /* We create a NearestPoint algorithm */
          Cobyla myCobyla;
          myCobyla.setSpecificParameters(CobylaSpecificParameters());
          myCobyla.setMaximumIterationsNumber(100);
          myCobyla.setMaximumAbsoluteError(1.0e-10);
          myCobyla.setMaximumRelativeError(1.0e-10);
          myCobyla.setMaximumResidualError(1.0e-10);
          myCobyla.setMaximumConstraintError(1.0e-10);
          fullprint << "myCobyla=" << myCobyla << std::endl << std::flush;

          /* We create a FORM algorithm */
          /* The first parameter is a NearestPointAlgorithm */
          /* The second parameter is an event */
          /* The third parameter is a starting point for the design point research */
          FORM myAlgo(myCobyla, myEvent, mean);

          fullprint << "FORM=" << myAlgo << std::endl;

          /* Perform the simulation */
          myAlgo.run();

          /* Stream out the result */
          FORMResult result(myAlgo.getResult());
          UnsignedLong digits(5);
          fullprint << "importance factors=" << printNumericalPoint(result.getImportanceFactors(), digits) << std::endl;
          fullprint << "importance factors (classical)=" << printNumericalPoint(result.getImportanceFactors(true), digits) << std::endl;
          fullprint << "Hasofer reliability index=" << std::setprecision(digits) << result.getHasoferReliabilityIndex() << std::endl;
          fullprint << "result=" << result << std::endl;

          /* Hasofer Reliability Index Sensitivity */
          Analytical::Sensitivity hasoferReliabilityIndexSensitivity(result.getHasoferReliabilityIndexSensitivity());
          // Display only marginal sensitivities
          fullprint << "hasoferReliabilityIndexSensitivity = " << hasoferReliabilityIndexSensitivity << std::endl;


          /* Event Probability Sensitivity */
          Analytical::Sensitivity eventProbabilitySensitivity(result.getEventProbabilitySensitivity());
          fullprint << "eventProbabilitySensitivity = " << eventProbabilitySensitivity << std::endl;
        }
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
