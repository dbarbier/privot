#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try :

    # Generate the data for the curves to be drawn
    size = 50
    cursor = NumericalPoint(2)

    #curve y = 2x for x in [-25]
    data1 = NumericalSample(size, 2)
    #curve y = x*x for x in [-11]
    data2 = NumericalSample(size, 2)

    for i in range(size) :
        tmp = 7.*i/size+2
        cursor[0] = tmp
        cursor[1] = 2*tmp
        data1[i] = cursor

        tmp = 9.*i/size+1
        cursor[0] = tmp
        cursor[1] = tmp*tmp - (tmp -5) * tmp * tmp / 60
        data2[i] = cursor

    # Create an empty graph
    myGraph = Graph("Some curves", "x1", "x2", True, "topright", 1.0, 0)

    # Create the first curve
    myCurve1 = Curve(data1, "blue", "dashed", 2, "linear function")

    # Then, draw it
    myGraph.add(myCurve1)
    myGraph.draw("Graph_Curve_a_OT", 640, 480)

    # Check that the correct files have been generated by computing their checksum
    print  "bitmap=" , myGraph.getBitmap()
    print  "postscript=" , myGraph.getPostscript()

    # Create the second curve
    myCurve2 = Curve(data2, "red", "solid", 2, "polynomial function")

    # Add it to the graph and draw everything
    myGraph.add(myCurve2)
    for i in range(4):
        myGraph.setLogScale(i)
        myGraph.draw("Graph_Curve_b_OT_" + str(i), 640, 480)
        print  "bitmap=" , myGraph.getBitmap()
        print  "postscript=" , myGraph.getPostscript()

except :
    import sys
    print "t_Curve_std.py", sys.exc_type, sys.exc_value
