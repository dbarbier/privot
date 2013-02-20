#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    try:
        #Instance creation
        deviation = NumericalMathFunction("poutre")

        inPoint = NumericalPoint(4)
        inPoint[0] = 210.e9  # E
        inPoint[1] = 1000    # F
        inPoint[2] = 1.5     # L
        inPoint[3] = 0.      # I is zero so an error should be raised

        outPoint = deviation( inPoint )

        # Normally, we should never go here
        raise

    except :
        pass

except :
    import sys
    print "t_NumericalMathFunction_exec.py", sys.exc_type, sys.exc_value
