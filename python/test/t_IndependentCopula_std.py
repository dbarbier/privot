#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    # Instanciate one distribution object
    dim = 3

    copula = IndependentCopula(dim)
    print "Copula =", repr(copula)
    print "Copula =", copula

    # Is this copula an elliptical distribution?
    print "Elliptical distribution = ", copula.isElliptical()

    # Is this copula continuous ?
    print "Continuous copula = ",  copula.isContinuous()

    # Is this copula elliptical ?
    print "Elliptical copula = ", copula.hasEllipticalCopula()

    # Is this copula independant ?
    print "hasIndependentCopula = ",copula.hasIndependentCopula()

    # Test for realization of copula
    oneRealization = copula.getRealization()
    print "oneRealization=", repr(oneRealization)

    # Test for sampling
    size = 10000
    oneSample = copula.getSample( size )
    print "oneSample first=" , repr(oneSample[0]) , " last=" , repr(oneSample[size - 1])
    print "mean=" , repr(oneSample.computeMean())
    print "covariance=" , repr(oneSample.computeCovariance())

    # Define a point
    point = NumericalPoint(copula.getDimension(), 0.6)
    print "Point= " , repr(point)

    # Show PDF and CDF of point

    # derivative of PDF with regards its arguments
    DDF = copula.computeDDF( point )
    print "ddf     =" , repr(DDF)

    # PDF value
    PDF = copula.computePDF( point )
    print "pdf     =%.6f" % PDF

    # CDF value
    CDF = copula.computeCDF( point )
    print "cdf=%.6f" % CDF

    # derivative of the PDF with regards the parameters of the distribution
    PDFgr = copula.computePDFGradient( point )
    print "pdf gradient     =" , repr(PDFgr)

    # derivative of the CDF with regards the parameters of the distribution
    CDFgr = copula.computeCDFGradient( point )
    print "cdf gradient     =" , repr(CDFgr)

    # quantile
    quantile = copula.computeQuantile( 0.95 )
    print "quantile=" , repr(quantile)
    print "cdf(quantile)=%.6f" % copula.computeCDF(quantile)

    # mean
    mean = copula.getMean()
    print "mean=" , repr(mean)

    # covariance
    covariance = copula.getCovariance()
    print "covariance=" , repr(covariance)

    # parameters of the distribution
    parameters = copula.getParametersCollection()
    print "parameters=" , repr(parameters)

    # Specific to this copula

    # Extract the marginals
    for i in range(dim) :
        margin = copula.getMarginal(i)
        print "margin=", repr(margin)
        print "margin PDF=%.6f" % margin.computePDF(NumericalPoint(1,0.25))
        print "margin CDF=%.6f" % margin.computeCDF(NumericalPoint(1,0.25))
        print "margin quantile=", repr(margin.computeQuantile(0.95))
        print "margin realization=", repr(margin.getRealization())

    # Extract a 2-D marginal
    indices = Indices(2, 0)
    indices[0] = 1
    indices[1] = 0
    print "indices=", repr(indices)
    margins = copula.getMarginal(indices)
    print "margins=", repr(margins)
    print "margins PDF=%.6f" % margins.computePDF(NumericalPoint(2,0.25))
    print "margins CDF=%.6f" % margins.computeCDF(NumericalPoint(2,0.25))
    quantile = margins.computeQuantile(0.95)
    print "margins quantile=", repr(quantile)
    print "margins CDF(quantile)=%.6f" % margins.computeCDF(quantile)
    print "margins realization=", repr(margins.getRealization())

except :
    import sys
    print "t_IndependantCopula.py", sys.exc_type, sys.exc_value
