//                                               -*- C++ -*-
/**
 *  @file  t_Distributions_draw.cxx
 *  @brief The test file of class Normal for standard methods
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
  // UnsignedLong pointNumber(251);

  try
    {
      Normal dist1D(2.0, 4.0);
      Normal dist2D(NumericalPoint(2, 2.0), NumericalPoint(2, 4.0), CorrelationMatrix(2));
      Normal distND(NumericalPoint(5, 2.0), NumericalPoint(5, 4.0), CorrelationMatrix(5));
      // Check drawing methods for 1D distributions
      Graph graphPDF(dist1D.drawPDF());
      graphPDF.draw("DefaultDrawPDF_1D");
      fullprint << "graphPDF=" << graphPDF.getBitmap() << std::endl;
      graphPDF = dist1D.drawPDF(-4.0, 4.0, 101);
      graphPDF.draw("FullParametersDrawPDF_1D");
      fullprint << "graphPDF=" << graphPDF.getBitmap() << std::endl;
      graphPDF = dist1D.drawPDF(-4.0, 4.0);
      graphPDF.draw("RangeDrawPDF_1D");
      fullprint << "graphPDF=" << graphPDF.getBitmap() << std::endl;
      graphPDF = dist1D.drawPDF(101);
      graphPDF.draw("PointNumberDrawPDF_1D");
      fullprint << "graphPDF=" << graphPDF.getBitmap() << std::endl;
      Graph graphCDF(dist1D.drawCDF());
      graphCDF.draw("DefaultDrawCDF_1D");
      fullprint << "graphCDF=" << graphCDF.getBitmap() << std::endl;
      graphCDF = dist1D.drawCDF(-4.0, 4.0, 101);
      graphCDF.draw("FullParametersDrawCDF_1D");
      fullprint << "graphCDF=" << graphCDF.getBitmap() << std::endl;
      graphCDF = dist1D.drawCDF(-4.0, 4.0);
      graphCDF.draw("RangeDrawCDF_1D");
      fullprint << "graphCDF=" << graphCDF.getBitmap() << std::endl;
      graphCDF = dist1D.drawCDF(101);
      graphCDF.draw("PointNumberDrawCDF_1D");
      fullprint << "graphCDF=" << graphCDF.getBitmap() << std::endl;
      // Check drawing methods for 2D distributions
      graphPDF = dist2D.drawPDF();
      graphPDF.draw("DefaultDrawPDF_2D");
      fullprint << "graphPDF=" << graphPDF.getBitmap() << std::endl;
      graphPDF = dist2D.drawPDF(NumericalPoint(2, -4.0), NumericalPoint(2, 4.0), Indices(2, 101));
      graphPDF.draw("FullParametersDrawPDF_2D");
      fullprint << "graphPDF=" << graphPDF.getBitmap() << std::endl;
      graphPDF = dist2D.drawPDF(NumericalPoint(2, -4.0), NumericalPoint(2, 4.0));
      graphPDF.draw("RangeDrawPDF_2D");
      fullprint << "graphPDF=" << graphPDF.getBitmap() << std::endl;
      graphPDF = dist2D.drawPDF(Indices(2, 101));
      graphPDF.draw("PointNumberDrawPDF_2D");
      fullprint << "graphPDF=" << graphPDF.getBitmap() << std::endl;
      graphCDF = dist2D.drawCDF();
      graphCDF.draw("DefaultDrawCDF_2D");
      fullprint << "graphCDF=" << graphCDF.getBitmap() << std::endl;
      graphCDF = dist2D.drawCDF(NumericalPoint(2, -4.0), NumericalPoint(2, 4.0), Indices(2, 101));
      graphCDF.draw("FullParametersDrawCDF_2D");
      fullprint << "graphCDF=" << graphCDF.getBitmap() << std::endl;
      graphCDF = dist2D.drawCDF(NumericalPoint(2, -4.0), NumericalPoint(2, 4.0));
      graphCDF.draw("RangeDrawCDF_2D");
      fullprint << "graphCDF=" << graphCDF.getBitmap() << std::endl;
      graphCDF = dist2D.drawCDF(Indices(2, 101));
      graphCDF.draw("PointNumberDrawCDF_2D");
      fullprint << "graphCDF=" << graphCDF.getBitmap() << std::endl;
      // Check drawing methods for ND distributions
      graphPDF = distND.drawMarginal1DPDF(2, -4.0, 4.0, 101);
      graphPDF.draw("FullParametersDrawMarginal1DPDF_ND");
      fullprint << "graphPDF=" << graphPDF.getBitmap() << std::endl;
      graphPDF = distND.drawMarginal2DPDF(2, 3, NumericalPoint(2, -4.0), NumericalPoint(2, 4.0), Indices(2, 101));
      graphPDF.draw("FullParametersDrawMarginal2DPDF_ND");
      fullprint << "graphPDF=" << graphPDF.getBitmap() << std::endl;
      graphCDF = distND.drawMarginal1DCDF(2, -4.0, 4.0, 101);
      graphCDF.draw("FullParametersDrawMarginal1DCDF_ND");
      fullprint << "graphCDF=" << graphCDF.getBitmap() << std::endl;
      graphCDF = distND.drawMarginal2DCDF(2, 3, NumericalPoint(2, -4.0), NumericalPoint(2, 4.0), Indices(2, 101));
      graphCDF.draw("FullParametersDrawMarginal2DCDF_ND");
      fullprint << "graphCDF=" << graphCDF.getBitmap() << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
