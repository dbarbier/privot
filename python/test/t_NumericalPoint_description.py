#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    # Constructor with size
    point1 = NumericalPointWithDescription(2)
    point1[0] = 100.
    point1[1] = 101.

    # Copy constructor
    point2 = NumericalPointWithDescription(point1)

    # Description
    print "point1.getDescription() = ", repr(point1.getDescription())
    print "point2.getDescription() = ", repr(point2.getDescription())

    description = Description(2)
    description[0] = "point1_X"
    description[1] = "point1_Y"

    point1.setDescription(description)

    print "point1.getDescription() = ", repr(point1.getDescription())
    print "point2.getDescription() = ", repr(point2.getDescription())

except :
    import sys
    print "t_NumericalPoint_description.py", sys.exc_type, sys.exc_value
