#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    continuousDistributionCollection = DistributionCollection()
    discreteDistributionCollection = DistributionCollection()
    distributionCollection = DistributionCollection()

    beta = Beta(2.,3.,0.,1.)
    distributionCollection.add(beta)
    continuousDistributionCollection.add(beta)

    gamma = Gamma(1.,2.,3.)
    distributionCollection.add(gamma)
    continuousDistributionCollection.add(gamma)

    gumbel = Gumbel(1.,2.)
    distributionCollection.add(gumbel)
    continuousDistributionCollection.add(gumbel)

    lognormal = LogNormal(1.,1.,2.)
    distributionCollection.add(lognormal)
    continuousDistributionCollection.add(lognormal)

    logistic = Logistic(1.,1.)
    distributionCollection.add(logistic)
    continuousDistributionCollection.add(logistic)

    normal = Normal(1.,2.)
    distributionCollection.add(normal)
    continuousDistributionCollection.add(normal)

    truncatednormal = TruncatedNormal(1.,1.,0.,3.)
    distributionCollection.add(truncatednormal)
    continuousDistributionCollection.add(truncatednormal)

    student = Student(10.,10.)
    distributionCollection.add(student)
    continuousDistributionCollection.add(student)

    triangular = Triangular(-1.,2.,4.)
    distributionCollection.add(triangular)
    continuousDistributionCollection.add(triangular)

    uniform = Uniform(1.,2.)
    distributionCollection.add(uniform)
    continuousDistributionCollection.add(uniform)

    weibull = Weibull(1., 1., 2.)
    distributionCollection.add(weibull)
    continuousDistributionCollection.add(weibull)

    geometric = Geometric(.5)
    distributionCollection.add(geometric)
    discreteDistributionCollection.add(geometric)

    poisson = Poisson(2.)
    distributionCollection.add(poisson)
    discreteDistributionCollection.add(poisson)

    collection = UserDefinedPairCollection(3, UserDefinedPair(NumericalPoint(1), 0.0))

    point = NumericalPoint(1)
    point[0] = 1.0
    collection[0] = UserDefinedPair(point, 0.3)
    point[0] = 2.0
    collection[1] = UserDefinedPair(point, 0.2)
    point[0] = 3.0
    collection[2] = UserDefinedPair(point, 0.5)
    userdefined = UserDefined(collection)
    distributionCollection.add(userdefined)
    discreteDistributionCollection.add(userdefined)

    size = 10000

    # Number of continuous distributions
    continuousDistributionNumber = continuousDistributionCollection.getSize()
    # Number of discrete distributions
    discreteDistributionNumber = discreteDistributionCollection.getSize()
    # Number of distributions
    distributionNumber = continuousDistributionNumber + discreteDistributionNumber

    # We create a collection of NumericalSample of size "size" and of dimension 1 (scalar values) : the collection has distributionNumber NumericalSamples
    sampleCollection = [NumericalSample(size, 1) for i in range(distributionNumber)]
    # We create a collection of NumericalSample of size "size" and of dimension 1 (scalar values) : the collection has continuousDistributionNumber NumericalSamples
    continuousSampleCollection = [NumericalSample(size, 1) for i in range(continuousDistributionNumber)]
    # We create a collection of NumericalSample of size "size" and of dimension 1 (scalar values) : the collection has discreteDistributionNumber NumericalSamples
    discreteSampleCollection = [NumericalSample(size, 1) for i in range(discreteDistributionNumber)]

    for i in range(continuousDistributionNumber) :
        continuousSampleCollection[i] = continuousDistributionCollection[i].getSample(size)
        continuousSampleCollection[i].setName(continuousDistributionCollection[i].getName())
        sampleCollection[i] = continuousSampleCollection[i]
    for i in range(discreteDistributionNumber) :
        discreteSampleCollection[i] = discreteDistributionCollection[i].getSample(size)
        discreteSampleCollection[i].setName(discreteDistributionCollection[i].getName())
        sampleCollection[continuousDistributionNumber + i] = discreteSampleCollection[i]

    # Test the normality of several samples using the Anderson Darling test
    andersonDarlingResult = NumericalPoint(distributionNumber)
    for i in range(distributionNumber) :
        result = NormalityTest().AndersonDarlingNormal(sampleCollection[i])
        andersonDarlingResult[i] = result.getBinaryQualityMeasure()
        print "sample " , sampleCollection[i].getName() , " result=" , andersonDarlingResult[i]
    print "andersonDarlingResult=" , repr(andersonDarlingResult)

    # Test the normality of several samples using the Cramer Von Mises test
    cramerVonMisesResult = NumericalPoint(distributionNumber)
    for i in range(distributionNumber) :
        result = NormalityTest().CramerVonMisesNormal(sampleCollection[i])
        cramerVonMisesResult[i] = result.getBinaryQualityMeasure()
        print "sample " , sampleCollection[i].getName() , " result=" , cramerVonMisesResult[i]
    print "cramerVonMisesResult=" , repr(cramerVonMisesResult)

except :
    import sys
    print "t_NormalityTest_std.py", sys.exc_type, sys.exc_value
