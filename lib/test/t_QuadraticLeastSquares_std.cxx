//                                               -*- C++ -*-
/**
 *  @file  t_QuadraticLeastSquares_std.cxx
 *  @brief The test file of class QuadraticLeastSquares for standard methods
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
      NumericalMathFunction myFunc("TestResponseSurface");
      NumericalSample data(9, myFunc.getInputDimension());
      NumericalPoint point(myFunc.getInputDimension());
      point[0] = 0.5;
      point[1] = 0.5;
      data[0] = point;
      point[0] = -1;
      point[1] = -1;
      data[1] = point;
      point[0] = -1;
      point[1] = 1;
      data[2] = point;
      point[0] = 1;
      point[1] = -1;
      data[3] = point;
      point[0] = 1;
      point[1] = 1;
      data[4] = point;
      point[0] = -0.5;
      point[1] = -0.5;
      data[5] = point;
      point[0] = -0.5;
      point[1] = 0.5;
      data[6] = point;
      point[0] = 0.5;
      point[1] = -0.5;
      data[7] = point;
      point[0] = 0.5;
      point[1] = 0.5;
      data[8] = point;
      {
        QuadraticLeastSquares myLeastSquares(data, myFunc);
        myLeastSquares.run();
        NumericalMathFunction responseSurface(myLeastSquares.getResponseSurface());
        fullprint << "myLeastSquares=" << myLeastSquares << std::endl;
        fullprint << "responseSurface=" << responseSurface << std::endl;
        NumericalPoint in(myFunc.getInputDimension(), 0.2);
        fullprint << "myFunc(" << in << ")=" << myFunc(in) << std::endl;
        fullprint << "responseSurface(" << in << ")=" << responseSurface(in) << std::endl;
      }
      {
        NumericalSample dataOut(myFunc(data));
        QuadraticLeastSquares myLeastSquares(data, dataOut);
        myLeastSquares.run();
        NumericalMathFunction responseSurface(myLeastSquares.getResponseSurface());
        fullprint << "myLeastSquares=" << myLeastSquares << std::endl;
        fullprint << "responseSurface=" << responseSurface << std::endl;
        NumericalPoint in(myFunc.getInputDimension(), 0.2);
        fullprint << "myFunc(" << in << ")=" << myFunc(in) << std::endl;
        fullprint << "responseSurface(" << in << ")=" << responseSurface(in) << std::endl;
      }
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
