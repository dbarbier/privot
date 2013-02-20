#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    laguerre = LaguerreFactory(2.5, LaguerreFactory.PROBABILITY)
    print "laguerre=", laguerre
    laguerre = LaguerreFactory(2.5, LaguerreFactory.ANALYSIS)
    print "laguerre=", laguerre
    laguerre = LaguerreFactory(2.5)
    print "laguerre=", laguerre
    for i in range(10):
        print "laguerre(", i, ")=", laguerre.build(i)
    roots = laguerre.getRoots(10)
    print "laguerre(10) roots=", repr(roots)
    weights = NumericalPoint()
    nodes = laguerre.getNodesAndWeights(10, weights)
    print "laguerre(10) nodes=", nodes, "and weights=", weights
except :
    import sys
    print "t_LaguerreFactory_std.py", sys.exc_type, sys.exc_value
