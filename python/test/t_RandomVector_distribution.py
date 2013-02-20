#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    # We create a distribution
    meanPoint = NumericalPoint(1)
    meanPoint[0] = 1.0
    sigma = NumericalPoint(1)
    sigma[0] = 1.0
    R = CorrelationMatrix(1)
    distribution = Normal(meanPoint, sigma, R)

    ref_distribution = distribution

    print "distribution = " , repr(ref_distribution)

    # We create a distribution-based RandomVector
    vect = RandomVector(UsualRandomVector(distribution))
    print "vect=" , vect

    # Check standard methods of class RandomVector
    print "vect dimension=" , vect.getDimension()
    print "vect realization (first )=" , repr(vect.getRealization())
    print "vect realization (second)=" , repr(vect.getRealization())
    print "vect realization (third )=" , repr(vect.getRealization())
    print "vect sample =" , repr(vect.getSample(5))

except :
    import sys
    print "t_RandomVector_distribution.py", sys.exc_type, sys.exc_value
