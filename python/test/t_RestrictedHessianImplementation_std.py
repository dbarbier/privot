#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    evaluation = AnalyticalNumericalMathEvaluationImplementation(["x0", "x1", "x2"], ["y0"], ["x0^2+2*x1+3*x2^3"])
    hessian = AnalyticalNumericalMathHessianImplementation(evaluation)
    restrictionIndices = [1, 2]
    restrictionIndices[0] = 1
    restrictionIndices[1] = 2
    referencePoint = [-1.0, 1.0, 3.0]
    restriction = RestrictedHessianImplementation(hessian, restrictionIndices, referencePoint)
    print "restricted evaluation=", restriction
    point = [4.0, -4.0]
    print "value at ", point, "=", restriction.hessian(point)
    point = [-1.0, 4.0, -4.0]
    print "reference value=", hessian.hessian(point)

except :
    import sys
    print "t_RestrictedHessianImplementation_std.py", sys.exc_type, sys.exc_value
