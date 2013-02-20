//                                               -*- C++ -*-
/**
 *  @file  t_FilteringWindows_std.cxx
 *  @brief The test file of class FilteringWindows
 *         This tests the Hamming and Hanning classes
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
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


      /* Hanning filter */
      Hanning myHanningFilter;
      fullprint << "myHanningFilter = " << myHanningFilter << std::endl;

      /* Hamming filter */
      Hamming myHammingFilter;
      fullprint << "myHammingFilter = " << myHammingFilter << std::endl;

      /* Evaluation of values between t=0 and t=1 using a step = 0.01 */
      const UnsignedLong    steps(100);
      const NumericalScalar tMin(0.0);
      const NumericalScalar tMax(1.0);
      const NumericalScalar tStep((tMax - tMin) / steps);

      for (UnsignedLong i = 0; i < steps + 1; ++i )
        {
          const NumericalScalar t(tMin + i * tStep);
          fullprint << "t = " << t
                    << " Hanning = " << myHanningFilter(t)
                    << " Hamming = " << myHammingFilter(t) << std::endl;
        }

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
