#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    # We create a numerical math function
    myFunction = NumericalMathFunction("poutre")

    dim = myFunction.getInputDimension()
    # We create a normal distribution point of dimension 1
    mean = NumericalPoint(dim, 0.0)
    mean[0] = 50.0 # E
    mean[1] =  1.0 # F
    mean[2] = 10.0 # L
    mean[3] =  5.0 # I
    sigma = NumericalPoint(dim, 1.0)
    R = IdentityMatrix(dim)
    myDistribution = Normal(mean, sigma, R)

    # We create a 'usual' RandomVector from the Distribution
    vect = RandomVector(myDistribution)

    # We create a composite random vector
    output = RandomVector(myFunction, vect)

    # We create an Event from this RandomVector
    myEvent= Event(output, ComparisonOperator(Less()), -3.0)

    # We create a NearestPoint algorithm
    myCobyla = Cobyla()
    myCobyla.setSpecificParameters(CobylaSpecificParameters())
    myCobyla.setMaximumIterationsNumber(100)
    myCobyla.setMaximumAbsoluteError(1.0e-10)
    myCobyla.setMaximumRelativeError(1.0e-10)
    myCobyla.setMaximumResidualError(1.0e-10)
    myCobyla.setMaximumConstraintError(1.0e-10)
    print "myCobyla=", myCobyla

    # We create a Analytical algorithm
    # The first parameter is a NearestPointAlgorithm
    # The second parameter is an event
    # The third parameter is a starting point for the design point research
    myAlgo = Analytical(myCobyla, myEvent, mean)

    print "Analytical=", myAlgo
    print "physical starting point=", myAlgo.getPhysicalStartingPoint()
    print "event=", myAlgo.getEvent()
    print "nearest point algorithm=", myAlgo.getNearestPointAlgorithm()
    print "result=", myAlgo.getAnalyticalResult()
    myAlgo.run()
    print "result=", myAlgo.getAnalyticalResult()

except :
    import sys
    print "t_Analytical_std.py", sys.exc_type, sys.exc_value
