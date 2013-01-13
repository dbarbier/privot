//                                               -*- C++ -*-
/**
 *  @file  t_Secant_std.cxx
 *  @brief The test file of class Secant for standard methods
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
      Description inputVariables(3);
      inputVariables[0] = "t";
      inputVariables[1] = "y0";
      inputVariables[2] = "y1";
      Description outputVariables(2);
      outputVariables[0] = "dy0";
      outputVariables[1] = "dy1";
      Description formulas(2);
      formulas[0] = "t - y0";
      formulas[1] = "y1 + t^2";
      NumericalMathFunction f(inputVariables, outputVariables, formulas);
      DynamicalFunction phi(TemporalFunction(f));
      RungeKutta solver(phi);
      fullprint << "ODE solver=" << solver << std::endl;
      NumericalPoint initialState(2);
      initialState[0] = -1.0;
      initialState[1] =  1.0;
      UnsignedLong nt(100);
      NumericalPoint timeGrid(nt);
      for (UnsignedLong i = 0; i < nt; ++i)
	{
	  timeGrid[i] = (i * i) / NumericalScalar((nt - 1) * (nt - 1));
	}
      NumericalSample result(solver.solve(initialState, timeGrid));
      fullprint << "result=" << result << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
