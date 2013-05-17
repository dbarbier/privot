//                                               -*- C++ -*-
/**
 *  @file  t_ARMAState_std.cxx
 *  @brief The test file of class ARMAState
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

      RandomGenerator::SetSeed(0);
      /* Constructor with size */
      const UnsignedLong dim(1);
      const UnsignedLong p(6);
      const UnsignedLong q(4);

      NumericalSample valuesX(p, dim);
      NumericalSample valuesEpsilon(q, dim);

      /**/
      for(UnsignedLong j = 0 ; j < dim ; ++j)
        {
          // Fill the AR-part (the last p-coefficients X_{-1}, X{-2},..., X_{-p})
          for(UnsignedLong i = 0 ; i < p ; ++i)
            {
              valuesX[i][j] = 2.0 * i + 3.0 * j + 1.0;
            }

          // Fill the MA-part (the last p-coefficients \epsilon_{-1}, \epsilon_{-2},..., \epsilon_{-p})
          for(UnsignedLong i = 0 ; i < q ; ++i)
            {
              valuesEpsilon[i][j] = RandomGenerator::Generate() ;
            }

        }

      /** Print the initial state of the ARMA : coefficients*/
      fullprint << "X values = " << valuesX << std::endl;
      fullprint << "Epsilon values = " << valuesEpsilon << std::endl;

      // default constructor
      ARMAState myDefaultState;
      fullprint << "ARMAState with default constructor = " << myDefaultState.__repr__() << std::endl;

      // parameters constructor
      ARMAState myState(valuesX, valuesEpsilon);
      fullprint << "ARMAState  = " << myState.__repr__() << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}