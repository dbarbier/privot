#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    krawtchouk = KrawtchoukFactory(10, 0.3)
    for i in range(10):
        print "krawtchouk(", i, ")=", krawtchouk.build(i)
    roots = krawtchouk.getRoots(10)
    print "krawtchouk(10) roots=", repr(roots)
    weights = NumericalPoint()
    nodes = krawtchouk.getNodesAndWeights(10, weights)
    print "krawtchouk(10) nodes=", nodes, "and weights=", weights
except :
    import sys
    print "t_KrawtchoukFactory_std.py", sys.exc_type, sys.exc_value
