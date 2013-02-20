#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    # Create an intance
    myFunc = NumericalMathFunction("x", "x^2")
    mySpatialFunc = SpatialFunction(myFunc)

    print "mySpatialFunc=", mySpatialFunc
    # Get the input and output description
    print "mySpatialFunc input description=", mySpatialFunc.getInputDescription()
    print "mySpatialFunc output description=", mySpatialFunc.getOutputDescription()
    # Get the input and output dimension, based on description
    print "mySpatialFunc input dimension=", mySpatialFunc.getInputDimension()
    print "mySpatialFunc output dimension=", mySpatialFunc.getOutputDimension()
    # Create a TimeSeries
    tg = RegularGrid(0.0, 0.2, 6)
    data = NumericalSample(tg.getN(), myFunc.getInputDimension())
    for i in range(data.getSize()):
        for j in range(data.getDimension()):
            data[i, j] = i * data.getDimension() + j
    ts = TimeSeries(tg, data)
    print "input time series=", ts
    print "output time series=", mySpatialFunc(ts)
    # Get the number of calls
    print "called ", mySpatialFunc.getCallsNumber(), " times"

except :
    import sys
    print "t_SpatialFunction_std.py", sys.exc_type, sys.exc_value
