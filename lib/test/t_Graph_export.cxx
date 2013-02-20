//                                               -*- C++ -*-
/**
 *  @file  t_Graph_export.cxx
 *  @brief The test file of Graph class for EDF case number 1
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

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {
      UnsignedLong maxPoint(101);
      NumericalScalar xMin(-2.0 * M_PI);
      NumericalScalar xMax(2.0 * M_PI);
      NumericalSample data(maxPoint, 2);
      for (UnsignedLong i = 0; i < maxPoint; ++i)
        {
          NumericalScalar x(xMin + (xMax - xMin) * i / (maxPoint - 1.0));
          data[i][0] = x;
          data[i][1] = sin(x);
        }
      Graph sinGraph("Sin function", "x", "y", true, "topright");
      sinGraph.add(Curve(data, "red", "solid", 2, "sin"));
      // Test export to all formats
      sinGraph.draw("sinGraph_ALL", 640, 480, GraphImplementation::ALL);
      fullprint << "sinGraph (ALL)=" << sinGraph.getBitmap() << ", " << sinGraph.getPostscript() << ", " << sinGraph.getVectorial() << std::endl;
      // Test export to PNG format
      sinGraph.draw("sinGraph_PNG", 640, 480, GraphImplementation::PNG);
      fullprint << "sinGraph (PNG)=" << sinGraph.getBitmap() << std::endl;
      // Test export to EPS format
      sinGraph.draw("sinGraph_EPS", 640, 480, GraphImplementation::EPS);
      fullprint << "sinGraph (EPS)=" << sinGraph.getPostscript() << std::endl;
      // Test export to FIG format
      sinGraph.draw("sinGraph_FIG", 640, 480, GraphImplementation::FIG);
      fullprint << "sinGraph (FIG)=" << sinGraph.getVectorial() << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
