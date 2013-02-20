#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
      # Create a collection of test-cases and the associated references
      numberOfTests = 3
      testCases = list()
      references = DistributionCollection(numberOfTests)
      testCases.append(DistributionCollection(2))
      testCases[0][0] = Uniform(-1.0, 3.0)
      testCases[0][1] = Uniform(-1.0, 3.0)
      references[0] = Triangular(-2.0, 2.0, 6.0)
      testCases.append(DistributionCollection(3))
      testCases[1][0] = Normal()
      testCases[1][1] = Normal(1.0, 2.0)
      testCases[1][2] = Normal(-2.0, 2.0)
      references[1] = Normal(-1.0, 3.0)
      testCases.append(DistributionCollection(3))
      testCases[2][0] = Exponential()
      testCases[2][1] = Exponential()
      testCases[2][2] = Exponential()
      references[2] = Gamma(3.0, 1.0, 0.0)
      print "testCases=", testCases
      print "references=", references
      for testIndex in range(len(testCases)):
          # Instanciate one distribution object
          distribution = RandomMixture(testCases[testIndex])
          distribution.setBlockMin(5)
          distribution.setBlockMax(20)
          distributionReference = references[testIndex]
          print "Distribution ", repr(distribution)
          print "Distribution ", distribution

          # Is this distribution elliptical ?
          print "Elliptical = ", distribution.isElliptical()

          # Is this distribution continuous ?
          print "Continuous = ", distribution.isContinuous()

          # Test for realization of distribution
          oneRealization = distribution.getRealization()
          print "oneRealization=", oneRealization

          # Test for sampling
          size = 10000
          oneSample = distribution.getSample( size )
          print "oneSample first=", oneSample[0], " last=", oneSample[size - 1]
          print "mean=", oneSample.computeMean()
          print "covariance=", oneSample.computeCovariance()

          # Define a point
          point = NumericalPoint(distribution.getDimension(), 0.5)
          print "Point= ", point

          # Show PDF and CDF of point
          eps = 1e-5
          DDF = distribution.computeDDF(point)
          print "ddf      =", DDF
          print "ddf (ref)=", distributionReference.computeDDF(point)
          PDF = distribution.computePDF(point)
          print "pdf      =%.6f" % PDF
          print "pdf  (FD)=%.6f" % ((distribution.computeCDF( point + NumericalPoint(1, eps) ) - distribution.computeCDF( point  + NumericalPoint(1, -eps) )) / (2.0 * eps))
          print "pdf (ref)=%.6f" % distributionReference.computePDF(point)
          CDF = distribution.computeCDF( point )
          print "cdf      =%.6f" % CDF
          print "cdf (ref)=%.6f" % distributionReference.computeCDF(point)
          CF = distribution.computeCharacteristicFunction( point[0] )
          print "characteristic function=%.6f + %.6fi" % (CF.real, CF.imag) 
          LCF = distribution.computeLogCharacteristicFunction( point[0] )
          print "log characteristic function=%.6f + %.6fi" % (LCF.real, LCF.imag)
          quantile = distribution.computeQuantile( 0.95 )
          print "quantile      =", quantile
          print "quantile (ref)=", distributionReference.computeQuantile(0.95)
          print "cdf(quantile)=%.6f" % distribution.computeCDF(quantile)
          mean = distribution.getMean()
          print "mean      =", mean
          print "mean (ref)=", distributionReference.getMean()
          standardDeviation = distribution.getStandardDeviation()
          print "standard deviation      =", standardDeviation
          print "standard deviation (ref)=", distributionReference.getStandardDeviation()
          skewness = distribution.getSkewness()
          print "skewness      =", skewness
          print "skewness (ref)=", distributionReference.getSkewness()
          kurtosis = distribution.getKurtosis()
          print "kurtosis      =", kurtosis
          print "kurtosis (ref)=", distributionReference.getKurtosis()
          covariance = distribution.getCovariance()
          print "covariance      =", covariance
          print "covariance (ref)=", distributionReference.getCovariance()
          parameters = distribution.getParametersCollection()
          print "parameters=", parameters
          print "Standard representative=", distribution.getStandardRepresentative()

          # Specific to this distribution
          weights = distribution.getWeights()
          print "weights=", weights
          distribution.setWeights(2.0 * weights)
          print "new weights=", distribution.getWeights()
          print "blockMin=", distribution.getBlockMin()
          print "blockMax=", distribution.getBlockMax()
          print "maxSize=", distribution.getMaxSize()
          print "alpha=", distribution.getAlpha()
          print "beta=", distribution.getBeta()
      # Tests of the simplification mechanism
      weights = NumericalPoint(0)
      coll = DistributionCollection(0)
      coll.add(Dirac(0.5))
      weights.add(1.0)
      coll.add(Normal(1.0, 2.0))
      weights.add(2.0)
      coll.add(Normal(2.0, 1.0))
      weights.add(-3.0)
      coll.add(Uniform(-2.0, 2.0))
      weights.add(-1.0)
      coll.add(Exponential(2.0, -3.0))
      weights.add(1.5)
      rm = RandomMixture(coll, weights)
      coll.add(rm)
      weights.add(-2.5)
      coll.add(Gamma(3.0, 4.0, -2.0))
      weights.add(2.5)
      distribution = RandomMixture(coll, weights)
      print "distribution=", repr(distribution)
      print "distribution=", distribution
      for i in range(10):
	   x = distribution.getMean()[0] + (-3.0 + 6.0 * i / 9.0) + distribution.getStandardDeviation()[0]
           print "pdf(", x, ")=%.6f" % distribution.computePDF(x)

except :
    import sys
    print "t_RandomMixture_std.py", sys.exc_type, sys.exc_value
