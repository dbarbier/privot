//                                               -*- C++ -*-
/**
 * @file  t_AggregatedNumericalMathEvaluationImplementation_std.cxx
 * @brief The test file of class AggregatedNumericalMathEvaluationImplementation for standard methods
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
      UnsignedLong size(4);
      NumericalPoint locations(size);
      NumericalPoint values(size);
      // Build locations/values with non-increasing locations
      for (UnsignedLong i = 0; i < size; ++i)
	{
	  locations[i] = ((i - 0.3 * size) * (i + 0.7 * size)) / size;
	  values[i] = locations[i] * locations[i];
	}
      PiecewiseEvaluationImplementation evaluation(locations, values);
      fullprint << "evaluation=" << evaluation << std::endl;
      // Check the values
      for (UnsignedLong i = 0; i < 2 * size; ++i)
	{
	  NumericalPoint x(1, (-1.0 + (2.0 * i) / size) * size / 2.0);
	  fullprint << "f(" << x[0] << ")=" << evaluation(x) << std::endl;
	}
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }

  return ExitCode::Success;
}
