//                                               -*- C++ -*-
/**
 *  @file  t_BoxCoxFactory_std.cxx
 *  @brief The test file of class BoxCoxFactory for standard methods
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

  try
    {
      // TimeGrid parameters
      const UnsignedLong n(101);
      const NumericalScalar timeStart(0.0);
      const NumericalScalar timeStep(0.1);
      RegularGrid timeGrid(timeStart, timeStep, n);

      // White noise
      const WhiteNoise whiteNoise(Uniform(), timeGrid);

      // Composite process
      const CompositeProcess process(SpatialFunction(NumericalMathFunction("x", "x+2")), whiteNoise);
      // A realization of the process
      const TimeSeries timeSeries(process.getRealization());
      const NumericalSample sample(timeSeries.getSample());

      // Now we build the factory
      BoxCoxFactory factory;

      // Creation of the BoxCoxTransform
      BoxCoxTransform myBoxCox(factory.build(timeSeries));

      fullprint << "myBoxCox (time-series)=" << myBoxCox << std::endl;
      fullprint << "myBoxCox (sample)     =" << factory.build(sample) << std::endl;

      // Creation of the BoxCoxTransform using shift
      NumericalPoint shift(1, 1.0);
      BoxCoxTransform myBoxCoxShift(factory.build(timeSeries, shift));
      
      fullprint << "myBoxCox with shift (time-series)=" << myBoxCoxShift << std::endl;
      fullprint << "myBoxCox with shift (sample)     =" << factory.build(sample, shift) << std::endl;

      // Creation of the BoxCoxTransform using shift with graph
      Graph graph;
      BoxCoxTransform myBoxCoxShiftGraph(factory.build(timeSeries, shift, graph));
      
      fullprint << "BoxCox graph (time-series)=" << graph << std::endl;
      graph.draw("BoxCoxGraph");

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
