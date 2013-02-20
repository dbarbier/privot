//                                               -*- C++ -*-
/**
 *  @file  t_FixedExperiment_std.cxx
 *  @brief The test file of class FixedExperiment for standard methods
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
 *  @date   2008-05-21 17:44:02 +0200 (mer, 21 mai 2008)
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
      UnsignedLong size(5);
      UnsignedLong dim(6);
      NumericalSample refSample(size, dim);
      for (UnsignedLong i = 0; i < size; ++i)
        for (UnsignedLong j = 0; j < dim; ++j)
          refSample[i][j] = i + j;
      fullprint << "ref. sample=" << refSample << std::endl;
      FixedExperiment myPlane(refSample);
      fullprint << "myPlane = " << myPlane << std::endl;
      NumericalPoint weights(0);
      NumericalSample sample(myPlane.generate(weights));
      fullprint << "sample = " << sample << std::endl;
      fullprint << "weights = " << weights << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }

  return ExitCode::Success;
}
