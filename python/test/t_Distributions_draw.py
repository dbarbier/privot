#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
pointNumber = 251

try :
    dist1D = Normal(2.0, 4.0)
    dist2D = Normal(NumericalPoint(2, 2.0), NumericalPoint(2, 4.0), CorrelationMatrix(2))
    distND = Normal(NumericalPoint(5, 2.0), NumericalPoint(5, 4.0), CorrelationMatrix(5))
    # Check drawing methods for 1D distributions
    graphPDF = dist1D.drawPDF()
    graphPDF.draw("DefaultDrawPDF_1D")
    print "graphPDF=", graphPDF.getBitmap()
    graphPDF = dist1D.drawPDF(-4.0, 4.0, 101)
    graphPDF.draw("FullParametersDrawPDF_1D")
    print "graphPDF=", graphPDF.getBitmap()
    graphPDF = dist1D.drawPDF(-4.0, 4.0)
    graphPDF.draw("RangeDrawPDF_1D")
    print "graphPDF=", graphPDF.getBitmap()
    graphPDF = dist1D.drawPDF(101)
    graphPDF.draw("PointNumberDrawPDF_1D")
    print "graphPDF=", graphPDF.getBitmap()
    graphCDF = dist1D.drawCDF()
    graphCDF.draw("DefaultDrawCDF_1D")
    print "graphCDF=", graphCDF.getBitmap()
    graphCDF = dist1D.drawCDF(-4.0, 4.0, 101)
    graphCDF.draw("FullParametersDrawCDF_1D")
    print "graphCDF=", graphCDF.getBitmap()
    graphCDF = dist1D.drawCDF(-4.0, 4.0)
    graphCDF.draw("RangeDrawCDF_1D")
    print "graphCDF=", graphCDF.getBitmap()
    graphCDF = dist1D.drawCDF(101)
    graphCDF.draw("PointNumberDrawCDF_1D")
    print "graphCDF=", graphCDF.getBitmap()
    # Check drawing methods for 2D distributions
    graphPDF = dist2D.drawPDF()
    graphPDF.draw("DefaultDrawPDF_2D")
    print "graphPDF=", graphPDF.getBitmap()
    graphPDF = dist2D.drawPDF([-4.0, -4.0], [4.0, 4.0], [101, 101])
    graphPDF.draw("FullParametersDrawPDF_2D")
    print "graphPDF=", graphPDF.getBitmap()
    graphPDF = dist2D.drawPDF([-4.0, -4.0], [4.0, 4.0])
    graphPDF.draw("RangeDrawPDF_2D")
    print "graphPDF=", graphPDF.getBitmap()
    graphPDF = dist2D.drawPDF([101, 101])
    graphPDF.draw("PointNumberDrawPDF_2D")
    print "graphPDF=", graphPDF.getBitmap()
    graphCDF = dist2D.drawCDF()
    graphCDF.draw("DefaultDrawCDF_2D")
    print "graphCDF=", graphCDF.getBitmap()
    graphCDF = dist2D.drawCDF([-4.0, -4.0], [4.0, 4.0], [101, 101])
    graphCDF.draw("FullParametersDrawCDF_2D")
    print "graphCDF=", graphCDF.getBitmap()
    graphCDF = dist2D.drawCDF([-4.0, -4.0], [4.0, 4.0])
    graphCDF.draw("RangeDrawCDF_2D")
    print "graphCDF=", graphCDF.getBitmap()
    graphCDF = dist2D.drawCDF([101, 101])
    graphCDF.draw("PointNumberDrawCDF_2D")
    print "graphCDF=", graphCDF.getBitmap()
    # Check drawing methods for ND distributions
    graphPDF = distND.drawMarginal1DPDF(2, -4.0, 4.0, 101)
    graphPDF.draw("FullParametersDrawMarginal1DPDF_ND")
    print "graphPDF=", graphPDF.getBitmap()
    graphPDF = distND.drawMarginal2DPDF(2, 3, [-4.0, -4.0], [4.0, 4.0], [101, 101])
    graphPDF.draw("FullParametersDrawMarginal2DPDF_ND")
    print "graphPDF=", graphPDF.getBitmap()
    graphCDF = distND.drawMarginal1DCDF(2, -4.0, 4.0, 101)
    graphCDF.draw("FullParametersDrawMarginal1DCDF_ND")
    print "graphCDF=", graphCDF.getBitmap()
    graphCDF = distND.drawMarginal2DCDF(2, 3, [-4.0, -4.0], [4.0, 4.0], [101, 101])
    graphCDF.draw("FullParametersDrawMarginal2DCDF_ND")
    print "graphCDF=", graphCDF.getBitmap()
except  :
    import sys
    print "t_Distributions_draw.py", sys.exc_type, sys.exc_value
