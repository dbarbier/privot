#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    # Analytical construction
    input = Description(1)
    input[0] = "x"
    output = Description(1)
    output[0] = "y"
    formulas = Description(output.getSize())
    formulas[0] = "x-cos(x)"
    analytical = NumericalMathFunction(input, output, formulas)
    myAlgo = Brent()
    print "myAlgo=" , myAlgo
    value = 0.0
    print "Solve " , formulas[0] , "=%.5f" % value , " for " , input[0]
    print "x=%.5f" % myAlgo.solve(analytical, value, -4.0, 4.0 / 3.0)
    print "myAlgo=" , myAlgo

except  :
    import sys
    print "t_Brent.py", sys.exc_type, sys.exc_value
