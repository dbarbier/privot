#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try :
    # Create an intance
    inputVars = Description(["t"])
    formula = Description(["sin(t)", "cos(t)"])
    myFunc = NumericalMathFunction(inputVars, formula)
    myTrendFunc = TrendTransform(myFunc)

    print "myTrendFunc=", myTrendFunc
    # Get the input description and dimension
    print "myTrendFunc input description=", myTrendFunc.getInputDescription()
    print "myTrendFunc input dimension=", myTrendFunc.getInputDimension()
    # Get the output description and dimension
    print "myTrendFunc output description=", myTrendFunc.getOutputDescription()
    print "myTrendFunc output dimension=", myTrendFunc.getOutputDimension()
    # Create a TimeSeries
    tg = RegularGrid(0.0, 0.1, 11)
    data = NumericalSample(tg.getN(), formula.getSize())
    for i in range(data.getSize()):
        for j in range(data.getDimension()):
            data[i, j] = i * data.getDimension() + j
    ts = TimeSeries(tg, data)
    print "input time series =  "
    print ts
    print "output time series = "
    print myTrendFunc(ts)
    # Get the number of calls
    print "called ", myTrendFunc.getCallsNumber(), " times"

except :
    import sys
    print "t_TrendTransform_std.py", sys.exc_type, sys.exc_value
