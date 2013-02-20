#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    distribution = GumbelCopula(1.5)
    size = 1000
    sample = distribution.getSample(size)
    factory = GumbelCopulaFactory()
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedGumbelCopula = factory.buildAsGumbelCopula(sample)
    print "GumbelCopula          =", distribution
    print "Estimated gumbelCopula=", estimatedGumbelCopula
    estimatedGumbelCopula = factory.buildAsGumbelCopula()
    print "Default gumbelCopula=", estimatedGumbelCopula
    estimatedGumbelCopula = factory.buildAsGumbelCopula(distribution.getParametersCollection())
    print "GumbelCopula from parameters=", estimatedGumbelCopula

except :
    import sys
    print "t_GumbelCopulaFactory_std.py", sys.exc_type, sys.exc_value
