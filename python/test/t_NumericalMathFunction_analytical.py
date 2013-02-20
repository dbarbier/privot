#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    elementaryFunctions = Description(0)
    elementaryFunctions.add("sin")
    elementaryFunctions.add("cos")
    elementaryFunctions.add("tan")
    elementaryFunctions.add("asin")
    elementaryFunctions.add("acos")
    elementaryFunctions.add("atan")
    elementaryFunctions.add("sinh")
    elementaryFunctions.add("cosh")
    elementaryFunctions.add("tanh")
    elementaryFunctions.add("asinh")
    elementaryFunctions.add("acosh")
    elementaryFunctions.add("atanh")
    elementaryFunctions.add("log2")
    elementaryFunctions.add("log10")
    elementaryFunctions.add("log")
    elementaryFunctions.add("ln")
    elementaryFunctions.add("lngamma")
    elementaryFunctions.add("gamma")
    elementaryFunctions.add("exp")
    elementaryFunctions.add("erf")
    elementaryFunctions.add("erfc")
    elementaryFunctions.add("sqrt")
    elementaryFunctions.add("cbrt")
    elementaryFunctions.add("besselJ0")
    elementaryFunctions.add("besselJ1")
    elementaryFunctions.add("besselY0")
    elementaryFunctions.add("besselY1")
    elementaryFunctions.add("sign")
    elementaryFunctions.add("rint")
    elementaryFunctions.add("abs")
    # Check the creation of the elementary functions
    x = NumericalPoint(1, 0.4)
    for i in range(elementaryFunctions.getSize()):
        x = NumericalPoint(1, 0.4)
        # acosh only defined for 1 <= x <= pi
        if elementaryFunctions[i] == "acosh":
            x[0] = 1.4

        f = NumericalMathFunction("x", elementaryFunctions[i] + "(x)", "y")
        print "f=", f
        print "f(", x[0], ")=%.4e" % f(x)[0]

        try :
            df = f.gradient(x)[0, 0]
        except :
            f.setGradientImplementation(CenteredFiniteDifferenceGradient(ResourceMap.GetAsNumericalScalar( "CenteredFiniteDifferenceGradient-DefaultEpsilon" ), f.getEvaluationImplementation()))
            df = f.gradient(x)[0, 0]
        print "df(", x[0], ")=%.4e" % df

        try :
            d2f = f.hessian(x)[0, 0, 0]
        except :
            f.setHessianImplementation(CenteredFiniteDifferenceHessian(ResourceMap.GetAsNumericalScalar( "CenteredFiniteDifferenceHessian-DefaultEpsilon" ), f.getEvaluationImplementation()))
            d2f = f.hessian(x)[0, 0, 0]
        print "d2f(", x[0], ")=%.4e" % d2f


    inp = Description(2)
    inp[0] = "x0"
    inp[1] = "x1"
    out = Description(2)
    out[0] = "y0"
    out[1] = "y1"
    form = Description(2)
    form[0] = "x0+x1"
    form[1] = "x0-x1"

    nmf = NumericalMathFunction(inp,out,form)
    marginal0 = nmf.getMarginal(0)
    marginal1 = nmf.getMarginal(1)
    print "marginal 0=", marginal0
    print "marginal 1=", marginal1

except :
    import sys
    print "t_NumericalMathFunction_analytical.py", sys.exc_type, sys.exc_value
