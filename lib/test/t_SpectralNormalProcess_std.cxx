//                                               -*- C++ -*-
/**
 *  @file  t_SpectralNormalProcess_std.cxx
 *  @brief The test file of class Spectral Normal Process
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

      RandomGenerator::SetSeed(0);

      /* Default dimension parameter to evaluate the model */
      const UnsignedLong defaultDimension(3);

      /* Amplitude values */
      NumericalPoint amplitude(defaultDimension, 1.0);
      /* Scale values */
      NumericalPoint scale(defaultDimension, 1.0);

      /* Second order model with parameters */
      ExponentialCauchy myModel(amplitude, scale);

      /* checking the copy-cast*/
      SecondOrderModel mySecondOrderModel(myModel);

      const UnsignedLong points(8);
      const NumericalScalar tMin(0.0);
      const NumericalScalar tStep(1.0 / (points - 1));

      // RegularGrid --> Build list of frequencies using the RegularGrid
      RegularGrid myTimeGrid(tMin, tStep, points);

      SpectralNormalProcess mySpectralProcess0(myModel, myTimeGrid);

      std::cout << "mySpectralProcess0 = " << mySpectralProcess0 << std::endl;
      std::cout << "Realization = " << mySpectralProcess0.getRealization() << std::endl;

      // Constructor using maximalFrequency value and size of discretization
      const NumericalScalar maximalFrequency(10.0);
      SpectralNormalProcess mySpectralProcess1(myModel, maximalFrequency, points);
      RegularGrid tg(mySpectralProcess1.getTimeGrid());

      std::cout << "mySpectralProcess1 = " << mySpectralProcess1 << std::endl;
      std::cout << "Realization = " << mySpectralProcess1.getRealization() << std::endl;

      /* Second order model with parameters */
      CauchyModel mySpecModel(amplitude, scale);
      std::cout << "mySpecModel = " << mySpecModel << std::endl;

      SpectralNormalProcess mySpectralProcess2(mySpecModel, myTimeGrid);
      std::cout << "mySpectralProcess2 = " << mySpectralProcess2 << std::endl;
      std::cout << "Realization = " << mySpectralProcess2.getRealization() << std::endl;
      SpectralNormalProcess mySpectralProcess3(mySpecModel, maximalFrequency, points);
      std::cout << "mySpectralProcess3 = " << mySpectralProcess3 << std::endl;
      std::cout << "Realization = " << mySpectralProcess3.getRealization() << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
