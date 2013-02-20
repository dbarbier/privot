#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    # First, build two functions from R^3->R
    f1 = NumericalMathFunction(["x1", "x2", "x3"], ["y"], ["x1^3 * sin(x2 + 2.5 * x3) - (x1 + x2)^2 / (1.0 + x3^2)"])
    f2 = NumericalMathFunction(["x1", "x2", "x3"], ["y", "z"], ["exp(-x1 * x2 + x3) / cos(1.0 + x2 * x3 - x1)", "sin(x2+x3*x1^2)"])
    # Second, build the function
    myFunction = NumericalMathFunction(f1 * f2)
    inPoint = NumericalPoint([1.2, 2.3, 3.4])
    print "myFunction=", myFunction
    print "myFunction=", myFunction.__str__()
    value = myFunction(inPoint)
    print "Value at ", inPoint, "="
    print value
    print "Value at ", inPoint, "="
    print value.__str__()
    gradient = myFunction.gradient(inPoint)
    print "Gradient at ", inPoint, "="
    print gradient
    print "Gradient at ", inPoint, "="
    print gradient.__str__()
    hessian = myFunction.hessian(inPoint)
    print "Hessian at ", inPoint, "="
    print hessian
    print "Hessian at ", inPoint, "="
    print hessian.__str__()

except :
    import sys
    print "t_NumericalMathFunction_product.py", sys.exc_type, sys.exc_value
