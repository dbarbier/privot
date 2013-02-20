#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    distribution = Trapezoidal(1.0, 2.3, 4.5, 5.0)
    size = 10000
    sample = distribution.getSample(size)
    factory = TrapezoidalFactory()
    estimatedDistribution = factory.build(sample)
    print "distribution=", repr(distribution)
    oldPrecision = PlatformInfo.GetNumericalPrecision()
    PlatformInfo.SetNumericalPrecision(4)
    print "Estimated distribution=", repr(estimatedDistribution)
    PlatformInfo.SetNumericalPrecision(oldPrecision)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedTrapezoidal = factory.buildAsTrapezoidal(sample)
    print "Trapezoidal          =", distribution
    oldPrecision = PlatformInfo.GetNumericalPrecision()
    PlatformInfo.SetNumericalPrecision( 4 )
    print "Estimated trapezoidal=", estimatedTrapezoidal
    PlatformInfo.SetNumericalPrecision( oldPrecision )
    estimatedTrapezoidal = factory.buildAsTrapezoidal()
    print "Default trapezoidal=", estimatedTrapezoidal
    estimatedTrapezoidal = factory.buildAsTrapezoidal(distribution.getParametersCollection())
    print "Trapezoidal from parameters=", estimatedTrapezoidal
except :
    import sys
    print "t_TrapezoidalFactory_std.py", sys.exc_type, sys.exc_value
