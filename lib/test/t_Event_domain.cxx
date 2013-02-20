//                                               -*- C++ -*-
/**
 *  @file  t_Event_domain.cxx
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
 *  @date   2012-02-17 19:35:43 +0100 (ven. 17 févr. 2012)
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

      UnsignedLong dim(2);
      Normal distribution(dim);

      //
      // Case 1: composite random vector based event
      //
      {
        // The input vector
        RandomVector X(distribution);
        // The model: the identity function
        Description inVars(dim);
        for (UnsignedLong i = 0; i < dim; ++i) inVars[i] = OSS() << "x" << i;
        NumericalMathFunction model(inVars, inVars);
        // The output vector
        RandomVector Y(model, X);
        // The domain: [0, 1]^dim
        Interval domain(dim);
        // The event
        Event event(Y, domain);

        fullprint << "sample=" << event.getSample(10) << std::endl;

      }
      //
      // Case 2: process based event
      //
      {
        // The input process
        WhiteNoise X(distribution);
        // The domain: [0, 1]^dim
        Interval domain(dim);
        // The event
        Event event(X, domain);

        fullprint << "sample=" << event.getSample(10) << std::endl;

      }


    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
