#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    distribution = NegativeBinomial(15, 0.7)
    size = 10000
    sample = distribution.getSample(size)
    factory = NegativeBinomialFactory()
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedNegativeBinomial = factory.buildAsNegativeBinomial(sample)
    print "NegativeBinomial          =", distribution
    print "Estimated negativeBinomial=", estimatedNegativeBinomial
    estimatedNegativeBinomial = factory.buildAsNegativeBinomial()
    print "Default negativeBinomial=", estimatedNegativeBinomial
    estimatedNegativeBinomial = factory.buildAsNegativeBinomial(distribution.getParametersCollection())
    print "NegativeBinomial from parameters=", estimatedNegativeBinomial

except :
    import sys
    print "t_NegativeBinomialFactory_std.py", sys.exc_type, sys.exc_value
