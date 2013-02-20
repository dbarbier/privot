#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try :
    iMax = 5
    distribution = Triangular(-1.0, 0.3, 1.0)
    algo = ChebychevAlgorithm(distribution)
    print "algo=", repr(algo)
    for i in range(iMax):
        print distribution.getClassName() + " polynomial(", i, ")=", algo.getRecurrenceCoefficients(i)
    algo.setReferenceFamily(OrthogonalUniVariatePolynomialFamily(HermiteFactory()))
    print "algo=", repr(algo)
    for i in range(iMax):
        print distribution.getClassName() + " polynomial(", i, ")=", algo.getRecurrenceCoefficients(i)

except :
    import sys
    print "t_ChebychevAlgorithm_std.py", sys.exc_type, sys.exc_value
