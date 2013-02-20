#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    charlier = CharlierFactory(2.5)
    print "charlier=",charlier
    for i in range(10):
        print "charlier(", i, ")=", charlier.build(i)
    roots = charlier.getRoots(10)
    print "charlier(10) roots=", repr(roots)
    weights = NumericalPoint()
    nodes = charlier.getNodesAndWeights(10, weights)
    print "charlier(10) nodes=", nodes, "and weights=", weights
except :
    import sys
    print "t_CharlierFactory_std.py", sys.exc_type, sys.exc_value
