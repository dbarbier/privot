//                                               -*- C++ -*-
/**
 *  @file  t_TemporalNormalProcess_std.cxx
 *  @brief The test file of class Temporal Normal Process
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

      /* Default dimension parameter to evaluate the model */
      const UnsignedLong defaultDimension(1);

      /* Amplitude values */
      NumericalPoint amplitude(defaultDimension, 1.0);
      /* Scale values */
      NumericalPoint scale(defaultDimension, 1.0);

      /* Second order model with parameters */
      ExponentialCauchy myModel(amplitude, scale);
      fullprint << "myModel = " << myModel << std::endl;

      /* checking the copy-cast*/
      SecondOrderModel mySecondOrderModel(myModel);
      fullprint << "mySecondOrderModel = " << mySecondOrderModel << std::endl;

      const NumericalScalar tmin(0.0);
      const NumericalScalar step(0.1);
      const UnsignedLong n(11);

      RegularGrid myTimeGrid(tmin, step, n);
      const UnsignedLong size(100);

      TemporalNormalProcess myProcess(myModel, myTimeGrid);
      fullprint << "myProcess = " << myProcess << std::endl;
      fullprint << "mean over " << size << " realizations = " << myProcess.getSample(size).computeMean() << std::endl;

      /* Second order model with parameters */
      ExponentialModel myCovModel(amplitude, scale);
      fullprint << "myCovModel = " << myCovModel << std::endl;

      TemporalNormalProcess myProcess1(myCovModel, myTimeGrid);
      fullprint << "myProcess1 = " << myProcess1 << std::endl;
      fullprint << "mean over " << size << " realizations= " << myProcess1.getSample(size).computeMean() << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
