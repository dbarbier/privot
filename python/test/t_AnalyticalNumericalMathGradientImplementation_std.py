#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    evaluation = AnalyticalNumericalMathEvaluationImplementation(["x0", "x1", "x2"], ["y0"], ["x0^2+2*x1+3*x2^3"])
    gradient = AnalyticalNumericalMathGradientImplementation(evaluation)
    point = [-1.0, 4.0, -4.0]
    print "gradient=", gradient
    print "value at point", point, "=", gradient.gradient(point)

except :
    import sys
    print "t_AnalyticalNumericalMathGradientImplementation_std.py", sys.exc_type, sys.exc_value
