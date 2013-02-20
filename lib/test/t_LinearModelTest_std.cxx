//                                               -*- C++ -*-
/**
 *  @file  t_LinearModelTest_std.cxx
 *  @brief The test file of class Test
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
  setRandomGenerator();

  UnsignedLong size(100);
  UnsignedLong dim(10);
  CorrelationMatrix R(dim);
  for (UnsignedLong i = 0; i < dim; i++)
    {
      for (UnsignedLong j = 0; j < i; j++)
        {
          R(i, j) = (i + j + 1.0) / (2.0 * dim);
        }
    }
  NumericalPoint mean(dim, 2.0);
  NumericalPoint sigma(dim, 3.0);
  Normal distribution(mean, sigma, R);
  NumericalSample sample(distribution.getSample(size));
  NumericalSample sampleX(size, dim - 1);
  NumericalSample sampleY(size, 1);
  for (UnsignedLong i = 0; i < size; i++)
    {
      sampleY[i][0] = sample[i][0];
      for (UnsignedLong j = 1; j < dim; j++)
        {
          sampleX[i][j - 1] = sample[i][j];
        }
    }
  NumericalSample sampleZ(size, 1);
  for (UnsignedLong i = 0; i < size; i++)
    {
      sampleZ[i][0] = sampleY[i][0] * sampleY[i][0];
    }
  fullprint << "LinearModelAdjustedRSquared=" << LinearModelTest::LinearModelAdjustedRSquared(sampleY, sampleZ) << std::endl;
  fullprint << "LinearModelFisher=" << LinearModelTest::LinearModelFisher(sampleY, sampleZ) << std::endl;
  fullprint << "LinearModelResidualMean=" << LinearModelTest::LinearModelResidualMean(sampleY, sampleZ) << std::endl;
  fullprint << "LinearModelRSquared=" << LinearModelTest::LinearModelRSquared(sampleY, sampleZ) << std::endl;
  return ExitCode::Success;
}
