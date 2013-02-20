#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    #We create a NumericalMathFunction
    myFunc = NumericalMathFunction("TestCompositionRight")
    #We create a distribution
    dim = myFunc.getInputDimension()
    meanPoint = NumericalPoint(dim, 0.2)
    sigma = NumericalPoint(dim)
    for i in range(dim) :
        sigma[i] = 0.1 * (i + 1)
    R = CorrelationMatrix(IdentityMatrix(dim))
    for i in range(1,dim) :
        R[i, i - 1] = 0.25

    distribution =  Normal(meanPoint, sigma, R)

    #We create a distribution-based RandomVector
    X = RandomVector(distribution)
    #We create a composite RandomVector Y from X and myFunction
    Y = RandomVector(myFunc, X)

    #We create a quadraticCumul algorithm
    myQuadraticCumul = QuadraticCumul(Y)
    #We test the attributs here
    print "myQuadraticCumul=", myQuadraticCumul
    #We compute the several elements provided by the quadratic cumul algorithm
    print "First order mean=", repr(myQuadraticCumul.getMeanFirstOrder())
    print "Second order mean=", repr(myQuadraticCumul.getMeanSecondOrder())
    print "Covariance=", repr(myQuadraticCumul.getCovariance())
    print "Value at mean=", repr(myQuadraticCumul.getValueAtMean())
    print "Gradient at mean=", repr(myQuadraticCumul.getGradientAtMean())
    print "Hessian at mean=", repr(myQuadraticCumul.getHessianAtMean())

except :
    import sys
    print "t_QuadraticCumul_run.py", sys.exc_type, sys.exc_value
