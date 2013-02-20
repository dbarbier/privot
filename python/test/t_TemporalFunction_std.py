#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    # Create an intance
    inputVars = Description(["t", "x"])
    formula = Description(["x + t^2"])
    myFunc = NumericalMathFunction(inputVars, formula)
    myTemporalFunc = TemporalFunction(myFunc)

    print "myTemporalFunc=", myTemporalFunc
    # Get the input and output description
    print "myTemporalFunc input description=", myTemporalFunc.getInputDescription()
    print "myTemporalFunc output description=", myTemporalFunc.getOutputDescription()
    # Get the input and output dimension, based on description
    print "myTemporalFunc input dimension=", myTemporalFunc.getInputDimension()
    print "myTemporalFunc output dimension=", myTemporalFunc.getOutputDimension()
    # Create a TimeSeries
    tg = RegularGrid(0.0, 0.2, 6)
    data = NumericalSample(tg.getN(), myFunc.getInputDimension() - 1)
    for i in range(data.getSize()):
        for j in range(data.getDimension()):
            data[i, j] = i * data.getDimension() + j
    ts = TimeSeries(tg, data)
    print "input time series=", ts
    print "output time series=", myTemporalFunc(ts)
    # Get the number of calls
    print "called ", myTemporalFunc.getCallsNumber(), " times"

except :
    import sys
    print "t_TemporalFunction_std.py", sys.exc_type, sys.exc_value
