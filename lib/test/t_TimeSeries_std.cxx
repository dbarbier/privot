//                                               -*- C++ -*-
/**
 *  @file  t_TimeSeries_std.cxx
 *  @brief The test file of class TimeSeries for standard methods
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

class TestObject : public TimeSeries
{
public:
  explicit TestObject() : TimeSeries(1, 1) {}
  explicit TestObject(const String & name) : TimeSeries(1, 1)
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

      const UnsignedLong DIM = 2;

      // We create an empty TimeSeries
      TimeSeries ts1(0, DIM);
      ts1.setName("Ts1");

      // We populate the empty ts
      for (UnsignedLong p = 0; p < 3; ++p)
        {
          NumericalPoint pt(DIM);
          for (UnsignedLong i = 0; i < DIM; ++i) pt[i] = 10. * (p + 1) + i;
          ts1.add( pt );
        }
      fullprint << "ts1=" << ts1 << std::endl;

      // We get the second element of the ts
      NumericalPoint secondElement = ts1[1];
      fullprint << "second element=" << secondElement << std::endl;

      // We set the third element to a valid new element
      NumericalPoint newPoint(DIM + 1);
      for (UnsignedLong i = 0; i < DIM; ++i) newPoint[i + 1] = 1000. * (i + 1);
      ts1[2] = newPoint;
      fullprint << "ts1=" << ts1 << std::endl;

      // We get the values of the second element of the ts
      NumericalPoint values1 = ts1.getValueAtIndex(1);
      fullprint << "values of the second element=" << values1 << std::endl;

      // We set new values for the second element of the ts
      NumericalPoint newValues = values1 * 2.;
      ts1.setValueAtIndex(1, newValues);
      NumericalPoint values2 = ts1.getValueAtIndex(1);
      fullprint << "ts1=" << ts1 << std::endl;
      fullprint << "modified values of the second element=" << values2 << " should be " << newValues << std::endl;

      // We get the values of the second element of the ts
      NumericalPoint values3 = ts1.getValueAtNearestTime( -1.0 );
      fullprint << "values at t=-1.0 =" << values3 << std::endl;
      NumericalPoint values4 = ts1.getValueAtNearestTime( 1.45 );
      fullprint << "values at t=1.45 =" << values4 << std::endl;
      NumericalPoint values5 = ts1.getValueAtNearestTime( 1.54 );
      fullprint << "values at t=1.54 =" << values5 << std::endl;
      NumericalPoint values6 = ts1.getValueAtNearestTime( 14.5 );
      fullprint << "values at t=14.5 =" << values6 << std::endl;

      // We set new values for the third element of the ts
      ts1.setValueAtNearestTime( 1.54, values6 * -1.0 );
      fullprint << "ts1=" << ts1 << std::endl;

      TimeSeries ts2(10, DIM);
      fullprint << "ts2=" << ts2 << std::endl;


      try
        {
          // We get the tenth element of the ts
          // THIS SHOULD NORMALY FAIL
          NumericalPoint tenthElement = ts1.at(9);

          // Normally, we should never go here
          throw TestFailed("Exception has NOT been thrown or catched !");

        }
      catch (OutOfBoundException & ex)
        {
          // Nothing to do
        }

      RegularGrid tg1( 0.0, 0.1, 11 );
      TimeSeries ts3( tg1, DIM );
      fullprint << "ts3=" << ts3 << std::endl;

      RegularGrid tg2( 0.0, 0.2, 6 );
      TimeSeries ts4( tg2, DIM );
      fullprint << "ts4=" << ts4 << std::endl;


      // We append a sample to a time series
      TimeSeries ts5(3, DIM);
      NumericalSample ns1(3, NumericalPoint(DIM, 99.9));
      fullprint << "ts5=" << ts5 << std::endl;
      ts5.add( ns1 );
      fullprint << "ts5=" << ts5 << std::endl;

      // We retrieve the values of the time series as a sample
      NumericalSample ns2 = ts5.getSample();
      fullprint << "ns2=" << ns2 << std::endl;

      // We build a time series from a time grid and a sample
      NumericalSample ns3 = ts4.getSample();
      TimeSeries ts6( tg2, ns3 );
      fullprint << "ts6=" << ts6 << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
