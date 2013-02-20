//                                               -*- C++ -*-
/**
 *  @file  t_LinearTaylor_std.cxx
 *  @brief The test file of class LinearTaylor for standard methods
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
      NumericalScalar eps(0.2);
      /** Instance creation */
      NumericalMathFunction myFunc("TestResponseSurface");
      NumericalPoint center(myFunc.getInputDimension());
      for(UnsignedLong i = 0; i < center.getDimension(); i++)
        {
          center[i] = 1.0 + i;
        }
      LinearTaylor myTaylor(center, myFunc);
      myTaylor.run();
      NumericalMathFunction responseSurface(myTaylor.getResponseSurface());
      fullprint << "myTaylor=" << myTaylor << std::endl;
      fullprint << "responseSurface=" << responseSurface << std::endl;
      fullprint << "myFunc(" << center << ")=" << myFunc(center) << std::endl;
      fullprint << "responseSurface(" << center << ")=" << responseSurface(center) << std::endl;
      NumericalPoint in(center);
      in[0] += eps;
      in[1] -= eps / 2;
      fullprint << "myFunc(" << in << ")=" << myFunc(in) << std::endl;
      fullprint << "responseSurface(" << in << ")=" << responseSurface(in) << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
