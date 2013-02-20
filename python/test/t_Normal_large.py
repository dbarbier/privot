#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    # Big test case for correlated components

    # Instanciate one distribution object
    dim = 4
    meanPoint = NumericalPoint(dim, 1.0)
    sigma = NumericalPoint(dim, 1.0)
    R =  CorrelationMatrix(dim)
    for i in range(1,dim) :
        R[i, i - 1] = 0.5

    distribution = Normal(meanPoint, sigma, R)

    # Test for sampling
    size = 1000
    oneSample = distribution.getSample( size )
    print "sample of size ",  size,  " first=",  repr(oneSample[0]),  " last=",  repr(oneSample[oneSample.getSize() - 1])
    mean = oneSample.computeMean()
    print "mean error=%.6f" % ((mean - meanPoint).norm() / meanPoint.norm())
    covariance = oneSample.computeCovariance()
    errorCovariance = 0.0
    for i in range(dim) :
        for j in range(dim) :
            errorCovariance += fabs(covariance[i, j] - sigma[i] * sigma[j] * R[i, j])
    print "covariance error=%.6f" % (errorCovariance / (dim * dim))

    # Define a point
    zero = NumericalPoint(dim, 0.0)

    # Show PDF and CDF of zero point
    zeroPDF = distribution.computePDF( zero )
    zeroCDF = distribution.computeCDF( zero )
    print "Zero point = ", repr(zero), " pdf=%.6f" % zeroPDF, repr(zero), " cdf=%.6f" % zeroCDF, " density generator=%.6f" % distribution.computeDensityGenerator(0.0)

    # Extract the marginals
    for i in range(dim) :
        margin = distribution.getMarginal(i)
        print "margin=", repr(margin)
        print "margin PDF=%.6f" % margin.computePDF( NumericalPoint(1))
        print "margin CDF=%.6f" % margin.computeCDF(NumericalPoint(1))
        print "margin quantile=", repr(margin.computeQuantile(0.5))
        print "margin realization=", repr(margin.getRealization())

    # Extract a 2-D marginal
    indices = Indices(2, 0)
    indices[0] = 1
    indices[1] = 0
    print "indices=", repr(indices)
    margins = distribution.getMarginal(indices)
    print "margins=", repr(margins)
    print "margins PDF=%.6f" % margins.computePDF(NumericalPoint(2))
    print "margins CDF=%.6f" % margins.computeCDF(NumericalPoint(2))
    quantile = NumericalPoint(margins.computeQuantile(0.5))
    print "margins quantile=", repr(quantile)
    print "margins CDF(qantile)=%.6f" % margins.computeCDF(quantile)
    print "margins realization=", repr(margins.getRealization())

    # Very big test case for independent components
    dim = 200
    meanPoint = NumericalPoint(dim, 0.1)
    sigma = NumericalPoint(dim, 1.0)
    distribution = Normal(meanPoint, sigma, IdentityMatrix(dim))
    print "Has independent copula? ", distribution.hasIndependentCopula()

    # Test for sampling
    oneSample = distribution.getSample( divmod(size, 10)[0] )
    print "sample of size ", size, " first=", repr(oneSample[0]), " last=" ,  repr(oneSample[oneSample.getSize() - 1])
    mean = oneSample.computeMean()
    print "mean error=%.6f" % ((mean - meanPoint).norm() / meanPoint.norm())
    covariance = oneSample.computeCovariance()
    errorCovariance = 0.0
    for i in range(dim) :
        for j in range(dim) :
            if (i==j) :
                temp = sigma[i] * sigma[j]
            else :
                temp = 0.0
        errorCovariance += fabs(covariance[i, j] - temp)
    print "covariance error=%.6f" % (errorCovariance / (dim * dim))

    # Define a point
    zero = NumericalPoint(dim, 0.0)

    # Show PDF and CDF of zero point
    zeroPDF = distribution.computePDF( zero )
    zeroCDF = distribution.computeCDF( zero )
    print "Zero point= " , repr(zero), " pdf=%.6f" % zeroPDF, " cdf=%.6f" % zeroCDF, " density generator=%.6f" % distribution.computeDensityGenerator(0.0)

except :
    import sys
    print "t_Normal_large.py", sys.exc_type, sys.exc_value
