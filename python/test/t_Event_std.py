#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    # We create a numerical point of dimension 1
    point = NumericalPoint(4)
    ref_point = point

    point[0] = 101
    point[1] = 202
    point[2] = 303
    point[3] = 404

    print "point = " , repr(ref_point)


    # We create a 'constant' RandomVector from the NumericalPoint
    vect = RandomVector(point)
    print "vect=" , vect

    # Check standard methods of class RandomVector
    print "vect dimension=" , vect.getDimension()
    print "vect realization (first )=" , repr(vect.getRealization())
    print "vect sample =" , repr(vect.getSample(5))

    # We create a numerical math function
    myFunction = NumericalMathFunction("poutre")

    # We create a composite random vector
    output = RandomVector(myFunction, vect)

    # We create an Event from this RandomVector

    myEvent = Event(output,ComparisonOperator(Less()), 50)
    print "myEvent=" , repr(myEvent)

    # We compute one realization of the event
    print "myEvent realization=" , repr(myEvent.getRealization())

    # We compute a sample of the event
    print "myEvent sample=" , repr(myEvent.getSample(10))

except :
    import sys
    print "t_Event_std.py", sys.exc_type, sys.exc_value
