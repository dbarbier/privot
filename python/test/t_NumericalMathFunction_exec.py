#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    #Instance creation
    deviation = NumericalMathFunction("poutre")

    outPoint = deviation( (210.e9, 1000, 1.5, 2.e-6) )

    print "deviation =", repr(outPoint)

except :
    import sys
    print "t_NumericalMathFunction_exec.py", sys.exc_type, sys.exc_value
