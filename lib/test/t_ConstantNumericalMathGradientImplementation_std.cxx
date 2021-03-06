//                                               -*- C++ -*-
/**
 *  @file  t_ConstantNumericalMathGradientImplementation_std.cxx
 *  @brief The test file of class ConstantNumericalMathGradientImplementation for standard methods
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
      UnsignedLong inputDimension(3);
      UnsignedLong outputDimension(2);
      // Constant term
      Matrix constant(inputDimension, outputDimension);
      constant(0, 0) = 1.0;
      constant(1, 0) = 2.0;
      constant(2, 0) = 5.0;
      constant(0, 1) = 7.0;
      constant(1, 1) = 9.0;
      constant(2, 1) = 3.0;

      ConstantNumericalMathGradientImplementation myGradient(constant);
      myGradient.setName("constantGradient");
      NumericalPoint inPoint(inputDimension);
      inPoint[0] = 7.0;
      inPoint[1] = 8.0;
      inPoint[2] = 9.0;
      Matrix outMatrix = myGradient.gradient( inPoint );
      fullprint << "myGradient=" << myGradient << std::endl;
      fullprint << myGradient.getName() << "( " << inPoint << " ) = " << outMatrix << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }

  return ExitCode::Success;
}
