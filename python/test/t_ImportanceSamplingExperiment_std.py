#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    distribution = Normal(4)
    size = 10
    weightingDistribution = Normal(distribution)
    weightingDistribution.setMean(NumericalPoint(4, 1.0))
    myPlane = ImportanceSamplingExperiment(distribution, weightingDistribution, size)
    print "myPlane = ", myPlane
    weights = NumericalPoint(0)
    sample = NumericalSample(myPlane.generate(weights))
    print "sample = ", repr(sample)
    print "weights = ", repr(weights)
except :
    import sys
    print "t_ImportanceSamplingExperiment_std.py", sys.exc_type, sys.exc_value
