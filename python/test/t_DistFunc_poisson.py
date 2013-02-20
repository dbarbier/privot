#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    # Poisson related functions
    # rPoisson
    lambdaMin = 0.2
    lambdaMax = 5.0
    n1 = 5
    nR = 5
    for i1 in range(n1):
        lambda_ = lambdaMin + (lambdaMax - lambdaMin) * i1 / (n1 - 1)
        for iR in range(nR):
            print "rPoisson(", lambda_, ")=", DistFunc.rPoisson(lambda_)
except :
    import sys
    print "t_DistFunc_poisson.py", sys.exc_type, sys.exc_value
