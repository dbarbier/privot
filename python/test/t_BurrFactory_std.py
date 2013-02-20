#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    distribution = Burr(2.5, 1.5)
    size = 10000
    sample = distribution.getSample(size)
    factory = BurrFactory()
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedBurr = factory.buildAsBurr(sample)
    print "Burr          =", distribution
    print "Estimated burr=", estimatedBurr
    estimatedBurr = factory.buildAsBurr()
    print "Default burr=", estimatedBurr
    estimatedBurr = factory.buildAsBurr(distribution.getParametersCollection())
    print "Burr from parameters=", estimatedBurr

except :
    import sys
    print "t_BurrFactory_std.py", sys.exc_type, sys.exc_value
