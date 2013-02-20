#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    distribution = Laplace(2.5, -1.3)
    size = 10000
    sample = distribution.getSample(size)
    factory = LaplaceFactory()
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedLaplace = factory.buildAsLaplace(sample)
    print "Laplace          =", distribution
    print "Estimated laplace=", estimatedLaplace
    estimatedLaplace = factory.buildAsLaplace()
    print "Default laplace=", estimatedLaplace
    estimatedLaplace = factory.buildAsLaplace(distribution.getParametersCollection())
    print "Laplace from parameters=", estimatedLaplace

except :
    import sys
    print "t_LaplaceFactory_std.py", sys.exc_type, sys.exc_value
