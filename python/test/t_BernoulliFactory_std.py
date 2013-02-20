#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    distribution = Bernoulli(0.7)
    size = 10000
    sample = distribution.getSample(size)
    factory = BernoulliFactory()
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedBernoulli = factory.buildAsBernoulli(sample)
    print "Bernoulli          =", distribution
    print "Estimated Bernoulli=", estimatedBernoulli
    estimatedBernoulli = factory.buildAsBernoulli()
    print "Default Bernoulli=", estimatedBernoulli
    estimatedBernoulli = factory.buildAsBernoulli(distribution.getParametersCollection())
    print "Bernoulli from parameters=", estimatedBernoulli

except :
    import sys
    print "t_BernoulliFactory_std.py", sys.exc_type, sys.exc_value
