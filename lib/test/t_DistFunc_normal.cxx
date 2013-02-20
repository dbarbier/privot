//                                               -*- C++ -*-
/**
 *  @file  t_DistFunc_normal.cxx
 *  @brief The test file of class DistFunc for standard methods
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
 *  @date   2009-05-28 14:47:53 +0200 (jeu. 28 mai 2009)
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
      // Normal related functions
      {
        // pNormal
        NumericalScalar xMin(0.1);
        NumericalScalar xMax(0.9);
        UnsignedLong nX(10);
        for (UnsignedLong iX = 0; iX < nX; ++iX)
          {
            NumericalScalar x(xMin + (xMax - xMin) * iX / (nX - 1));
            fullprint << "pNormal(" << x << ")=" << DistFunc::pNormal(x) << ", complementary=" << DistFunc::pNormal(x, true) << std::endl;
          }
      } // pNormal
      {
        // qNormal
        NumericalScalar qMin(0.1);
        NumericalScalar qMax(0.9);
        UnsignedLong nQ(10);
        for (UnsignedLong iQ = 0; iQ < nQ; ++iQ)
          {
            NumericalScalar q(qMin + (qMax - qMin) * iQ / (nQ - 1));
            fullprint << "qNormal(" << q << ")=" << DistFunc::qNormal(q) << ", complementary=" << DistFunc::qNormal(q, true) << std::endl;
          }
      } // qNormal
      {
        // rNormal
        UnsignedLong nR(10);
        for (UnsignedLong iR = 0; iR < nR; ++iR)
          {
            fullprint << "rNormal()=" << DistFunc::rNormal() << std::endl;
          }
      } // rNormal
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
