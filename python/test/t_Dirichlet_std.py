#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    # Instanciate one distribution object
    for dim in range(1, 5):
        theta = NumericalPoint(dim + 1)
        for i in range(dim + 1):
            theta[i] = (i + 1.0) / 4.0
        distribution = Dirichlet(theta)
        description = Description(dim)
        for j in range(1, dim + 1):
            oss = "Marginal " + str(j)
            description[j-1] = oss
        distribution.setDescription(description)
        print "Distribution ", repr(distribution)
        print "Distribution ", distribution

        # Is this distribution elliptical ?
        print "Elliptical = ", distribution.isElliptical()

        # Is this distribution continuous ?
        print "Continuous = ", distribution.isContinuous()

        # Test for realization of distribution
        oneRealization = distribution.getRealization()
        print "oneRealization=", repr(oneRealization)

        # Test for sampling
        size = 10000
        oneSample = distribution.getSample( size )
        print "oneSample first=", repr(oneSample[0]), " last=", repr(oneSample[size - 1])
        print "mean=", repr(oneSample.computeMean())
        print "covariance=", repr(oneSample.computeCovariance())
        # Define a point
        point = NumericalPoint( distribution.getDimension(), 0.5 / distribution.getDimension() )
        print "Point= ", repr(point)

        # Show PDF and CDF of point
        LPDF = distribution.computeLogPDF( point )
        print "log pdf=", LPDF
        PDF = distribution.computePDF( point )
        print "pdf     =", PDF
        CDF = distribution.computeCDF( point )
        print "cdf=", CDF
        quantile = distribution.computeQuantile( 0.95 )
        print "quantile=", repr(quantile)
        print "cdf(quantile)= %.6f" % distribution.computeCDF(quantile)
        mean = distribution.getMean()
        print "mean=", repr(mean)
        standardDeviation = distribution.getStandardDeviation()
        print "standard deviation=", repr(standardDeviation)
        skewness = distribution.getSkewness()
        print "skewness=", repr(skewness)
        kurtosis = distribution.getKurtosis()
        print "kurtosis=", repr(kurtosis)
        covariance = distribution.getCovariance()
        print "covariance=", repr(covariance)
        # Extract the marginals
        for i in range(dim):
            margin = distribution.getMarginal(i)
            print "margin=", margin
            print "margin PDF=", margin.computePDF(NumericalPoint(1, 0.5))
            print "margin CDF=", margin.computeCDF(NumericalPoint(1, 0.5))
            print "margin quantile=", repr(margin.computeQuantile(0.95))
            print "margin realization=", repr(margin.getRealization())
        if (dim >= 2):
            # Extract a 2-D marginal
            indices = Indices(2, 0)
            indices[0] = 1
            indices[1] = 0
            print "indices=", indices
            margins = distribution.getMarginal(indices)
            print "margins=", margins
            print "margins PDF=", margins.computePDF(NumericalPoint(2, 0.5))
            print "margins CDF=", margins.computeCDF(NumericalPoint(2, 0.5))
            quantile = margins.computeQuantile(0.95)
            print "margins quantile=", repr(quantile)
            print "margins CDF(quantile)= %.6f" % margins.computeCDF(quantile)
            print "margins realization=", repr(margins.getRealization())

except :
    import sys
    print "t_Dirichlet_std.py", sys.exc_type, sys.exc_value
