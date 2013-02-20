//                                               -*- C++ -*-
/**
 *  @file  t_ResourceMap_std.cxx
 *  @brief The test file of class ResourceMap for standard methods
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
 *  @date   2009-09-30 17:51:57 +0200 (mer. 30 sept. 2009)
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
      // As the result of these methods is installation dependent, don't check the output
      fullprint << "Installation directory=" << Path::GetInstallationDirectory() << std::endl;
      fullprint << "Module directory=" << Path::GetModuleDirectory() << std::endl;
      Path::DirectoryList wrapperDirectoryList(Path::GetWrapperDirectoryList());
      for (UnsignedLong i = 0; i < wrapperDirectoryList.size(); ++i)
        fullprint << "wrapperDirectoryList[" << i << "]=" << wrapperDirectoryList[i] << std::endl;
      Path::DirectoryList configDirectoryList(Path::GetConfigDirectoryList());
      for (UnsignedLong i = 0; i < configDirectoryList.size(); ++i)
        fullprint << "configDirectoryList[" << i << "]=" << configDirectoryList[i] << std::endl;
      Path::DirectoryList list(2);
      FileName directory1(Path::CreateTemporaryDirectory("testDirectory1"));
      fullprint << "Directory 1=" << directory1 << std::endl;
      list[0] = directory1;
      FileName directory2(Path::CreateTemporaryDirectory("testDirectory2"));
      fullprint << "Directory 2=" << directory2 << std::endl;
      list[1] = directory2;
      // Create a file in dir2
      std::ofstream testFile(FileName(directory2 + FileName("/testFile")).c_str());
      testFile << "test" << std::endl;
      testFile.close();
      FileName findName(Path::FindFileByNameInDirectoryList("testFile", list));
      fullprint << "Find file=" << findName << std::endl;
      FileName fileName(Path::BuildTemporaryFileName("testFile"));
      fullprint << "Temporary file name=" << fileName << std::endl;
      Path::DeleteTemporaryDirectory(directory1);
      Path::DeleteTemporaryDirectory(directory2);
    }
  catch (FileOpenException & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }




  return ExitCode::Success;
}
