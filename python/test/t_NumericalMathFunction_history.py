#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    f = NumericalMathFunction("x", "x^2", "y")
    size = 4
    inputSample = NumericalSample(size, 1)
    for i in range(size):
        inputSample[i, 0] = i
    outputSample = f(inputSample)
    print "Is history enabled for f? ", f.isHistoryEnabled()
    print "input history=", f.getInputHistory()
    print "output history=", f.getOutputHistory()
    f.enableHistory()
    outputSample = f(inputSample)
    print "Is history enabled for f? ", f.isHistoryEnabled()
    print "input history=", f.getInputHistory()
    print "output history=", f.getOutputHistory()
    f.clearHistory()
    print "Is history enabled for f? ", f.isHistoryEnabled()
    print "input history=", f.getInputHistory()
    print "output history=", f.getOutputHistory()
    # Perform the computation twice
    outputSample = f(inputSample)
    outputSample = f(inputSample)
    print "input history=", f.getInputHistory()
    print "output history=", f.getOutputHistory()

except :
    import sys
    print "t_NumericalMathFunction_history.py", sys.exc_type, sys.exc_value
