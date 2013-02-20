//                                               -*- C++ -*-
/**
 *  @file  t_NumericalSample_csv.cxx
 *  @brief The test file of class NumericalSample for load and save methods
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
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
      char * envPath = getenv("OPENTURNS_NUMERICALSAMPLE_PATH");
      String path;
      if (envPath != NULL)
        {
          path = envPath;
          path += "/";
        }
      NumericalSample aSample = NumericalSample::ImportFromCSVFile(path + "sample_good.csv");
      aSample.setName("a good sample");
      fullprint << "aSample=" << aSample << std::endl;

      aSample = NumericalSample::ImportFromCSVFile(path + "sample_good_coma.csv", ",");
      aSample.setName("a good coma separated sample");
      fullprint << "aSample=" << aSample << std::endl;

      aSample = NumericalSample::ImportFromCSVFile(path + "sample_bad.csv");
      aSample.setName("a sample with bad entries");
      fullprint << "aSample with bad entries (see log)=" << aSample << std::endl;

      aSample = NumericalSample::ImportFromCSVFile(path + "sample_missing.csv");
      aSample.setName("a sample with missing entries");
      fullprint << "aSample with missing entries (see log)=" << aSample << std::endl;
      // We export the sample to an CSV file and then read it again
      aSample.exportToCSVFile("sample.csv");
      NumericalSample sameSample = NumericalSample::ImportFromCSVFile("sample.csv");
      if (aSample != sameSample) throw TestFailed("Exported sample differs from imported sample");

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
