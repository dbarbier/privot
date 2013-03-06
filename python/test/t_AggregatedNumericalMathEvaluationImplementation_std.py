#! /usr/bin/env python

from openturns import *

function1 = NumericalMathFunction(["x0", "x1", "x2"], ["y0"], ["x0^2+2*x1+3*x2^3"])
function2 = NumericalMathFunction(["x0", "x1", "x2"], ["z0", "z1"], ["cos(x0*sin(x2+x1))", "exp(x1 - x0 * sin(x2))"])
evaluation = AggregatedNumericalMathEvaluationImplementation([function1, function2])
print "evaluation=", evaluation
point = [4.0, -4.0, 1.0]
print "function 1 at", point, "=", function1(point)
print "function 2 at", point, "=", function2(point)
print "evaluation at", point, "=", evaluation(point)

