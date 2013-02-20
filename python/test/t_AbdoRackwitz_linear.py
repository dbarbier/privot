#! /usr/bin/env python

from openturns import *
from math import *

def printNumericalPoint(point, digits) :
    oss = "["
    eps = pow(0.1, digits)
    for i in range(point.getDimension()) :
        if i == 0 :
            sep = ""
        else :
            sep = ","
        if fabs(point[i]) < eps :
            oss += sep + "%.6f" % fabs(point[i])
        else :
            oss += sep + "%.6f" % point[i]
        sep = ","
    oss += "]"
    return oss

TESTPREAMBLE()

try :
    levelFunction = NumericalMathFunction("TestOptimLinear")
    # Add a finite difference gradient to the function, as Abdo Rackwitz algorithm
    # needs it
    myGradient = NonCenteredFiniteDifferenceGradient(1e-7, levelFunction.getEvaluationImplementation())
    print "myGradient = ", repr(myGradient)
    # Substitute the gradient
    levelFunction.setGradientImplementation(NonCenteredFiniteDifferenceGradient(myGradient))
    specific = AbdoRackwitzSpecificParameters()
    startingPoint = NumericalPoint(4, 0.0)
    myAlgorithm = AbdoRackwitz(specific, levelFunction)
    myAlgorithm.setStartingPoint(startingPoint)
    myAlgorithm.setLevelValue(3.0)
    myAlgorithm.run()
    print "result = ", printNumericalPoint(myAlgorithm.getResult().getMinimizer(), 4)
except :
    import sys
    print "t_AbdoRackwitz_linear.py", sys.exc_type, sys.exc_value
