//                                               -*- C++ -*-
/**
 *  @file  t_Pie_std.cxx
 *  @brief The test file of class Pie for standard methods
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

      NumericalSample data(0, 1);
      Description palette;
      Description labels;

      data.add(NumericalPoint(1, 5665855));
      labels.add("chirac");
      palette.add("blue");

      data.add(NumericalPoint(1, 4804713));
      labels.add("lepen");
      palette.add("white");

      data.add(NumericalPoint(1, 4610113));
      labels.add("jospin");
      palette.add("pink");

      data.add(NumericalPoint(1, 1949170));
      labels.add("bayrou");
      palette.add("yellow");

      data.add(NumericalPoint(1, 1630045));
      labels.add("laguiller");
      palette.add("red");

      data.add(NumericalPoint(1, 1518528));
      labels.add("chevenement");
      palette.add("ivory");

      data.add(NumericalPoint(1, 1495724));
      labels.add("mamere");
      palette.add("green");

      data.add(NumericalPoint(1, 1210562));
      labels.add("besancenot");
      palette.add("orange");

      data.add(NumericalPoint(1, 1204689));
      labels.add("saint-josse");
      palette.add("maroon");

      data.add(NumericalPoint(1, 1113484));
      labels.add("madelin");
      palette.add("violet");

      data.add(NumericalPoint(1, 3295588));
      labels.add("cumul candidats < 3.5% ");
      palette.add("purple");


      // Create an empty graph
      Graph myGraph("Presidentielle 2002 - 1er tour", "", "", false, "");

      // Create the first cloud
#ifdef WIN32
      OT:: // mingw defined a Pie(...) function
#endif
        Pie myPie1(data, labels, NumericalPoint(2), 1, palette);

      // Then, draw it
      myGraph.add(myPie1);
      myGraph.draw("Graph_Pie_OT", 640, 480);

      // Check that the correct files have been generated by computing their checksum
      fullprint << "bitmap=" << myGraph.getBitmap() << std::endl;
      fullprint << "postscript=" << myGraph.getPostscript() << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}