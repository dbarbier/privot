#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    mu = NumericalPoint(4, 0.0)
    sigma = NumericalPoint(4, 1.0)
    a = NumericalPoint(4)
    b = NumericalPoint(4)
    a[0] = -4.0
    b[0] = 4.0
    a[1] = -1.0
    b[1] = 4.0
    a[2] = 1.0
    b[2] = 2.0
    a[3] = 3.0
    b[3] = 6.0

    PlatformInfo.SetNumericalPrecision(4)
    for i in range(4) :
        distribution = TruncatedNormal(mu[i], sigma[i], a[i], b[i])
        size = 10000
        sample = distribution.getSample(size)
        factory = TruncatedNormalFactory()
        estimatedDistribution = factory.build(sample)
        print "distribution=", repr(distribution)
        print "Estimated distribution=", repr(estimatedDistribution)
        estimatedDistribution = factory.build()
        print "Default distribution=", estimatedDistribution
        estimatedDistribution = factory.build(distribution.getParametersCollection())
        print "Distribution from parameters=", estimatedDistribution
        estimatedTruncatedNormal = factory.buildAsTruncatedNormal(sample)
        print "TruncatedNormal          =", distribution
        print "Estimated TruncatedNormal=", estimatedTruncatedNormal
        estimatedTruncatedNormal = factory.buildAsTruncatedNormal()
        print "Default TruncatedNormal=", estimatedTruncatedNormal
        estimatedTruncatedNormal = factory.buildAsTruncatedNormal(distribution.getParametersCollection())
        print "TruncatedNormal from parameters=", estimatedTruncatedNormal
        
except :
    import sys
    print "t_TruncatedNormalFactory_std.py", sys.exc_type, sys.exc_value
