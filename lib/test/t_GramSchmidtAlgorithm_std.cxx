//                                               -*- C++ -*-
/**
 *  @file  t_GramSchmidtAlgorithm_std.cxx
 *  @brief The test file of class GramSchmidtAlgorithm for standard methods
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

NumericalPoint clean(NumericalPoint in)
{
  UnsignedLong dim(in.getDimension());
  for(UnsignedLong i = 0; i < dim; i++)
    if (fabs(in[i]) < 1.e-10) in[i] = 0.0;
  return in;
}

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {
      const UnsignedLong iMax(5);
      Triangular distribution(-1.0, 0.3, 1.0);
      GramSchmidtAlgorithm algo(distribution);
      fullprint << "algo=" << algo << std::endl;
      for (UnsignedLong i = 0; i < iMax; ++i)
        fullprint << distribution.getClassName() << " polynomial(" << i << ")=" << clean(algo.getRecurrenceCoefficients(i)).__str__() << std::endl;
      algo.setReferenceFamily(LegendreFactory());
      fullprint << "Reference family=" << algo.getReferenceFamily() << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }

  return ExitCode::Success;
}
