//                                               -*- C++ -*-
/**
 *  @file  t_LinearModelFactory_std.cxx
 *  @brief The test file of class LinearModelFactory
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
  setRandomGenerator();

  /* Default constructor */
  LinearModelFactory lmfact;

  /* lm build */
  UnsignedLong size(20);
  NumericalSample oneSample(size, 1);
  NumericalSample twoSample(size, 1);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      oneSample[i][0] = 7.0 * sin(-3.5 + (6.5 * i) / (size - 1.0)) + 2.0;
      twoSample[i][0] = -2.0 * oneSample[i][0] + 3.0 + 0.05 * sin(oneSample[i][0]);
    }
  LinearModel test = lmfact.build(oneSample, twoSample);
  fullprint << "test = " << test << std::endl;

  return ExitCode::Success;
}
