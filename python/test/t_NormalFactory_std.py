#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    dim = 3
    mean = NumericalPoint(dim)
    sigma = NumericalPoint(dim)
    R = CorrelationMatrix(dim)
    for i in range(dim) :
        mean[i] = i + 0.5
        sigma[i] = 2 * i + 1.0
        for j in range(i) :
            R[i, j] = 0.5 * (1.0 + i) / dim
    distribution = Normal(mean, sigma, R)
    size = 10000
    sample = distribution.getSample(size)
    factory = NormalFactory()
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedNormal = factory.buildAsNormal(sample)
    print "Normal          =", distribution
    print "Estimated normal=", estimatedNormal
    estimatedNormal = factory.buildAsNormal()
    print "Default normal=", estimatedNormal
    estimatedNormal = factory.buildAsNormal(distribution.getParametersCollection())
    print "Normal from parameters=", estimatedNormal

except :
    import sys
    print "t_NormalFactory_std.py", sys.exc_type, sys.exc_value
