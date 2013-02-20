//                                               -*- C++ -*-
/**
 *  @file  t_DistFunc_gamma.cxx
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
      // Gamma related functions
      {
        // pGamma
        NumericalScalar kMin(0.2);
        NumericalScalar kMax(5.0);
        UnsignedLong nK(5);
        NumericalScalar xMin(0.1);
        NumericalScalar xMax(0.9);
        UnsignedLong nX(5);
        for (UnsignedLong i1 = 0; i1 < nK; ++i1)
          {
            NumericalScalar k(kMin + (kMax - kMin) * i1 / (nK - 1));
            for (UnsignedLong iX = 0; iX < nX; ++iX)
              {
                NumericalScalar x(xMin + (xMax - xMin) * iX / (nX - 1));
                fullprint << "pGamma(" << k << ", " << x << ")=" << DistFunc::pGamma(k, x) << ", complementary=" << DistFunc::pGamma(k, x, true) << std::endl;
              }
          }
      } // pGamma
      {
        // qGamma
        NumericalScalar kMin(0.2);
        NumericalScalar kMax(5.0);
        UnsignedLong nK(5);
        NumericalScalar qMin(0.1);
        NumericalScalar qMax(0.9);
        UnsignedLong nQ(5);
        for (UnsignedLong i1 = 0; i1 < nK; ++i1)
          {
            NumericalScalar k(kMin + (kMax - kMin) * i1 / (nK - 1));
            for (UnsignedLong iQ = 0; iQ < nQ; ++iQ)
              {
                NumericalScalar q(qMin + (qMax - qMin) * iQ / (nQ - 1));
                fullprint << "qGamma(" << k << ", " << q << ")=" << DistFunc::qGamma(k, q) << ", complementary=" << DistFunc::qGamma(k, q, true) << std::endl;
              }
          }
      } // qGamma
      {
        // rGamma
        NumericalScalar kMin(0.2);
        NumericalScalar kMax(5.0);
        UnsignedLong nK(5);
        UnsignedLong nR(5);
        for (UnsignedLong i1 = 0; i1 < nK; ++i1)
          {
            NumericalScalar k(kMin + (kMax - kMin) * i1 / (nK - 1));
            for (UnsignedLong iR = 0; iR < nR; ++iR)
              {
                fullprint << "rGamma(" << k << ")=" << DistFunc::rGamma(k) << std::endl;
              }
          }
      } // rGamma
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
