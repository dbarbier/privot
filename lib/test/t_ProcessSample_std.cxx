//                                               -*- C++ -*-
/**
 *  @file  t_ProcessSample_std.cxx
 *  @brief The test file of class ProcessSample for standard methods
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

typedef Collection<TimeSeries> TimeSeriesCollection;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {

      // We create a sample and we fill it
      const UnsignedLong size = 6;
      const UnsignedLong dimension = 1;
      NumericalSample sample(size, dimension);
      for (UnsignedLong i = 0; i < size; ++i)
        for (UnsignedLong j = 0; j < dimension; ++j)
          sample[i][j] = i + j + 1;

      // TimeGrid
      RegularGrid timeGrid(0.0, 1.0 / (size - 1.0), size);

      // TimeSeries
      TimeSeries timeSerie(timeGrid, sample);

      // We create an empty ProcessSample with default constructor
      ProcessSample psample0;
      psample0.setName("PSample0");
      fullprint << "Default constructor" << std::endl;
      fullprint << "psample0=" << psample0 << std::endl;

      // We create an empty ProcessSample with timeGrid, size and dimension arguments
      ProcessSample psample1(timeGrid, 4, dimension);
      fullprint << "Constructor based on size, dimension and timeGrid" << std::endl;
      fullprint << "psample1=" << psample1 << std::endl;

      // change the first component using operator []
      psample1[0] = timeSerie;
      psample1.setName("PSample1");
      fullprint << "changing psample1[0] with []" << std::endl;
      fullprint << "psample1[0]=" << psample1[0] << std::endl;

      // We create a ProcessSample with size and timeSerie arguments
      ProcessSample psample2(3, timeSerie);
      psample2.setName("PSample2");
      fullprint << "Constructor based on size / timeSerie" << std::endl;
      fullprint << "psample2=" << psample2 << std::endl;

      // TimeSeriesCollection
      TimeSeriesCollection collection;
      collection.add(timeSerie);

      sample.scale( NumericalPoint( 1, 0.1 ) );
      TimeSeries newtimeSeries(timeGrid, sample);
      collection.add( newtimeSeries );

      // We create a ProcessSample with collection arguments arguments
      ProcessSample psample3( collection );

      psample3.setName("PSample3");
      fullprint << "Constructor based on collection " << std::endl;
      fullprint << "psample3=" << psample3 << std::endl;

      TimeSeries ts = psample3[1];
      fullprint << "last element of psample3=" << ts << std::endl;

      // print mean twice for non-regression test #434
      fullprint << "psample 3 mean=" << psample3.computeMean() << std::endl;
      fullprint << "psample 3 mean=" << psample3.computeMean() << std::endl;
      fullprint << "psample 3 temporal mean=" << psample3.computeTemporalMean() << std::endl;




    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
