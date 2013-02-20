#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    myFunc = NumericalMathFunction("TestResponseSurface")
    data = NumericalSample(9, myFunc.getInputDimension())
    point = NumericalPoint(myFunc.getInputDimension())
    point[0] = 0.5
    point[1] = 0.5
    data[0] = point
    point[0] = -1
    point[1] = -1
    data[1] = point
    point[0] = -1
    point[1] = 1
    data[2] = point
    point[0] = 1
    point[1] = -1
    data[3] = point
    point[0] = 1
    point[1] = 1
    data[4] = point
    point[0] = -0.5
    point[1] = -0.5
    data[5] = point
    point[0] = -0.5
    point[1] = 0.5
    data[6] = point
    point[0] = 0.5
    point[1] = -0.5
    data[7] = point
    point[0] = 0.5
    point[1] = 0.5
    data[8] = point
    myLeastSquares = QuadraticLeastSquares(data, myFunc)
    myLeastSquares.run()
    responseSurface = myLeastSquares.getResponseSurface()
    print "myLeastSquares=", repr(myLeastSquares)
    print "responseSurface=", repr(responseSurface)
    inPoint = NumericalPoint(myFunc.getInputDimension(), 0.2)
    print "myFunc(", repr(inPoint), ")=", repr(myFunc(inPoint))
    print "responseSurface(", repr(inPoint), ")=", repr(responseSurface(inPoint))
    dataOut = myFunc(data)
    myLeastSquares = QuadraticLeastSquares(data, dataOut)
    myLeastSquares.run()
    responseSurface = myLeastSquares.getResponseSurface()
    print "myLeastSquares=", repr(myLeastSquares)
    print "responseSurface=", repr(responseSurface)
    inPoint = NumericalPoint(myFunc.getInputDimension(), 0.2)
    print "myFunc(", repr(inPoint), ")=", repr(myFunc(inPoint))
    print "responseSurface(", repr(inPoint), ")=", repr(responseSurface(inPoint))
except :
    import sys
    print "t_QuadraticLeastSquares_std.py", sys.exc_type, sys.exc_value
