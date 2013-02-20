#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    eps = 1e-2
    # Instance creation
    myFunc = NumericalMathFunction("TestResponseSurface")
    epsilon = NumericalPoint(myFunc.getInputDimension(), eps)
    inPoint = NumericalPoint(epsilon.getDimension(), 1.0)
    myHessian = CenteredFiniteDifferenceHessian(epsilon, myFunc.getEvaluationImplementation())

    print "myHessian=" , repr(myHessian)
    print "myFunc.hessian(" , repr(inPoint) , ")=" , repr(myFunc.hessian(inPoint))
    print "myHessian.hessian(" , repr(inPoint) , ")=" , repr(myHessian.hessian(inPoint))

    # Substitute the hessian
    myFunc.setHessianImplementation(CenteredFiniteDifferenceHessian(myHessian))
    print "myFunc.hessian(" , repr(inPoint) , ")=" , repr(myFunc.hessian(inPoint)) , " (after substitution)"

except :
    import sys
    print "t_CenteredFiniteDifferenceHessian_std.py", sys.exc_type, sys.exc_value
