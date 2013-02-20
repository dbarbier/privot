#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    #Instance creation
    myFunc = NumericalMathFunction("TestCompositionLeft")

    #Copy constructor
    newFunc = NumericalMathFunction(myFunc)

    print "myFunc="+ repr(myFunc)
    print "myFunc input parameter(s)="
    for i in range(myFunc.getInputDimension()) :
        print myFunc.getInputDescription()[i]
    print "myFunc output parameter(s) and marginal(s)="
    for i in range(myFunc.getOutputDimension()) :
        print myFunc.getOutputDescription()[i]
        print "Marginal function", i, "=", repr(myFunc.getMarginal(i))

except :
    import sys
    print "t_NumericalMathFunction_std.py", sys.exc_type, sys.exc_value
