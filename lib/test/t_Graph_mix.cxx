//                                               -*- C++ -*-
/**
 *  @file  t_Graph_mix.cxx
 *  @brief The test file of Graph class for EDF case number 9
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

      // Instanciate one distribution object
      UnsignedLong dim(2);
      NumericalPoint meanPoint(dim, 1.0);
      meanPoint[0] = 12.0;
      meanPoint[1] = 5.0;
      NumericalPoint sigma(dim, 1.0);
      sigma[0] = 3.0;
      sigma[1] = 1.0;
      CorrelationMatrix R = IdentityMatrix(dim);
      for (UnsignedLong i = 1; i < dim; i++)
        {
          R(i, i - 1) = 0.75;
        }


      Normal distribution(meanPoint, sigma, R);

      // Test for sampling
      UnsignedLong size = 200;
      NumericalSample sample(distribution.getSample( size ));
      NumericalSample tmp(2, NumericalPoint(2));
      tmp[0][0] = sample.getMin()[0];
      tmp[0][1] = meanPoint[1] - sigma[1] / sigma[0] * (meanPoint[0] - tmp[0][0]);
      tmp[1][0] = sample.getMax()[0];
      tmp[1][1] = meanPoint[1] - sigma[1] / sigma[0] * (meanPoint[0] - tmp[1][0]);

      // Create the cloud
      Cloud myCloud(sample, "blue", "fsquare", "Normal Cloud");

      // Create the line
      Curve myLine(tmp, "red", "solid", 2, "Line", true);

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

      NumericalPoint centre(2);
      centre[0] = 7.0;
      centre[1] = 10.0;
      // Create the piechart
#ifdef WIN32
      OT:: // mingw defined a Pie(...) function
#endif
        Pie myPie(data, labels, centre, 4, palette);

      // Create an empty graph
      Graph myGraph("Cloud/line/pie", "x1", "x2", true, "topright", 1.0);

      // Then, draw it
      myGraph.add(myCloud);
      myGraph.add(myLine);
      myGraph.add(myPie);
      myGraph.draw("Graph_mix_OT", 640, 480);


      fullprint << "bitmap=" << myGraph.getBitmap() << std::endl;
      fullprint << "postscript=" << myGraph.getPostscript() << std::endl;
      // Test if a drawable can be extracted from a graph to be added to another graph
      Graph firstGraph("Overall graph", "x", "y", true, "topright");
      firstGraph.add(myCloud);
      Graph secondGraph("temporary graph", "x", "y", true, "topright");
      secondGraph.add(myLine);
      // Extract the drawable from the second graph and add it to the first graph
      firstGraph.add(secondGraph.getDrawables()[0]);
      firstGraph.draw("OverallGraph");

      fullprint << "bitmap=" << firstGraph.getBitmap() << std::endl;
      fullprint << "postscript=" << firstGraph.getPostscript() << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
