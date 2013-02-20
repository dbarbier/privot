#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    dim = 3

    copula = MinCopula(dim)
    copula.setName("a min copula")
    print "Copula ", repr(copula)
    print "Copula ", copula
    print "Mean ", copula.getMean()
    print "Covariance ", copula.getCovariance()
    # Is this copula an elliptical distribution?
    print "Elliptical distribution= ", copula.isElliptical()

    # Is this copula elliptical ?
    print "Elliptical copula= ", copula.hasEllipticalCopula()

    # Is this copula independent ?
    print "Independent copula= ", copula.hasIndependentCopula()

    # Test for realization of copula
    oneRealization = copula.getRealization()
    print "oneRealization=", oneRealization

    # Test for sampling
    size = 10
    oneSample = copula.getSample( size )
    print "oneSample=", oneSample

    # Test for sampling
    size = 10000
    anotherSample = copula.getSample( size )
    print "anotherSample mean=", anotherSample.computeMean()
    print "anotherSample covariance=", anotherSample.computeCovariance()

    # Define a point
    point = NumericalPoint(dim, 0.2)

    # Show PDF and CDF of zero point
    zeroPDF = copula.computePDF( point )
    zeroCDF = copula.computeCDF( point )
    print "point= ", point, "pdf=", zeroPDF, "cdf=", zeroCDF

    # Get 50% quantile
    quantile = copula.computeQuantile( 0.5 )
    print "Quantile=", quantile
    print "CDF(quantile)=", copula.computeCDF(quantile)
    # Extract the marginals
    for i in range(dim):
        margin = copula.getMarginal(i)
        print "margin=", margin
        print "margin PDF=", margin.computePDF(NumericalPoint(1,0.25))
        print "margin CDF=", margin.computeCDF(NumericalPoint(1,0.25))
        print "margin quantile=", margin.computeQuantile(0.95)
        print "margin realization=", margin.getRealization()

    # Extract a 2-D marginal
    indices = Indices(2, 0)
    indices[0] = 1
    indices[1] = 0
    print "indices=", indices
    margins = copula.getMarginal(indices)
    print "margins=", margins
    print "margins PDF=", margins.computePDF(NumericalPoint(2, 0.25))
    print "margins CDF=", margins.computeCDF(NumericalPoint(2, 0.25))
    quantile = margins.computeQuantile(0.95)
    print "margins quantile=", quantile
    print "margins CDF(quantile)=", margins.computeCDF(quantile)
    print "margins realization=", margins.getRealization()

except :
    import sys
    print "t_MinCopula.py", sys.exc_type, sys.exc_value
