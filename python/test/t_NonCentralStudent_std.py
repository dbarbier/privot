#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    # Instanciate one distribution object
    distribution = NonCentralStudent(7.2, 4.8, -3.7)
    print "Distribution " , repr(distribution)
    print "Distribution " , distribution

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
    print "oneSample first=" , repr(oneSample[0]) , " last=" , repr(oneSample[size - 1])
    print "mean=" , repr(oneSample.computeMean())
    print "covariance=" , repr(oneSample.computeCovariance())

    # Define a point
    point = NumericalPoint( distribution.getDimension() , 1.0)
    print "Point= " , repr(point)

    # Show PDF and CDF of point
    eps = 1e-5

    # derivative of PDF with regards its arguments
    DDF = distribution.computeDDF( point )
    print "ddf     =" , repr(DDF)
    # by the finite difference technique
    print "ddf (FD)=" , repr(NumericalPoint(1, (distribution.computePDF( point + NumericalPoint(1, eps) ) - distribution.computePDF( point  + NumericalPoint(1, -eps) )) / (2.0 * eps)))

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
    PDFgrFD = NumericalPoint(3)
    PDFgrFD[0] = (NonCentralStudent(distribution.getNu() + eps, distribution.getDelta(), distribution.getGamma()).computePDF(point) -
                  NonCentralStudent(distribution.getNu() - eps, distribution.getDelta(), distribution.getGamma()).computePDF(point)) / (2.0 * eps)
    PDFgrFD[1] = (NonCentralStudent(distribution.getNu(), distribution.getDelta() + eps, distribution.getGamma()).computePDF(point) -
                  NonCentralStudent(distribution.getNu(), distribution.getDelta() - eps, distribution.getGamma()).computePDF(point)) / (2.0 * eps)
    PDFgrFD[2] = (NonCentralStudent(distribution.getNu(), distribution.getDelta(), distribution.getGamma() + eps).computePDF(point) -
                  NonCentralStudent(distribution.getNu(), distribution.getDelta(), distribution.getGamma() - eps).computePDF(point)) / (2.0 * eps)
    print "pdf gradient (FD)=" , repr(PDFgrFD)

    # derivative of the PDF with regards the parameters of the distribution
    CDFgr = distribution.computeCDFGradient( point )
    print "cdf gradient     =" , repr(CDFgr)
    CDFgrFD = NumericalPoint(3)
    CDFgrFD[0] = (NonCentralStudent(distribution.getNu() + eps, distribution.getDelta(), distribution.getGamma()).computeCDF(point) -
                  NonCentralStudent(distribution.getNu() - eps, distribution.getDelta(), distribution.getGamma()).computeCDF(point)) / (2.0 * eps)
    CDFgrFD[1] = (NonCentralStudent(distribution.getNu(), distribution.getDelta() + eps, distribution.getGamma()).computeCDF(point) -
                  NonCentralStudent(distribution.getNu(), distribution.getDelta() - eps, distribution.getGamma()).computeCDF(point)) / (2.0 * eps)
    CDFgrFD[2] = (NonCentralStudent(distribution.getNu(), distribution.getDelta(), distribution.getGamma() + eps).computeCDF(point) -
                  NonCentralStudent(distribution.getNu(), distribution.getDelta(), distribution.getGamma() - eps).computeCDF(point)) / (2.0 * eps)
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

except :
    import sys
    print "t_NonCentralStudent_std.py", sys.exc_type, sys.exc_value
