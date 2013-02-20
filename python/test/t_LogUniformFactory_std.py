#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    distribution = LogUniform(1.0, 2.5)
    size = 10000
    sample = distribution.getSample(size)
    factory = LogUniformFactory
    covariance = CovarianceMatrix
    estimatedDistribution = factory.build(sample)
    print "Distribution          =", distribution
    print "Estimated distribution=", estimatedDistribution
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedLogUniform = factory.buildAsLogUniform(sample)
    print "LogUniform          =", distribution
    print "Estimated logUniform=", estimatedLogUniform
    estimatedLogUniform = factory.buildAsLogUniform()
    print "Default logUniform=", estimatedLogUniform
    estimatedLogUniform = factory.buildAsLogUniform(distribution.getParametersCollection())
    print "LogUniform from parameters=", estimatedLogUniform

except :
    import sys
    print "t_LogUniformFactory_std.py", sys.exc_type, sys.exc_value
