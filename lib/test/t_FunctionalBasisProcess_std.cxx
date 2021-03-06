//                                               -*- C++ -*-
/**
 *  @file  t_FunctionalBasisProcess_std.cxx
 *  @brief The test file of class FunctionalBasisProcess
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
 *  @author lebrun
 *  @date   2011-10-26 13:54:47 +0200 (mer. 26 oct. 2011)
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

      /* Functional basis */
      UnsignedLong basisDimension(10);
      Basis basis(basisDimension);
      Collection<Distribution> coefficients(basisDimension);
      UnsignedLong oldPrecision(PlatformInfo::GetNumericalPrecision());
      PlatformInfo::SetNumericalPrecision(20);
      for (UnsignedLong i = 0; i < basisDimension; ++i)
        {
          basis[i] = NumericalMathFunction("x", String(OSS() << "sin(" << i << "*x)"));
          coefficients[i] = Normal(0.0, (1.0 + i));
        }
      PlatformInfo::SetNumericalPrecision(oldPrecision);
      Process process = FunctionalBasisProcess(ComposedDistribution(coefficients), basis);

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
