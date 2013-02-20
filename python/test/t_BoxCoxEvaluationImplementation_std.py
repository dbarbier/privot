#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    # Dimension of test
    dimension = 3

    # Lambda
    lambdaVector = NumericalPoint(dimension)
    lambdaVector[0] = 0.
    lambdaVector[1] = 0.5
    lambdaVector[2] = 1

    # Init of the function
    myFunction = BoxCoxEvaluationImplementation(lambdaVector)
    myFunction.setName("myBoxCoxFunction")

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
    print "t__BoxCoxEvaluationImplementation_std.py", sys.exc_type, sys.exc_value
