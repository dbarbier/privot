#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    # Instanciate one distribution object
    R = CorrelationMatrix(3)
    R[0, 1] = 0.5
    R[0, 2] = 0.25
    copula = ComposedCopula([FrankCopula(3.0), NormalCopula(R), ClaytonCopula(2.0)])

    print "Copula ", repr(copula)
    print "Copula ", copula

    # Is this copula elliptical ?
    print "Elliptical distribution= ", copula.isElliptical()

    # Is this copula continuous ?
    print "Continuous = ", copula.isContinuous()

    # Is this copula elliptical ?
    print "Elliptical = ", copula.hasEllipticalCopula()

    # Is this copula independent ?
    print "Independent = ", copula.hasIndependentCopula()

    # Test for realization of copula
    oneRealization = copula.getRealization()
    print "oneRealization=", repr(oneRealization)

    # Test for sampling
    size = 10000
    oneSample = copula.getSample( size )
    print "oneSample first=", repr(oneSample[0]), " last=", repr(oneSample[size - 1])
    print "mean=", repr(oneSample.computeMean())
    print "covariance=", repr(oneSample.computeCovariance())

    # Define a point
    point = NumericalPoint( copula.getDimension(), 0.6 )
    print "Point= ", repr(point)

    # Show PDF and CDF of point
    # NumericalScalar eps(1e-5)
    DDF = copula.computeDDF( point )
    print "ddf     =", repr(DDF)
    PDF = copula.computePDF( point )
    print "pdf     =%.6f" % PDF
    CDF = copula.computeCDF( point )
    print "cdf=%.6f" % CDF
    #    NumericalPoint PDFgr = copula.computePDFGradient( point )
    #    print "pdf gradient     =", PDFgr
    #    NumericalPoint CDFgr = copula.computeCDFGradient( point )
    #    print "cdf gradient     =", CDFgr
    quantile = copula.computeQuantile( 0.95 )
    print "quantile=", repr(quantile)
    print "cdf(quantile)=%.6f" % copula.computeCDF(quantile)
    mean = copula.getMean()
    print "mean=", repr(mean)
    covariance = copula.getCovariance()
    print "covariance=", repr(covariance)
    parameters = copula.getParametersCollection()
    print "parameters=", repr(parameters)

    # Specific to this copula

    # Extract a 5-D marginal
    dim = 5
    point = NumericalPoint(dim, 0.25)
    indices = Indices(dim, 0)
    indices[0] = 1
    indices[1] = 2
    indices[2] = 3
    indices[3] = 5
    indices[4] = 6
    print "indices=", repr(indices)
    margins = copula.getMarginal(indices)
    print "margins=", repr(margins)
    print "margins PDF=%.6f" % margins.computePDF(point)
    print "margins CDF=%.6f" % margins.computeCDF(point)
    quantile = margins.computeQuantile(0.95)
    print "margins quantile=", repr(quantile)
    print "margins CDF(quantile)=%.6f" % margins.computeCDF(quantile)
    print "margins realization=", repr(margins.getRealization())

except :
    import sys
    print "t_ComposedCopula.py", sys.exc_type, sys.exc_value
