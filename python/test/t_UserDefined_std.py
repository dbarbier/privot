#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    # Instanciate one distribution object
    collection = UserDefinedPairCollection(3, UserDefinedPair(NumericalPoint(1), 0.0))
    point = NumericalPoint(1)

    point[0] = 1.0
    collection[0] = UserDefinedPair(point, 0.30)

    point[0] = 2.0
    collection[1] = UserDefinedPair(point, 0.10)

    point[0] = 3.0
    collection[2] = UserDefinedPair(point, 0.60)
    distribution = UserDefined(collection)
    print  "Distribution " , repr(distribution)
    print  "Distribution " , distribution

    # Is this distribution elliptical ?
    print "Elliptical = ", distribution.isElliptical()

    # Is this distribution continuous ?
    print "Continuous = ", distribution.isContinuous()

    # Test for realization of distribution
    oneRealization = distribution.getRealization()
    print "oneRealization=", repr(oneRealization)

    # Test for sampling
    size = 10
    oneSample = distribution.getSample( size )
    print "oneSample=Ok", repr(oneSample)

    # Define a point
    point = NumericalPoint( distribution.getDimension(), 2.0 )

    # Show PDF and CDF of a point
    pointPDF = distribution.computePDF( point )
    pointCDF = distribution.computeCDF( point )
    print "point= ", repr(point), " pdf=", pointPDF, " cdf=", pointCDF

    # Get 95% quantile
    quantile = distribution.computeQuantile( 0.95 )
    print "Quantile=", repr(quantile)

    for i in range(6):
        print "standard moment n=", i, " value=", distribution.getStandardMoment(i)
    print "Standard representative=", distribution.getStandardRepresentative()

    sample = NumericalSample(4, 3)
    for i in range(4):
        for j in range(3):
            sample[i, j] = 10 * (i + 1) + 0.1 * (j + 1)

    multivariateUserDefined = UserDefined(sample)
    print "Multivariate UserDefined=", multivariateUserDefined
    print "Marginal 0=", multivariateUserDefined.getMarginal(0)
    indices = Indices(2)
    indices[0] = 2
    indices[1] = 0
    print "Marginal (2, 0)=", multivariateUserDefined.getMarginal(indices)

except :
    import sys
    print "t_UserDefined_std.py", sys.exc_type, sys.exc_value
