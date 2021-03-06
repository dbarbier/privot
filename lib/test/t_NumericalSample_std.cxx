//                                               -*- C++ -*-
/**
 *  @file  t_NumericalSample_std.cxx
 *  @brief The test file of class NumericalSample for standard methods
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

class TestObject : public NumericalSample
{
public:
  explicit TestObject() : NumericalSample(1, 1) {}
  explicit TestObject(const String & name) : NumericalSample(1, 1)
  {
    setName(name);
  }
  virtual ~TestObject() {}
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

      // We create an empty NumericalSample
      NumericalSample sample1(0, 2);
      sample1.setName("Sample1");

      // We populate the empty sample
      NumericalPoint point1(2);
      point1[0] = 10.;
      point1[1] = 20.;
      NumericalPoint point2(2);
      point2[0] = 11.;
      point2[1] = 21.;
      NumericalPoint point3(2);
      point3[0] = 12.;
      point3[1] = 22.;
      sample1.add(point1);
      sample1.add(point2);
      sample1.add(point3);
      fullprint << "sample1=" << sample1 << std::endl;

      // We get the second element of the sample
      NumericalPoint secondElement = sample1[1];
      fullprint << "second element=" << secondElement << std::endl;

      // We set the third element to a valid new element
      NumericalPoint newPoint(2);
      newPoint[0] = 1000;
      newPoint[1] = 2000;
      sample1[2] = newPoint;
      fullprint << "sample1=" << sample1 << std::endl;

      NumericalPoint translation(2, 5.0);
      sample1.translate(translation);
      fullprint << "after a translation of vector=" << translation << " sample1=" << sample1 << std::endl;

      NumericalPoint scaling(2, 2.0);
      sample1.scale(scaling);
      fullprint << "after a scaling of vector=" << scaling << " sample1=" << sample1 << std::endl;

      NumericalSample sample2(10, point1);
      fullprint << "sample2=" << sample2 << std::endl;

      sample2[5] = point2;
      fullprint << "sample2=" << sample2 << std::endl;


      try
        {
          // We get the tenth element of the sample
          // THIS SHOULD NORMALY FAIL
          NumericalPoint tenthElement = sample1.at(9);

          // Normally, we should never go here
          throw TestFailed("Exception has NOT been thrown or catched !");

        }
      catch (OutOfBoundException & ex)
        {
          // Nothing to do
        }




      // We try to create a sample with 5 times the same point
      NumericalPoint samePoint(3);
      samePoint[0] = 1000.;
      samePoint[1] = 2000.;
      samePoint[2] = 3000.;
      NumericalSample sample3(5, samePoint);
      fullprint << "sample3=" << sample3 << std::endl;

      // We append a new point to the sample...
      NumericalPoint anotherNewPoint(3);
      anotherNewPoint[0] = -1000.;
      anotherNewPoint[1] = -2000.;
      anotherNewPoint[2] = -3000.;
      sample3.add(anotherNewPoint);
      fullprint << "sample3=" << sample3 << std::endl;

      // ... then the first point again
      sample3.add(samePoint);
      fullprint << "sample3=" << sample3 << std::endl;

      // We find the new point into the sample
      UnsignedLong pos = sample3.find(anotherNewPoint);
      //fullprint << "pos=" << pos << std::endl;
      if (pos != 5)
        {
          OSS oss;
          oss << "Error in finding point into sample. Expected 5. Got " << pos;
          throw TestFailed(oss);
        }

      //for(UnsignedLong i=0; i<sample3.getSize(); ++i)
      //  fullprint << "sample3[" << i << "]=" << sample3[i] << std::endl;

      // We erase the new point from the sample
      sample3.erase(pos);
      fullprint << "sample3=" << sample3 << std::endl;


    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
