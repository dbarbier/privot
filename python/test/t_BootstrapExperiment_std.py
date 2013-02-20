#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    size = 5
    dim = 6
    refSample = NumericalSample(size, dim)
    for i in range(size):
        p = NumericalPoint(dim)
        for j in range(dim):
            p[j] = i + j
        refSample[i] = p
    print "ref. sample=", repr(refSample)
    myPlane = BootstrapExperiment(refSample)
    print "myPlane = ", myPlane
    weights = NumericalPoint(0)
    sample = myPlane.generate(weights)
    print "sample = ", repr(sample)
    print "weights = ", repr(weights)

except :
    import sys
    print "t_BootstrapExperiment_std.py", sys.exc_type, sys.exc_value
