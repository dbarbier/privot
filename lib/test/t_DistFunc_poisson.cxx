//                                               -*- C++ -*-
/**
 *  @file  t_DistFunc_poisson.cxx
 *  @brief The test file of class DistFunc for standard methods
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
 *  @author dutka
 *  @date   2009-05-28 14:47:53 +0200 (jeu. 28 mai 2009)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);
  setRandomGenerator();

  try
    {
      // Poisson related functions
      {
        // rPoisson
        NumericalScalar lambdaMin(0.2);
        NumericalScalar lambdaMax(5.0);
        UnsignedLong n1(5);
        UnsignedLong nR(5);
        for (UnsignedLong i1 = 0; i1 < n1; ++i1)
          {
            NumericalScalar lambda(lambdaMin + (lambdaMax - lambdaMin) * i1 / (n1 - 1));
            for (UnsignedLong iR = 0; iR < nR; ++iR)
              {
                fullprint << "rPoisson(" << lambda << ")=" << DistFunc::rPoisson(lambda) << std::endl;
              }
          }
      } // rPoisson
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
