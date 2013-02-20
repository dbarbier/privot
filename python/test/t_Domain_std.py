#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    # The 1D interface
    dim = 2
    a = NumericalPoint(dim, -1.0);
    b = NumericalPoint(dim, 2.0);
    domain  = Domain(Interval(a, b))
    p1 = NumericalPoint(dim, 0.5)
    p2 = NumericalPoint(dim, 2.5)
    print "Domain=", domain
    # Accessors
    print "Dimension=", domain.getDimension()
    # Contains
    print  "is point " , p1 , " inside " ,  domain ,  "? " ,  domain.contains(p1)
    print  "is point " , p2 , " inside " ,  domain ,  "? " ,  domain.contains(p2)
except :
    import sys
    print "t_Domain_std.py", sys.exc_type, sys.exc_value
