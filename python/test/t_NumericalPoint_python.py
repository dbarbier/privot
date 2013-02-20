#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    #Instance creation
    pt1 = NumericalPoint( [1.1, 2.2, 3.3] )
    print repr(pt1)

    #Instance creation
    pt2 = NumericalPoint( (1.1, 2.2, 3.3) )
    print repr(pt2)

    print "Equality ? ", (pt1 == pt2)

    # Slicing
    print 'rslice[:]=', pt1[:]
    print 'rslice[1:4]=', pt1[1:4]

except :
    import sys
    print "t_NumericalPoint_python.py", sys.exc_type, sys.exc_value
