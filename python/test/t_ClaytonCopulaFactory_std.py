#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    distribution = ClaytonCopula(1.5)
    size = 1000
    sample = distribution.getSample(size)
    factory = ClaytonCopulaFactory()
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedClaytonCopula = factory.buildAsClaytonCopula(sample)
    print "ClaytonCopula          =", distribution
    print "Estimated claytonCopula=", estimatedClaytonCopula
    estimatedClaytonCopula = factory.buildAsClaytonCopula()
    print "Default claytonCopula=", estimatedClaytonCopula
    estimatedClaytonCopula = factory.buildAsClaytonCopula(distribution.getParametersCollection())
    print "ClaytonCopula from parameters=", estimatedClaytonCopula

except :
    import sys
    print "t_ClaytonCopulaFactory_std.py", sys.exc_type, sys.exc_value
