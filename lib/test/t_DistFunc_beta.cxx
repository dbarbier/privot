//                                               -*- C++ -*-
/**
 *  @file  t_DistFunc_beta.cxx
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
      // Beta related functions
      {
        // pBeta
        NumericalScalar p1Min(0.2);
        NumericalScalar p1Max(5.0);
        UnsignedLong n1(5);
        NumericalScalar p2Min(0.2);
        NumericalScalar p2Max(5.0);
        UnsignedLong n2(5);
        NumericalScalar xMin(0.1);
        NumericalScalar xMax(0.9);
        UnsignedLong nX(5);
        for (UnsignedLong i1 = 0; i1 < n1; ++i1)
          {
            NumericalScalar p1(p1Min + (p1Max - p1Min) * i1 / (n1 - 1));
            for (UnsignedLong i2 = 0; i2 < n2; ++i2)
              {
                NumericalScalar p2(p2Min + (p2Max - p2Min) * i2 / (n2 - 1));
                for (UnsignedLong iX = 0; iX < nX; ++iX)
                  {
                    NumericalScalar x(xMin + (xMax - xMin) * iX / (nX - 1));
                    fullprint << "pBeta(" << p1 << ", " << p2 << ", " << x << ")=" << DistFunc::pBeta(p1, p2, x) << ", complementary=" << DistFunc::pBeta(p1, p2, x, true) << std::endl;
                  }
              }
          }
      } // pBeta
      {
        // qBeta
        NumericalScalar p1Min(0.2);
        NumericalScalar p1Max(5.0);
        UnsignedLong n1(5);
        NumericalScalar p2Min(0.2);
        NumericalScalar p2Max(5.0);
        UnsignedLong n2(5);
        NumericalScalar qMin(0.1);
        NumericalScalar qMax(0.9);
        UnsignedLong nQ(5);
        for (UnsignedLong i1 = 0; i1 < n1; ++i1)
          {
            NumericalScalar p1(p1Min + (p1Max - p1Min) * i1 / (n1 - 1));
            for (UnsignedLong i2 = 0; i2 < n2; ++i2)
              {
                NumericalScalar p2(p2Min + (p2Max - p2Min) * i2 / (n2 - 1));
                for (UnsignedLong iQ = 0; iQ < nQ; ++iQ)
                  {
                    NumericalScalar q(qMin + (qMax - qMin) * iQ / (nQ - 1));
                    fullprint << "qBeta(" << p1 << ", " << p2 << ", " << q << ")=" << DistFunc::qBeta(p1, p2, q) << ", complementary=" << DistFunc::qBeta(p1, p2, q, true) << std::endl;
                  }
              }
          }
      } // qBeta
      {
        // rBeta
        NumericalScalar p1Min(0.2);
        NumericalScalar p1Max(5.0);
        UnsignedLong n1(5);
        NumericalScalar p2Min(0.2);
        NumericalScalar p2Max(5.0);
        UnsignedLong n2(5);
        UnsignedLong nR(5);
        for (UnsignedLong i1 = 0; i1 < n1; ++i1)
          {
            NumericalScalar p1(p1Min + (p1Max - p1Min) * i1 / (n1 - 1));
            for (UnsignedLong i2 = 0; i2 < n2; ++i2)
              {
                NumericalScalar p2(p2Min + (p2Max - p2Min) * i2 / (n2 - 1));
                for (UnsignedLong iR = 0; iR < nR; ++iR)
                  {
                    fullprint << "rBeta(" << p1 << ", " << p2 << ")=" << DistFunc::rBeta(p1, p2) << std::endl;
                  }
              }
          }
      } // rBeta
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
