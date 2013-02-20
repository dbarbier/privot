#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    distribution = Geometric(0.7)
    size = 10000
    sample = distribution.getSample(size)
    factory = GeometricFactory()
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedGeometric = factory.buildAsGeometric(sample)
    print "Geometric          =", distribution
    print "Estimated geometric=", estimatedGeometric
    estimatedGeometric = factory.buildAsGeometric()
    print "Default geometric=", estimatedGeometric
    estimatedGeometric = factory.buildAsGeometric(distribution.getParametersCollection())
    print "Geometric from parameters=", estimatedGeometric

except :
    import sys
    print "t_GeometricFactory_std.py", sys.exc_type, sys.exc_value
