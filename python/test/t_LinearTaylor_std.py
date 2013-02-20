#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    eps = 0.2
    # Instance creation
    myFunc = NumericalMathFunction("TestResponseSurface")
    center = NumericalPoint(myFunc.getInputDimension())
    for i in range(center.getDimension()) :
        center[i] = 1.0+i
    myTaylor = LinearTaylor(center, myFunc)
    myTaylor.run()
    responseSurface = NumericalMathFunction(myTaylor.getResponseSurface())
    print "myTaylor=" , repr(myTaylor )
    print "responseSurface=" , repr(responseSurface )
    print "myFunc(" , repr(center) , ")=" , repr(myFunc(center))
    print "responseSurface(" , repr(center) , ")=" , repr(responseSurface(center))
    inPoint = NumericalPoint(center)
    inPoint[0] += eps
    inPoint[1] -= eps/2
    print "myFunc(" , repr(inPoint) , ")=" , repr(myFunc(inPoint))
    print "responseSurface(" , repr(inPoint) , ")=" , repr(responseSurface(inPoint))

except :
    import sys
    print "t_LinearTaylor_std.py", sys.exc_type, sys.exc_value
