//                                               -*- C++ -*-
/**
 *  @file  t_PersistentObject_std.cxx
 *  @brief The test file of class PersistentObject for standard methods
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

class TestObject : public PersistentObject
{
public:
  explicit TestObject() : PersistentObject() {}
  explicit TestObject(const String & name) : PersistentObject(name) {}
  virtual ~TestObject() {}
  virtual TestObject * clone() const
  {
    return new TestObject(*this);
  }
};


int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {
      // Test basic functionnalities
      checkClassWithClassName<TestObject>();

      // Test some extra functionnalities
      checkNameFeature<TestObject>();

      // Identity comparison
      TestObject o1, o2;
      if (! o1.is(o1))
        {
          OSS errorMessage;
          errorMessage << "OT::PersistentObject.is does NOT return the correct value. Says that object o1 IS NOT o1 !";
          throw TestFailed(errorMessage);
        }

      if (o1.is(o2))
        {
          OSS errorMessage;
          errorMessage << "OT::PersistentObject.is does NOT return the correct value. Says that object o1 IS object o2 !";
          throw TestFailed(errorMessage);
        }

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
