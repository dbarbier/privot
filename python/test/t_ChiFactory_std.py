#! /usr/bin/env python
# -*- coding: iso-8859-15 -*-

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    distribution = Chi(0.5)
    size = 10000
    sample = distribution.getSample(size)
    factory = ChiFactory()
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    distribution = Chi(1.0)
    sample = distribution.getSample(size)
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    distribution = Chi(2.5)
    sample = distribution.getSample(size)
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    print "Estimated distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedChi = factory.buildAsChi(sample)
    print "Chi          =", distribution
    print "Estimated chi=", estimatedChi
    estimatedChi = factory.buildAsChi()
    print "Default chi=", estimatedChi
    estimatedChi = factory.buildAsChi(distribution.getParametersCollection())
    print "Chi from parameters=", estimatedChi

except :
    import sys
    print "t_ChiFactory_std.py", sys.exc_type, sys.exc_value
