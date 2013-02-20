//                                               -*- C++ -*-
/**
 *  @file  t_FORM_sensitivity2.cxx
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
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
      oss << std::fixed << std::setprecision(digits) << (i == 0 ? "" : ",") << ((fabs(point[i]) < eps) ? fabs(point[i]) : point[i]);
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
      NumericalMathFunction myFunction("poutre");

      UnsignedLong dim(myFunction.getInputDimension());
      /* We create a normal distribution point of dimension 1 */
      NumericalPoint mean(dim, 0.0);
      mean[0] = 50.0; // E
      mean[1] =  1.0; // F
      mean[2] = 10.0; // L
      mean[3] =  5.0; // I
      NumericalPoint sigma(dim, 1.0);
      IdentityMatrix R(dim);
      Normal myDistribution(mean, sigma, R);

      /* we name the components of the distribution */
      Description componentDescription(dim);
      componentDescription[0] = "E";
      componentDescription[1] = "F";
      componentDescription[2] = "L";
      componentDescription[3] = "I";
      myDistribution.setDescription(componentDescription);

      /* We create a 'usual' RandomVector from the Distribution */
      RandomVector vect(myDistribution);

      /* We create a composite random vector */
      RandomVector output(myFunction, vect);
      Description outputDescription(1);
      outputDescription[0] = "Vertical Displacement";
      output.setDescription(outputDescription);

      /* We create an Event from this RandomVector */
      Event myEvent(output, Less(), -3.0);

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
      fullprint << "hasoferReliabilityIndexSensitivity = " << hasoferReliabilityIndexSensitivity << std::endl;


      /* Event Probability Sensitivity */
      Analytical::Sensitivity eventProbabilitySensitivity(result.getEventProbabilitySensitivity());
      fullprint << "eventProbabilitySensitivity = " << eventProbabilitySensitivity << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
