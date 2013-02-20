#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    #We test here the formulas on the identity function in R^4

    #We create a NumericalMathFunction
    myFuncIdentity = NumericalMathFunction("TestIdentity")

    #We create a distribution
    dim = myFuncIdentity.getInputDimension()
    meanPoint = NumericalPoint(dim, 0.1)
    sigma = NumericalPoint(dim, 1.0)
    R = CorrelationMatrix(dim)
    for i in range(dim) :
        R[i, i] = 1.0
    for i in range(1,dim) :
        R[i, i - 1] = 0.25

    distribution = Normal(meanPoint, sigma, R)

    #We create a distribution-based RandomVector
    X = RandomVector(distribution)
    print "InputDistribution = ", repr(distribution)

    #We create a  composite vector
    W = RandomVector(myFuncIdentity, X)

    #We create a quadraticCumul algorithm
    myQuadraticCumulIdentity = QuadraticCumul(W)

    #We test the attributs here
    print "myQuadraticCumul=", myQuadraticCumulIdentity

    #We compute the several elements provided by the quadratic cumul algorithm
    print "First order mean=", repr(myQuadraticCumulIdentity.getMeanFirstOrder())
    print "Second order mean=", repr(myQuadraticCumulIdentity.getMeanSecondOrder())
    print "Covariance=", repr(myQuadraticCumulIdentity.getCovariance())
    print "Value at mean=", repr(myQuadraticCumulIdentity.getValueAtMean())
    print "Gradient at mean=", repr(myQuadraticCumulIdentity.getGradientAtMean())
    print "Hessian at mean=", repr(myQuadraticCumulIdentity.getHessianAtMean())

except :
    import sys
    print "t_QuadraticCumul_std.py", sys.exc_type, sys.exc_value
