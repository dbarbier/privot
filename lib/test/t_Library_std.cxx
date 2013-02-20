//                                               -*- C++ -*-
/**
 *  @file  t_Library_std.cxx
 *  @brief The test file of class Library for standard methods
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
      const String path(std::getenv("OPENTURNS_WRAPPER_PATH"));

      /** Instance creation */
      LibraryLoader & loader = LibraryLoader::GetInstance();

      try
        {
#ifndef WIN32
          String libraryPath1 = path + "/testwrapper_1.so";
#else
          String libraryPath1 = path + "\\testwrapper_1-0.dll";
#endif
          Library library = loader.load( libraryPath1.c_str() );
          fullprint << loader  << std::endl;
          fullprint << library << std::endl;

          OT::LibrarySymbol symbol = library.getSymbol("fonction1");
          fullprint << "Symbol found at address " << symbol << std::endl;


        }
      catch (DynamicLibraryException & ex)
        {
          throw TestFailed(ex.__repr__());
        }


    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return OT::Test::ExitCode::Success;
}
