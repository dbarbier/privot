#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    distribution = Weibull(1.0, 2.5, -1.0)
    size = 10000
    sample = distribution.getSample(size)
    factory = WeibullFactory()
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedWeibull = factory.buildAsWeibull(sample)
    print "Estimated weibull=", estimatedWeibull
    estimatedWeibull = factory.buildAsWeibull()
    print "Default weibull=", estimatedWeibull
    estimatedWeibull = factory.buildAsWeibull(distribution.getParametersCollection())
    print "Weibull from parameters=", estimatedWeibull

except :
    import sys
    print "t_WeibullFactory_std.py", sys.exc_type, sys.exc_value
