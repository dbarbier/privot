#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    #We create a NumericalMathFunction
    myFunc = NumericalMathFunction("TestQuadraticCumul")

    #We create a distribution
    dim = myFunc.getInputDimension()
    meanPoint = NumericalPoint(dim, 0.1)
    sigma = NumericalPoint(dim, 1.0e-1)
    sigma[0] = 2.0e-1
    sigma[1] = 3.0e-1
    R = CorrelationMatrix(dim)
    for i in range(dim) :
        R[i, i] = 1.0
    for i in range(1,dim) :
        R[i, i-1] = 0.25

    myDistribution = Normal(meanPoint, sigma, R)

    # We name the components of the distribution
    componentDescription = Description(dim)
    componentDescription[0] = "Marginal 1"
    componentDescription[1] = "Marginal 2"
    myDistribution.setDescription(componentDescription)

    # We create a distribution-based RandomVector
    X = RandomVector(myDistribution)

    # We create a composite RandomVector Y from X and myFunction
    Y = RandomVector(myFunc, X)
    Y.setName("Interest Variable 1")

    # We create a quadraticCumul algorithm
    myQuadraticCumul = QuadraticCumul(Y)

    # We test the attributs here
    print "Importance factors=" , repr(myQuadraticCumul.getImportanceFactors())


except :
    import sys
    print "t_QuadraticCumul_importanceFactors.py", sys.exc_type, sys.exc_value
