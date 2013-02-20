//                                               -*- C++ -*-
/**
 *  @file  t_CompositeProcess_std.cxx
 *  @brief The test file of class CompositeProcess
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
 *  @date   2011-12-05 17:31:12 +0100 (lun. 05 déc. 2011)
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

      /*2 D case */
      RandomGenerator::SetSeed(0);


      /* Time grid creation */
      NumericalScalar Tmin(0.0);
      NumericalScalar deltaT(0.1);
      UnsignedLong steps(11);

      /* Initialization of the time grid timeGrid1*/
      RegularGrid timeGrid(Tmin, deltaT, steps);

      /* Default ARMA creation */
      ARMA myARMAProcess;

      /* We fix the time grid to the ARMA process */
      myARMAProcess.setTimeGrid(timeGrid);

      fullprint << "myAntecedent = " << myARMAProcess << std::endl;


      /* We build a 1D function */
      Description inputDescription(1, "x");
      Description formula(1, "2 * x + 5");
      NumericalMathFunction myOneDimensionalFunction(inputDescription, formula);

      /* We build a spatial function */
      SpatialFunction myFunction(myOneDimensionalFunction);

      /* Definition of the CompositeProcess */
      CompositeProcess myCompositeProcess(myFunction, myARMAProcess);
      fullprint << "myCompositeProcess =" << myCompositeProcess << std::endl;

      // We get a TimeSeries as realization of the CompositeProcess
      const TimeSeries realization(myCompositeProcess.getRealization());
      fullprint << "One ARMA realization=" << realization << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
