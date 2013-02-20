#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    sample = NumericalSample(3, 2)
    sample[0] = NumericalPoint( (1.0, 1.5) )
    sample[1] = NumericalPoint( (2.0, 2.5) )
    sample[2] = NumericalPoint( (3.0, 3.5) )

    factory = UserDefinedFactory()
    estimatedDistribution = factory.build(sample)
    print "sample=", repr(sample)
    print "Estimated distribution=", repr(estimatedDistribution)
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedUserDefined = factory.buildAsUserDefined(sample)
    print "Sample                =", sample
    print "Estimated UserDefined=", estimatedUserDefined
    estimatedUserDefined = factory.buildAsUserDefined()
    print "Default UserDefined=", estimatedUserDefined

except :
    import sys
    print "t_UserDefinedFactory_std.py", sys.exc_type, sys.exc_value
