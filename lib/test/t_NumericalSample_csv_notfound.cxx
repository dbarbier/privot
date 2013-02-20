//                                               -*- C++ -*-
/**
 *  @file  t_NumericalSample_csv_notfound.cxx
 *  @brief The test file of class NumericalSample when the CSV file is missing
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
 *  @date   2008-06-26 13:50:17 +0200 (Thu, 26 Jun 2008)
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

      try
        {
          NumericalSample aSample = NumericalSample::ImportFromCSVFile(path + "nosample.csv");
          throw TestFailed( "ERROR: test should have failed. Found CSV file 'nosample.csv' though it should not have been there" );

        }
      catch (FileNotFoundException & ex)
        {
          // Nothing to do
        }

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
