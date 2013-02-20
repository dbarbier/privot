#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    alpha = NumericalPoint(2)
    alpha[0] = 0.90
    alpha[1] = 0.95
    beta = NumericalPoint(2)
    beta[0] = 0.90
    beta[1] = 0.95
    for a in range(2) :
        for b in range(2) :
            print "alpha=",alpha[a] , "beta=", beta[b]
            for i in range(5) :
                print "n(",alpha[a],",",beta[b],",",i,")=", Wilks.ComputeSampleSize(alpha[a],beta[b],i)

except :
    import sys
    print "t_Wilks_std.py", sys.exc_type, sys.exc_value
