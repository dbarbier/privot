#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    distribution = Normal(4)
    distribution.setMean(NumericalPoint(4, 5.0))
    size = 10
    myPlane = LowDiscrepancyExperiment(HaltonSequence(), distribution, size)
    print "myPlane = ", myPlane
    weights = NumericalPoint(0)
    sample = NumericalSample(myPlane.generate(weights))
    print "sample = ", repr(sample)
    print "weights = ", repr(weights)
except :
    import sys
    print "t_LowDiscrepancyExperiment_std.py", sys.exc_type, sys.exc_value
