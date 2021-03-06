//                                               -*- C++ -*-
/**
 *  @file  t_WhiteNoise_std.cxx
 *  @brief The test file of class WhiteNoise
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

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {

      const NumericalScalar Tmin(0.0);
      const NumericalScalar deltaT(0.1);
      const UnsignedLong N(11);

      /* Initialization of the RegularGrid timeGrid1*/
      RegularGrid timeGrid(Tmin, deltaT, N);

      /* Distribution choice */
      Distribution dist = Uniform();
      fullprint << "dist = " << dist << std::endl;

      Process process = WhiteNoise(dist);

      /* Setting the timeGrid */
      process.setTimeGrid(timeGrid);

      /* Print of the process */
      fullprint << "process = " << process << std::endl;

      /* Initialization of the TimeSeries */
      TimeSeries timeSerie = process.getRealization();

      /* Full prints */
      fullprint << "timeSerie = " << timeSerie << std::endl;

      // Some steps further
      UnsignedLong stepNumber = 4;
      fullprint << "One future=" << process.getFuture(stepNumber) << std::endl;
      UnsignedLong size = 3;
      fullprint << "Some futures=" << process.getFuture(stepNumber, size) << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
