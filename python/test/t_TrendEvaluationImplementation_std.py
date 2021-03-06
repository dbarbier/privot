#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    # Dimension of test
    dimension = 2

    # function
    function = NumericalMathFunction("x", "sin(x)")

    # Init of the function
    myFunction = TrendEvaluationImplementation(function)
    myFunction.setName("myTrendFunction")

    # We check that for a NumericalPoint with same values, the function is done by component
    inPoint = NumericalPoint(dimension, 30.0)

    # result point of the function
    outPoint = myFunction( inPoint )
    print  "myFunction=" , myFunction
    print  myFunction.getName() , "( " , inPoint , " ) = " , outPoint

    # Creation of a NumericalSample
    size = 10
    inSample = NumericalSample(size, dimension)
    for index in range(size):
        inSample[index] = NumericalPoint(dimension, index + 1)

    # result of the function
    outSample = myFunction( inSample )
    print  myFunction.getName() , "( " , repr(inSample) , " ) = " , repr(outSample)


except :
    import sys
    print "t__TrendEvaluationImplementation_std.py", sys.exc_type, sys.exc_value
