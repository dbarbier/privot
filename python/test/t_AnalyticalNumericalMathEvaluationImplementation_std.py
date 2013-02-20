#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    evaluation = AnalyticalNumericalMathEvaluationImplementation(["x0", "x1", "x2"], ["y0", "y1"], ["x0^2+2*x1+3*x2^3", "cos(x0-sin(x1 * x2))"])
    point = [-1.0, 4.0, -4.0]
    print "evaluation=", evaluation
    print "value at ", point, "=", evaluation(point)
    for i in range(evaluation.getOutputDimension()):
        print "marginal", i, "=", evaluation.getMarginal(i)

except :
    import sys
    print "t_AnalyticalNumericalMathEvaluationImplementation_std.py", sys.exc_type, sys.exc_value
