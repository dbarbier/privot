//                                               -*- C++ -*-
/**
 *  @file  t_HistoryStrategy_std.cxx
 *  @brief The test file of class HistoryStrategy for standard methods
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
      UnsignedLong size(10);
      UnsignedLong dimension(2);
      NumericalSample sample(size, dimension);
      // Fill-in the sample
      for (UnsignedLong i = 0; i < size; ++i)
        {
          for (UnsignedLong j = 0; j < dimension; ++j)
            {
              sample[i][j] = i + NumericalScalar(j) / dimension;
            }
        }
      fullprint << "sample=" << sample << std::endl;
      // History using the Null strategy
      Null nullStrategy;
      for (UnsignedLong i = 0; i < size; ++i)
        {
          nullStrategy.store(sample[i]);
        }
      fullprint << "Null strategy sample=" << nullStrategy.getSample() << std::endl;
      // History using the Full strategy
      Full fullStrategy;
      for (UnsignedLong i = 0; i < size; ++i)
        {
          fullStrategy.store(sample[i]);
        }
      fullprint << "Full strategy sample=" << fullStrategy.getSample() << std::endl;
      // History using the Last strategy, large storage
      Last lastStrategy(3 * size);
      for (UnsignedLong i = 0; i < size; ++i)
        {
          lastStrategy.store(sample[i]);
        }
      fullprint << "Last strategy sample (large storage)=" << lastStrategy.getSample() << std::endl;
      lastStrategy = Last(size / 3);
      // History using the Last strategy, small storage
      for (UnsignedLong i = 0; i < size; ++i)
        {
          lastStrategy.store(sample[i]);
        }
      fullprint << "Last strategy sample (small storage)=" << lastStrategy.getSample() << std::endl;
      // History using the Compact strategy, large storage
      Compact compactStrategy(3 * size);
      for (UnsignedLong i = 0; i < size; ++i)
        {
          compactStrategy.store(sample[i]);
        }
      fullprint << "Compact strategy sample (large storage)=" << compactStrategy.getSample() << std::endl;
      compactStrategy = Compact(size / 3);
      // History using the Compact strategy, small storage
      for (UnsignedLong i = 0; i < size; ++i)
        {
          compactStrategy.store(sample[i]);
        }
      fullprint << "Compact strategy sample (small storage)=" << compactStrategy.getSample() << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
