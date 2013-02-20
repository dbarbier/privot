#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    center = NumericalPoint(4, 0.0)
    center.setName("center")
    center[0] = 0.5
    center[1] = 1.5
    center[2] = 2.5
    center[3] = 3.5
    levels = NumericalPoint(3, 0.0)
    levels.setName("levels")
    levels[0] = 4
    levels[1] = 8
    levels[2] = 16
    myPlane = Factorial(center, levels)
    print "myPlane = " , myPlane
    sample = myPlane.generate()
    print "sample = " , repr(sample)

except :
    import sys
    print "t_Factorial_std.py", sys.exc_type, sys.exc_value
