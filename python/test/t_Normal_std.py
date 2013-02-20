#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

def cleanScalar(inScalar) :
    if (fabs(inScalar) < 1.e-10) :
        inScalar = 0.0
    return inScalar

def cleanNumericalPoint(inNumericalPoint) :
    dim = inNumericalPoint.getDimension()
    for i in range(dim) :
        if (fabs(inNumericalPoint[i]) < 1.e-10) :
            inNumericalPoint[i] = 0.0
    return inNumericalPoint


def cleanMatrix(inMatrix) :
    rowDim = inMatrix.getNbRows()
    colDim = inMatrix.getNbColumns()
    for i in range(rowDim) :
        for j in range(colDim) :
            if (fabs(inMatrix[i, j]) < 1.e-10) :
                inMatrix[i, j] = 0.0
    return inMatrix


try :
    PlatformInfo.SetNumericalPrecision(5)
    # Instanciate one distribution object
    dim = 3
    meanPoint = NumericalPoint(dim, 1.0)
    meanPoint[0] = 0.5
    meanPoint[1] = -0.5
    sigma = NumericalPoint(dim, 1.0)
    sigma[0] = 2.0
    sigma[1] = 3.0
    R = CorrelationMatrix(dim)
    for i in range(1,dim) :
        R[i, i - 1] = 0.5

    distribution = Normal(meanPoint, sigma, R)

    distribution.setName("A normal distribution")
    description = Description(dim)
    description[0] = "Marginal 1"
    description[1] = "Marginal 2"
    description[2] = "Marginal 3"
    distribution.setDescription(description)

    print "Parameters collection=", repr(distribution.getParametersCollection())
    for i in range(6):
        print "standard moment n=", i, " value=", distribution.getStandardMoment(i)
    print "Standard representative=", distribution.getStandardRepresentative()

    print "Distribution ", repr(distribution)
    print "Distribution ", distribution
    print "Covariance ", repr(distribution.getCovariance())

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
    point = NumericalPoint( distribution.getDimension(), 0.5 )
    print "Point= " , repr(point)

    # Show PDF and CDF of point
    eps = 1e-5

    # derivative of PDF with respect to its arguments
    DDF = distribution.computeDDF( point )
    print "ddf     =" , repr(cleanNumericalPoint(DDF))
    # by the finite difference technique
    ddfFD = NumericalPoint(dim)
    for i in range(dim) :
        pointEps = point
        pointEps[i] += eps
        ddfFD[i] = distribution.computePDF(pointEps)
        pointEps[i] -= 2.0 * eps
        ddfFD[i] -= distribution.computePDF(pointEps)
        ddfFD[i] /= 2.0 * eps
    print "ddf (FD)=" , repr(cleanNumericalPoint(ddfFD))
    # PDF value
    LPDF = distribution.computeLogPDF( point )
    print "log pdf=%.6f" % LPDF
    PDF = distribution.computePDF( point )
    print "pdf     =%.6f" % PDF
    # by the finite difference technique from CDF
    if dim == 1 :
        print "pdf (FD)=%.6f" % cleanScalar((distribution.computeCDF( point + NumericalPoint(1, eps) ) - distribution.computeCDF( point  + NumericalPoint(1, -eps) )) / (2.0 * eps))
    CDF = distribution.computeCDF( point )
    print "cdf=%.6f" % CDF
    PDFgr = distribution.computePDFGradient( point )
    print "pdf gradient     =" , repr(PDFgr)
    # by the finite difference technique
    PDFgrFD = NumericalPoint(2 * dim)
    for i in range(dim) :
        meanPoint[i] += eps
        distributionLeft = Normal(meanPoint, sigma, R)
        meanPoint[i] -= 2.0 * eps
        distributionRight = Normal(meanPoint, sigma, R)
        PDFgrFD[i] = (distributionLeft.computePDF(point) - distributionRight.computePDF(point)) / (2.0 * eps)
        meanPoint[i] += eps
    for i in range(dim) :
        sigma[i] += eps
        distributionLeft = Normal(meanPoint, sigma, R)
        sigma[i] -= 2.0 * eps
        distributionRight = Normal(meanPoint, sigma, R)
        PDFgrFD[dim + i] = (distributionLeft.computePDF(point) - distributionRight.computePDF(point)) / (2.0 * eps)
        sigma[i] += eps
    print "pdf gradient (FD)=" , repr(cleanNumericalPoint(PDFgrFD))

    # derivative of the PDF with regards the parameters of the distribution
    #   CDFgr = distribution.computeCDFGradient( point )
    #     print "cdf gradient     =" , CDFgr

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

    # Specific to this distribution
    beta = point.norm2()
    densityGenerator = distribution.computeDensityGenerator(beta)
    print "density generator=%.6f" % densityGenerator

    print "pdf via density generator=%.6f" % EllipticalDistribution.computePDF(distribution, point)
    densityGeneratorDerivative = distribution.computeDensityGeneratorDerivative(beta)
    print "density generator derivative     =%.6f" % densityGeneratorDerivative
    print "density generator derivative (FD)=%.6f" % cleanScalar((distribution.computeDensityGenerator(beta + eps) - distribution.computeDensityGenerator(beta - eps)) / (2.0 * eps))
    densityGeneratorSecondDerivative = distribution.computeDensityGeneratorSecondDerivative(beta)
    print "density generator second derivative     =%.6f" % densityGeneratorSecondDerivative
    print "density generator second derivative (FD)=%.6f" % cleanScalar((distribution.computeDensityGeneratorDerivative(beta + eps) - distribution.computeDensityGeneratorDerivative(beta - eps)) / (2.0 * eps))

    # Compute the radial CDF
    radius = 2.0
    print "Radial CDF(%.6f" % radius , ")=%.6f" % distribution.computeRadialDistributionCDF(radius)

    # Extract the marginals
    for i in range(dim) :
        margin = distribution.getMarginal(i)
    print "margin=" , repr(margin)
    print "margin PDF=%.6f" % margin.computePDF(NumericalPoint(1, 0.5))
    print "margin CDF=%.6f" % margin.computeCDF(NumericalPoint(1, 0.5))
    print "margin quantile=" , repr(margin.computeQuantile(0.95))
    print "margin realization=" , repr(margin.getRealization())
    if (dim >= 2) :
        # Extract a 2-D marginal
        indices = Indices(2, 0)
        indices[0] = 1
        indices[1] = 0
        print "indices=" , repr(indices)
        margins = distribution.getMarginal(indices)
        print "margins=" , repr(margins)
        print "margins PDF=%.6f" % margins.computePDF(NumericalPoint(2, 0.5))
        print "margins CDF=%.6f" % margins.computeCDF(NumericalPoint(2, 0.5))
        quantile = margins.computeQuantile(0.95)
        print "margins quantile=" , repr(quantile)
        print "margins CDF(qantile)=%.6f" % margins.computeCDF(quantile)
        print "margins realization=" , repr(margins.getRealization())

    chol = distribution.getCholesky()
    invChol = distribution.getInverseCholesky()
    print "chol=" , repr(cleanMatrix(chol))
    print "invchol=" , repr(cleanMatrix(invChol))
    print "chol*t(chol)=" , repr(cleanMatrix(chol *chol.transpose()))
    print "chol*invchol=" , repr(cleanMatrix(chol*invChol))

except :
    import sys
    print "t_Normal_std.py", sys.exc_type, sys.exc_value
