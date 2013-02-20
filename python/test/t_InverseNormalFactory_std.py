#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    distribution = InverseNormal(2.5, 1.5)
    size = 10000
    sample = distribution.getSample(size)
    factory = InverseNormalFactory()
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedInverseNormal = factory.buildAsInverseNormal(sample)
    print "InverseNormal          =", distribution
    print "Estimated inverseNormal=", estimatedInverseNormal
    estimatedInverseNormal = factory.buildAsInverseNormal()
    print "Default inverseNormal=", estimatedInverseNormal
    estimatedInverseNormal = factory.buildAsInverseNormal(distribution.getParametersCollection())
    print "InverseNormal from parameters=", estimatedInverseNormal

except :
    import sys
    print "t_InverseNormalFactory_std.py", sys.exc_type, sys.exc_value
