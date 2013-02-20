#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    # Create an intance
    myFunc = DynamicalFunction()

    print "myFunc=", myFunc
    # Get the input and output description
    print "myFunc input description=", myFunc.getInputDescription()
    print "myFunc output description=", myFunc.getOutputDescription()
    # Get the input and output dimension, based on description
    print "myFunc input dimension=", myFunc.getInputDimension()
    print "myFunc output dimension=", myFunc.getOutputDimension()
    # Get the number of calls
    print "called ", myFunc.getCallsNumber(), " times"

except :
    import sys
    print "t_DynamicalFunction_std.py", sys.exc_type, sys.exc_value
