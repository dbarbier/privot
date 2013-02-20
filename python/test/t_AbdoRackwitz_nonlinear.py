#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

def printNumericalPoint(point, digits) :
    oss = "["
    eps = pow(0.1, digits)
    for i in range(point.getDimension()) :
        if i == 0 :
            sep = ""
        else :
            sep = ","
        if fabs(point[i]) < eps :
            oss += sep + "%.6f" % point[i]
        else :
            oss += sep + "%.6f" % point[i]
        sep = ","
    oss += "]"
    return oss

#TESTPREAMBLE()

try :
    levelFunction = NumericalMathFunction("TestOptimNonLinear")
    # Activate the cache as we will use an analytical method
    levelFunction.enableCache();
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
    myAlgorithm.setLevelValue(-0.5)
    print "myalgorithm=", repr(myAlgorithm)
    myAlgorithm.run()
    print "result = ", printNumericalPoint(myAlgorithm.getResult().getMinimizer(), 4)
    print "evaluation calls number=", levelFunction.getEvaluationCallsNumber()
    print "gradient   calls number=", levelFunction.getGradientCallsNumber()
    print "hessian    calls number=", levelFunction.getHessianCallsNumber()
except :
    import sys
    print "t_AbdoRackwitz_nonlinear.py", sys.exc_type, sys.exc_value
