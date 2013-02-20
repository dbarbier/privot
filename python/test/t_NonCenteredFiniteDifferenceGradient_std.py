#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    eps = 1e-4
    # Instance creation
    myFunc = NumericalMathFunction("TestResponseSurface")
    epsilon = NumericalPoint(myFunc.getInputDimension(), eps)
    inPoint = NumericalPoint(epsilon.getDimension(), 1.0)
    myGradient = NonCenteredFiniteDifferenceGradient(epsilon, myFunc.getEvaluationImplementation())

    print "myGradient=" , repr(myGradient)
    print "myFunc.gradient(" , repr(inPoint) , ")=" , repr(myFunc.gradient(inPoint))
    print "myGradient.gradient(" , repr(inPoint) , ")=" , repr(myGradient.gradient(inPoint))
    # Substitute the gradient
#DEBUG ici
    myFunc.setGradientImplementation(NonCenteredFiniteDifferenceGradient(myGradient))
    print "myFunc.gradient(" , repr(inPoint) , ")=" , repr(myFunc.gradient(inPoint)) , " (after substitution)"

except :
    print "t_NonCenteredFiniteDifferenceGradient_std.py"
