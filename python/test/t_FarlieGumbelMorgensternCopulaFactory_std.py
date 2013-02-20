#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    distribution = FarlieGumbelMorgensternCopula(0.7)
    size = 1000
    sample = distribution.getSample(size)
    factory = FarlieGumbelMorgensternCopulaFactory()
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedFarlieGumbelMorgensternCopula = factory.buildAsFarlieGumbelMorgensternCopula(sample)
    print "FarlieGumbelMorgensternCopula          =", distribution
    print "Estimated farlieGumbelMorgensternCopula=", estimatedFarlieGumbelMorgensternCopula
    estimatedFarlieGumbelMorgensternCopula = factory.buildAsFarlieGumbelMorgensternCopula()
    print "Default farlieGumbelMorgensternCopula=", estimatedFarlieGumbelMorgensternCopula
    estimatedFarlieGumbelMorgensternCopula = factory.buildAsFarlieGumbelMorgensternCopula(distribution.getParametersCollection())
    print "FarlieGumbelMorgensternCopula from parameters=", estimatedFarlieGumbelMorgensternCopula

except :
    import sys
    print "t_FarlieGumbelMorgensternFactory_std.py", sys.exc_type, sys.exc_value
