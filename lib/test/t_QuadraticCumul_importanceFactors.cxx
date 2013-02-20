//                                               -*- C++ -*-
/**
 *  @file  t_QuadraticCumul_importanceFactors.cxx
 *  @brief The test file of class QuadraticCumul for the standard methods
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


      /* We create a NumericalMathFunction */
      NumericalMathFunction myFunc("TestQuadraticCumul");
      /* We create a distribution */
      UnsignedLong dim(myFunc.getInputDimension());
      NumericalPoint meanPoint(dim, 0.1);
      NumericalPoint sigma(dim, 1.0e-1);
      sigma[0] = 2.0e-1;
      sigma[1] = 3.0e-1;
      CorrelationMatrix R(dim);
      for (UnsignedLong i = 0; i < dim; i++)
        {
          R(i, i) = 1.0;
        }
      for (UnsignedLong i = 1; i < dim; i++)
        {
          R(i, i - 1) = 0.25;
        }
      Normal myDistribution(meanPoint, sigma, R);

      /* we name the components of the distribution */
      Description componentDescription(dim);
      componentDescription[0] = "Marginal 1";
      componentDescription[1] = "Marginal 2";
      myDistribution.setDescription(componentDescription);

      /* We create a distribution-based RandomVector */
      RandomVector X(myDistribution);

      /* We create a composite RandomVector Y from X and myFunction */
      RandomVector Y(myFunc, X);
      Y.setName("Interest Variable 1");

      // We create a quadraticCumul algorithm
      QuadraticCumul myQuadraticCumul(Y);
      // We test the attributs here
      fullprint << "Importance factors=" << myQuadraticCumul.getImportanceFactors() << std::endl;

    }

  catch (OT::NotSymmetricDefinitePositiveException & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }

  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }

  return ExitCode::Success;
}
