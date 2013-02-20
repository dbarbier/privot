#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    levels = NumericalPoint(2, 0.0)
    levels.setName("levels")
    levels[0] = 4
    levels[1] = 4
    myPlane = Box(levels)
    print "myPlane = ", myPlane
    sample = myPlane.generate()
    print "sample = ", repr(sample)

except :
    import sys
    print "t_Box_std.py", sys.exc_type, sys.exc_value
