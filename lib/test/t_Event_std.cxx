//                                               -*- C++ -*-
/**
 *  @file  t_Event_std.cxx
 *  @brief The test file of Event class
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
  setRandomGenerator();

  try
    {

      /* We create a numerical point of dimension 1 */
      NumericalPoint point(4);
      const NumericalPoint & ref_point(point);

      point[0] = 101;
      point[1] = 202;
      point[2] = 303;
      point[3] = 404;

      fullprint << "point = " << ref_point << std::endl;

      /* We create a 'constant' RandomVector from the NumericalPoint */
      RandomVector vect(point);
      fullprint << "vect=" << vect << std::endl;

      /* Check standard methods of class RandomVector */
      fullprint << "vect dimension=" << vect.getDimension() << std::endl;
      fullprint << "vect realization (first )=" << vect.getRealization() << std::endl;
      fullprint << "vect sample =" << vect.getSample(5) << std::endl;

      /* We create a numerical math function */
      NumericalMathFunction myFunction("poutre");

      /* We create a composite random vector */
      RandomVector output(myFunction, vect);

      /* We create an Event from this RandomVector */
      Event myEvent(output, Less(), 50);
      fullprint << "myEvent=" << myEvent << std::endl;

      /* We compute one realization of the event */
      fullprint << "myEvent realization=" << myEvent.getRealization() << std::endl;
      fullprint << "myEvent antecedent realization=" << myEvent.getImplementation()->getAntecedent()->getRealization() << std::endl;

      /* We compute a sample of the event */
      fullprint << "myEvent sample=" << myEvent.getSample(10) << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
