#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    # Instanciate one distribution object
    distribution = Rayleigh(2.5, -0.5)
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
    point = NumericalPoint( distribution.getDimension(), 1.0 )
    print "Point= ", repr(point)

    # Show PDF and CDF of point
    eps = 1e-5
    DDF = distribution.computeDDF( point )
    print "ddf     =", repr(DDF)
    print "ddf (FD)=", repr(NumericalPoint(1, (distribution.computePDF( point + NumericalPoint(1, eps) ) - distribution.computePDF( point  + NumericalPoint(1, -eps) )) / (2.0 * eps)))
    PDF = distribution.computePDF( point )
    print "pdf     =", PDF
    print "pdf (FD)= %.9f" % ( (distribution.computeCDF( point + NumericalPoint(1, eps) ) - distribution.computeCDF( point  + NumericalPoint(1, -eps) )) / (2.0 * eps), )
    CDF = distribution.computeCDF( point )
    print "cdf=", CDF
    CF = distribution.computeCharacteristicFunction( point[0] )
    print "characteristic function=", CF
    PDFgr = distribution.computePDFGradient( point )
    print "pdf gradient     =", repr(PDFgr)
    PDFgrFD = NumericalPoint(2)
    PDFgrFD[0] = (Rayleigh(distribution.getSigma() + eps, distribution.getGamma()).computePDF(point) -
                  Rayleigh(distribution.getSigma() - eps, distribution.getGamma()).computePDF(point)) / (2.0 * eps)
    PDFgrFD[1] = (Rayleigh(distribution.getSigma(), distribution.getGamma() + eps).computePDF(point) -
                  Rayleigh(distribution.getSigma(), distribution.getGamma() - eps).computePDF(point)) / (2.0 * eps)
    print "pdf gradient (FD)=", repr(PDFgrFD)
    CDFgr = distribution.computeCDFGradient( point )
    print "cdf gradient     =", repr(CDFgr)
    CDFgrFD = NumericalPoint(2)
    CDFgrFD[0] = (Rayleigh(distribution.getSigma() + eps, distribution.getGamma()).computeCDF(point) -
                  Rayleigh(distribution.getSigma() - eps, distribution.getGamma()).computeCDF(point)) / (2.0 * eps)
    CDFgrFD[1] = (Rayleigh(distribution.getSigma(), distribution.getGamma() + eps).computeCDF(point) -
                  Rayleigh(distribution.getSigma(), distribution.getGamma() - eps).computeCDF(point)) / (2.0 * eps)
    print "cdf gradient (FD)=", repr(CDFgrFD)
    quantile = distribution.computeQuantile( 0.95 )
    print "quantile=", repr(quantile)
    print "cdf(quantile)=", distribution.computeCDF(quantile)
    mean = distribution.getMean()
    print "mean=", repr(mean)
    covariance = distribution.getCovariance()
    print "covariance=", repr(covariance)
    parameters = distribution.getParametersCollection()
    print "parameters=", repr(parameters)
    for i in range(6):
        print "standard moment n=", i, " value=", distribution.getStandardMoment(i)
    print "Standard representative=", distribution.getStandardRepresentative()

    # Specific to this distribution
    gamma = distribution.getGamma()
    print "gamma=", gamma
    standardDeviation = distribution.getStandardDeviation()
    print "standard deviation=", repr(standardDeviation)
    skewness = distribution.getSkewness()
    print "skewness=", repr(skewness)
    kurtosis = distribution.getKurtosis()
    print "kurtosis=", repr(kurtosis)
    sigma = distribution.getSigma()
    print "sigma=", sigma

except :
    import sys
    print "t_Rayleigh_std.py", sys.exc_type, sys.exc_value
