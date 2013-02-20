//                                               -*- C++ -*-
/**
 *  @file  t_BoxCoxFactory_std.cxx
 *  @brief The test file of class BoxCoxFactory for standard methods
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

  try
    {
      // Fix the realization as a Normal
      // Parameters are done such as the values are positive
      const NumericalScalar epsilon(0.01);
      Distribution noiseDistribution(Normal(0, epsilon));

      // TimeGrid parameters
      const UnsignedLong n(101);
      const NumericalScalar timeStart(0.0);
      const NumericalScalar timeStep(0.1);
      RegularGrid timeGrid(timeStart, timeStep, n);

      // White noise
      const WhiteNoise whiteNoise(noiseDistribution, timeGrid);

      // Now instantiation of a ARMA process
      const NumericalPoint arParameters(1, 0.1);
      const NumericalPoint maParameters(1, 5);
      const ARMACoefficients arCoefficients(arParameters);
      const ARMACoefficients maCoefficients(maParameters);

      // Create the ARMA process
      ARMA armaProcess(arCoefficients, maCoefficients, whiteNoise);

      // 1 realization of the process
      const TimeSeries realization(armaProcess.getRealization());

      // We have to translate manually in order to use the BoxCox evaluation on positive values
      NumericalSample sample(realization.getSample());

      const NumericalScalar alpha(-sample.getMin()[0] + 1.0e-4);

      // Now we build the factory using the alpha parameter
      BoxCoxFactory factory(alpha);

      // Evaluation of the BoxCoxTransfotm
      BoxCoxTransform myBoxCox(*(factory.build(realization)));

      fullprint << "myBoxCox = " << myBoxCox << std::endl;

      // We translate the input sample using the alpha value
      sample.translate(NumericalPoint(1, alpha));

      // We build a new TimeSeries
      const TimeSeries translateTimeSerie(timeGrid, sample);

      // Evaluation of the factory result on the new time series
      PlatformInfo::SetNumericalPrecision(5);
      fullprint << "input time series=" << translateTimeSerie << std::endl;
      fullprint << "f(time series) = " << myBoxCox(translateTimeSerie) << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
