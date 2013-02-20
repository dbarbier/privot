#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    distribution = LogNormal(1.5, 2.5, -1.5)
    size = 10000
    sample = distribution.getSample(size)
    factory = LogNormalFactory()
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedLogNormal = factory.buildAsLogNormal(sample)
    print "LogNormal          =", distribution
    print "Estimated logNormal=", estimatedLogNormal
    estimatedLogNormal = factory.buildAsLogNormal()
    print "Default logNormal=", estimatedLogNormal
    estimatedLogNormal = factory.buildAsLogNormal(distribution.getParametersCollection())
    print "LogNormal from parameters=", estimatedLogNormal

except :
    import sys
    print "t_LogNormalFactory_std.py", sys.exc_type, sys.exc_value
