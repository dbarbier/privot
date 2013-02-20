//                                               -*- C++ -*-
/**
 *  @file  t_SORM_std.cxx
 *  @brief The test file of SORM class
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

      /* We create a 'usual' RandomVector from the Distribution */
      RandomVector vect(myDistribution);

      /* We create a composite random vector */
      RandomVector output(myFunction, vect);

      /* We create an Event from this RandomVector */
      Event myEvent(output, Less(), -3.0);

      /* We create a NearestPoint algorithm */
      AbdoRackwitz myAbdoRackwitz;
      myAbdoRackwitz.setMaximumIterationsNumber(100);
      myAbdoRackwitz.setMaximumAbsoluteError(1.0e-10);
      myAbdoRackwitz.setMaximumRelativeError(1.0e-10);
      myAbdoRackwitz.setMaximumResidualError(1.0e-10);
      myAbdoRackwitz.setMaximumConstraintError(1.0e-10);
      myAbdoRackwitz.setSpecificParameters(AbdoRackwitzSpecificParameters());
      fullprint << "myAbdoRackwitz=" << myAbdoRackwitz << std::endl;

      /* We create a SORM algorithm */
      /* The first parameter is a NearestPointAlgorithm */
      /* The second parameter is an event */
      /* The third parameter is a starting point for the design point research */
      SORM myAlgo(myAbdoRackwitz, myEvent, mean);

      /* Perform the simulation */
      myAlgo.run();

      /* Stream out the result */
      SORMResult result(myAlgo.getResult());
      UnsignedLong digits(4);
      fullprint << "Breitung event probability=" << result.getEventProbabilityBreitung() << std::endl;
      fullprint << "Breitung generalized reliability index=" << std::setprecision(digits) << result.getGeneralisedReliabilityIndexBreitung() << std::endl;
      fullprint << "HohenBichler event probability=" << std::setprecision(digits) << result.getEventProbabilityHohenBichler() << std::endl;
      fullprint << "HohenBichler generalized reliability index=" << std::setprecision(digits) << result.getGeneralisedReliabilityIndexHohenBichler() << std::endl;
      fullprint << "Tvedt event probability=" << std::setprecision(digits) << result.getEventProbabilityTvedt() << std::endl;
      fullprint << "Tvedt generalized reliability index=" << std::setprecision(digits) << result.getGeneralisedReliabilityIndexTvedt() << std::endl;
      fullprint << "sorted curvatures=" << printNumericalPoint(result.getSortedCurvatures(), digits) << std::endl;
      fullprint << "standard space design point=" << printNumericalPoint(result.getStandardSpaceDesignPoint(), digits) << std::endl;
      fullprint << "physical space design point=" << printNumericalPoint(result.getPhysicalSpaceDesignPoint(), digits) << std::endl;
      fullprint << "is standard point origin in failure space? " << (result.getIsStandardPointOriginInFailureSpace() ? "true" : "false") << std::endl;
      fullprint << "importance factors=" << printNumericalPoint(result.getImportanceFactors(), digits) << std::endl;
      fullprint << "importance factors (classical)=" << printNumericalPoint(result.getImportanceFactors(true), digits) << std::endl;
      fullprint << "Hasofer reliability index=" << std::setprecision(digits) << result.getHasoferReliabilityIndex() << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
