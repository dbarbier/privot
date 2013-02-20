#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    # We create a NumericalMathFunction
    myFunction = NumericalMathFunction("wrapper")

    # We create a distribution
    dim = myFunction.getInputDimension()
    R = CorrelationMatrix(dim)
    for i in range(dim) :
        R[i,i] = 1.0
    for i in range(1,dim) :
        R[i, i - 1] = 0.5


    m = NumericalPoint(dim, 1.0)
    s = NumericalPoint(dim, 2.0)
    distribution = Normal(m, s, R)
    ref_distribution = distribution
    print "distribution = " , repr(ref_distribution)

    # We create a distribution-based RandomVector
    X = RandomVector(distribution)
    print "X=" , X
    print "is composite? " , X.isComposite()

    # Check standard methods of class RandomVector
    print "X dimension=" , X.getDimension()
    print "X realization (first )=" , repr(X.getRealization())
    print "X realization (second)=" , repr(X.getRealization())
    print "X realization (third )=" , repr(X.getRealization())
    print "X sample =" , repr(X.getSample(5))

    #We create a composite RandomVector Y from X and myFunction
    Y = RandomVector(CompositeRandomVector(myFunction, X))
    print "Y=" , Y
    print "is composite? " , Y.isComposite()

    # Check standard methods of class RandomVector
    print "Y dimension=" , Y.getDimension()
    print "Y realization (first )=" , repr(Y.getRealization())
    print "Y realization (second)=" , repr(Y.getRealization())
    print "Y realization (third )=" , repr(Y.getRealization())
    print "Y sample =" , repr(Y.getSample(5))


except :
    import sys
    print "t_RandomVector_function.py", sys.exc_type, sys.exc_value
