#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    meixner = MeixnerFactory(10, 0.3)
    for i in range(10):
        print "meixner(", i, ")=", meixner.build(i)
    roots = meixner.getRoots(10)
    print "meixner(10) roots=", repr(roots)
    weights = NumericalPoint()
    nodes = meixner.getNodesAndWeights(10, weights)
    print "meixner(10) nodes=", nodes, "and weights=", weights
except :
    import sys
    print "t_MeixnerFactory_std.py", sys.exc_type, sys.exc_value
