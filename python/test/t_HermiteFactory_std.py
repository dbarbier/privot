#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    hermite = HermiteFactory()
    print "hermite=",hermite
    for i in range(10):
        print "hermite(",i,")=",hermite.build(i)
    roots = hermite.getRoots(10)
    print "hermite(10) roots=", repr(roots)
    weights = NumericalPoint()
    nodes = hermite.getNodesAndWeights(10, weights)
    print "hermite(10) nodes=", nodes, "and weights=", weights
except :
    import sys
    print "t_HermiteFactory_std.py", sys.exc_type, sys.exc_value
