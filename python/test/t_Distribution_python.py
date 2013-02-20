#! /usr/bin/env python

from openturns import *
import random

TESTPREAMBLE()

class PDIST(PythonDistribution):

    def __init__(self, a = 0.0, b = 1.0):
        PythonDistribution.__init__(self, 1)
        self.a = a
        self.b = b

    def getRealization(self):
        X = [self.a + (self.b - self.a) * RandomGenerator.Generate()]
        return X

    def getSample(self, size):
        X = []
        for i in range(size):
            X.append([10. + self.a + (self.b - self.a) * RandomGenerator.Generate()])
        return X

    def computeCDF(self, X):
        return (X[0] - self.a) / (self.b - self.a)

    def computePDF(self, X):
        return 1 / (self.b - self.a)

    def getMean(self):
        return [0.5 * (self.a + self.b)]

    def getStandardDeviation(self):
        return [-1.]

    def getSkewness(self):
        return [-1.]

    def getKurtosis(self):
        return [-1.]

    def getMoment(self, n):
        return [-0.1*n]

    def getCenteredMoment(self, n):
        return [-0.1*n]

R = PDIST()
print R

try :
    # Instance creation
    myDist = Distribution( R )
    print "myDist=", repr( myDist )

    # Copy constructor
    newRV = Distribution( myDist )

    # Dimension
    dim = myDist.getDimension()
    print 'dimension=', dim

    # Realization
    X = myDist.getRealization()
    print 'realization=', X

    # Sample
    X = myDist.getSample(5)
    print 'sample=', X

    # PDF
    pdf = myDist.computePDF([0.2])
    print 'pdf=', pdf

    # CDF
    cdf = myDist.computeCDF([0.2])
    print 'cdf=', cdf

    # Mean
    mean = myDist.getMean()
    print 'mean=', mean

    # Standard deviation
    standardDeviation = myDist.getStandardDeviation()
    print 'standardDeviation=', standardDeviation

    # Skewness
    skewness = myDist.getSkewness()
    print 'skewness=', skewness

    # Kurtosis
    kurtosis = myDist.getKurtosis()
    print 'kurtosis=', kurtosis

    # Moment
    moment = myDist.getMoment(3)
    print 'moment=', moment

    # Centered moment
    centeredMoment = myDist.getCenteredMoment(3)
    print 'centeredMoment=', centeredMoment

except :
    import sys
    print "t_Distribution_python.py", sys.exc_type, sys.exc_value
