#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try :
    x = NumericalSample()
    y = NumericalSample()
    psi = Basis()

    bsf = LARLasso()
    print "bsf =", bsf
    bs = bsf.build( x, y, psi )
    print "seq =", bs

except :
    import sys
    print "t_LARLasso.py", sys.exc_type, sys.exc_value
