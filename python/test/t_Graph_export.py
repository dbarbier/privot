#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try :
    maxPoint = 101
    xMin = -2.0 * pi
    xMax = 2.0 * pi
    data = NumericalSample(maxPoint, 2)
    for i in range(maxPoint):
        x = xMin + (xMax - xMin) * i / (maxPoint - 1.0)
        data[i, 0] = x
        data[i, 1] = sin(x)
    sinGraph = Graph("Sin function", "x", "y", True, "topright")
    sinGraph.add(Curve(data, "red", "solid", 2, "sin"))
    # Test export to all formats
    sinGraph.draw("sinGraph_ALL", 640, 480, GraphImplementation.ALL)
    print "sinGraph (ALL)=", sinGraph.getBitmap(), ", ", sinGraph.getPostscript(), ", ", sinGraph.getVectorial()
    # Test export to PNG format
    sinGraph.draw("sinGraph_PNG", 640, 480, GraphImplementation.PNG)
    print "sinGraph (PNG)=", sinGraph.getBitmap()
    # Test export to EPS format
    sinGraph.draw("sinGraph_EPS", 640, 480, GraphImplementation.EPS)
    print "sinGraph (EPS)=", sinGraph.getPostscript()
    # Test export to FIG format
    sinGraph.draw("sinGraph_FIG", 640, 480, GraphImplementation.FIG)
    print "sinGraph (FIG)=", sinGraph.getVectorial()
except :
    import sys
    print "t_Graph_export.py", sys.exc_type, sys.exc_value
