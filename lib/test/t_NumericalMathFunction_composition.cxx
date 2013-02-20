//                                               -*- C++ -*-
/**
 *  @file  t_NumericalMathFunction_composition.cxx
 *  @brief The test file of class NumericalMathFunction for standard methods
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

      /** Left hand side of the composition */
      NumericalMathFunction left("TestCompositionLeft");

      /** Right hand side of the composition */
      NumericalMathFunction right("TestCompositionRight");

      /** Compositon of left and right */
      NumericalMathFunction composed(ComposedNumericalMathFunction(left, right));

      fullprint << "right=" << right << std::endl;
      fullprint << "left=" << left << std::endl;
      fullprint << "composed=" << composed << std::endl;

      /** Does it worked? */
      NumericalPoint x(right.getInputDimension(), 1.0);
      NumericalPoint y(right(x));
      NumericalPoint z(left(y));
      Matrix Dy(right.gradient(x));
      Matrix Dz(left.gradient(y));

      fullprint << "x=" << x << " y=right(x)=" << y << " z=left(y)=" << z << std::endl;
      fullprint << "left(right(x))=" << composed(x) << std::endl;
      fullprint << "D(right)(x)=" << Dy << " D(left)(y)=" << Dz;
      fullprint << " prod=" << Dy * Dz << std::endl;
      fullprint << "D(left(right(x)))=" << composed.gradient(x) << std::endl;
      SymmetricTensor result(composed.hessian(x));
      fullprint << "DD(left(right(x)))=" << std::endl;
      for(UnsignedLong k = 0; k < result.getNbSheets(); k++)
        {
          for(UnsignedLong j = 0; j < result.getNbColumns(); j++)
            {
              for(UnsignedLong i = 0; i < result.getNbRows(); i++)
                {
                  fullprint << std::setw(14) << std::scientific << std::right << result(i, j, k);
                }
              fullprint << std::endl;
            }
          fullprint << std::endl;
        }
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
