#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    #Instance creation */
    deviation = NumericalMathFunction("poutre")

    inPoint = NumericalPoint(4)
    inPoint[0] = 210.e9
    inPoint[1] = 1000
    inPoint[2] = 1.5
    inPoint[3] = 2.e-6

    tensor = deviation.hessian( inPoint )

    print "deviation.hessian = ", repr(tensor)

except :
    import sys
    print "t_NumericalMathFunction_hess.py", sys.exc_type, sys.exc_value
