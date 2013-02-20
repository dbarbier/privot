#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    # Instanciate one distribution object
    distribution = Arcsine(5.2, 11.6)
    print "Distribution ", repr(distribution)
    print "Distribution ", distribution

    # Get mean and covariance
    print "Mean= ", repr(distribution.getMean())
    print "Covariance= ", repr(distribution.getCovariance())

    # Is this distribution elliptical ?
    print "Elliptical = ", distribution.isElliptical()

    # Test for realization of distribution
    oneRealization = distribution.getRealization()
    print "oneRealization=", repr(oneRealization)

    # Test for sampling
    size = 10000
    oneSample = distribution.getSample( size )
    print "oneSample first=" , repr(oneSample[0]) , " last=" , repr(oneSample[size - 1])
    print "mean=" , repr(oneSample.computeMean())
    print "covariance=" , repr(oneSample.computeCovariance())

    # Define a point
    point = NumericalPoint( distribution.getDimension() , 9.1)
    print "Point= " , repr(point)

    # Show PDF and CDF of point
    eps = 1e-5
    max=distribution.getB()+distribution.getA()
    min=distribution.getB()-distribution.getA()
    # derivative of PDF with regards its arguments
    DDF = distribution.computeDDF( point )
    print "ddf     =", repr(DDF)
    # by the finite difference technique
    print "ddf (FD)=", repr(NumericalPoint(1, (distribution.computePDF( point + NumericalPoint(1, eps) ) - distribution.computePDF( point  + NumericalPoint(1, -eps) )) / (2.0 * eps)))

    # PDF value
    LPDF = distribution.computeLogPDF( point )
    print "log pdf=%.6f" % LPDF
    PDF = distribution.computePDF( point )
    print "pdf     =%.6f" % PDF
    # by the finite difference technique from CDF
    print "pdf (FD)=%.6f" % ((distribution.computeCDF( point + NumericalPoint(1, eps) ) - distribution.computeCDF( point  + NumericalPoint(1, -eps) )) / (2.0 * eps))

    # derivative of the PDF with regards the parameters of the distribution
    CDF = distribution.computeCDF( point )
    print "cdf=%.6f" % CDF
    CCDF = distribution.computeComplementaryCDF( point )
    print "ccdf=%.6f" % CCDF
    PDFgr = distribution.computePDFGradient( point )
    print "pdf gradient     =" , repr(PDFgr)
    # by the finite difference technique
    PDFgrFD = NumericalPoint(2)
    PDFgrFD[0] = (Arcsine(distribution.getA() + eps, distribution.getB()).computePDF(point) -
                  Arcsine(distribution.getA() - eps, distribution.getB()).computePDF(point)) / (2.0 * eps)
    PDFgrFD[1] = (Arcsine(distribution.getA(), distribution.getB() + eps).computePDF(point) -
                  Arcsine(distribution.getA(), distribution.getB() - eps).computePDF(point)) / (2.0 * eps)
    print "pdf gradient (FD)=" , repr(PDFgrFD)

    # derivative of the PDF with regards the parameters of the distribution
    CDFgr = distribution.computeCDFGradient( point )
    print "cdf gradient     =" , repr(CDFgr)
    CDFgrFD = NumericalPoint(2)
    CDFgrFD[0] = (Arcsine(distribution.getA() + eps, distribution.getB()).computeCDF(point) -
                  Arcsine(distribution.getA() - eps, distribution.getB()).computeCDF(point)) / (2.0 * eps)
    CDFgrFD[1] = (Arcsine(distribution.getA(), distribution.getB() + eps).computeCDF(point) -
                  Arcsine(distribution.getA(), distribution.getB() - eps).computeCDF(point)) / (2.0 * eps)
    print "cdf gradient (FD)=",  repr(CDFgrFD)

    # quantile
    quantile = distribution.computeQuantile( 0.95 )
    print "quantile=" , repr(quantile)
    print "cdf(quantile)=%.6f" % distribution.computeCDF(quantile)
    mean = distribution.getMean()
    print "mean=" , repr(mean)
    standardDeviation = distribution.getStandardDeviation()
    print "standard deviation=" , repr(standardDeviation)
    skewness = distribution.getSkewness()
    print "skewness=" , repr(skewness)
    kurtosis = distribution.getKurtosis()
    print "kurtosis=" , repr(kurtosis)
    covariance = distribution.getCovariance()
    print "covariance=" , repr(covariance)
    parameters = distribution.getParametersCollection()
    print "parameters=" , repr(parameters)
    for i in range(6):
        print "standard moment n=", i, " value=", distribution.getStandardMoment(i)
    print "Standard representative=", distribution.getStandardRepresentative()

    # Specific to this distribution
    mu = distribution.getMu()
    print "mu=", mu
    sigma = distribution.getSigma()
    print "sigma=",sigma
    newDistribution = Arcsine(mu, sigma, Arcsine.MUSIGMA)
    print "a from (mu, sigma)=",newDistribution.getA()
    print "b from (mu, sigma)=",newDistribution.getB()
except :
    import sys
    print "t_Arcsine_std.py", sys.exc_type, sys.exc_value
