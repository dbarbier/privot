#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    eps = 1e-2
    # Instance creation
    myFunc = NumericalMathFunction("TestResponseSurface")
    print "myFunc (before substitution) = ", repr(myFunc)
    epsilon = NumericalPoint(myFunc.getInputDimension(), eps)
    inPoint = NumericalPoint(epsilon.getDimension(), 1.0)
    myGradient = CenteredFiniteDifferenceGradient(epsilon, myFunc.getEvaluationImplementation())

    print "myGradient=" , repr(myGradient)
    print "myFunc.gradient(" , repr(inPoint) , ")=" , repr(myFunc.gradient(inPoint))
    print "myGradient.gradient(" , repr(inPoint) , ")=" , repr(myGradient.gradient(inPoint))

    # Substitute the gradient
    myFunc.setGradientImplementation(myGradient)
    print "myFunc (after substitution) = ", repr(myFunc)

    print "myFunc.gradient(" , repr(inPoint) , ")=" , repr(myFunc.gradient(inPoint)) , " (after substitution)"

except :
    import sys
    print "t_CenteredFiniteDifferenceGradient_std.py", sys.exc_type, sys.exc_value
