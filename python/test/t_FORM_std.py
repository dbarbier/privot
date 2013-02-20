#! /usr/bin/env python

from openturns import *

from math import *

TESTPREAMBLE()

def printNumericalPoint(point, digits) :
    oss = "["
    eps = pow(0.1, digits)
    for i in range(point.getDimension()) :
        if i == 0 :
            sep = ""
        else :
            sep = ","
        if fabs(point[i]) < eps :
            oss += sep + '%.6f' % fabs(point[i])
        else :
            oss += sep + '%.6f' % point[i]
        sep = ","
    oss += "]"
    return oss

try :

        # We create a numerical math function
    myFunction = NumericalMathFunction("poutre")

    dim = myFunction.getInputDimension()

    # We create a normal distribution point of dimension 1
    mean = NumericalPoint(dim, 0.0)
    # E
    mean[0] = 50.0
    # F
    mean[1] =  1.0
    # L
    mean[2] = 10.0
    # I
    mean[3] =  5.0
    sigma = NumericalPoint(dim, 1.0)
    R = IdentityMatrix(dim)
    myDistribution = Normal(mean, sigma, R)

    # We create a 'usual' RandomVector from the Distribution
    vect = RandomVector(myDistribution)

    # We create a composite random vector
    output = RandomVector(myFunction, vect)

    # We create an Event from this RandomVector
    myEvent = Event(output, ComparisonOperator(Less()), -3.0)

    # We create a NearestPoint algorithm
    myCobyla = Cobyla()
    myCobyla.setSpecificParameters(CobylaSpecificParameters())
    myCobyla.setMaximumIterationsNumber(100)
    myCobyla.setMaximumAbsoluteError(1.0e-10)
    myCobyla.setMaximumRelativeError(1.0e-10)
    myCobyla.setMaximumResidualError(1.0e-10)
    myCobyla.setMaximumConstraintError(1.0e-10)
    print  "myCobyla=", myCobyla

    # We create a FORM algorithm
    # The first parameter is a NearestPointAlgorithm
    # The second parameter is an event
    # The third parameter is a starting point for the design point research
    myAlgo = FORM(myCobyla, myEvent, mean)

    print  "FORM=" , myAlgo

    # Perform the simulation
    myAlgo.run()

    # Stream out the iresult
    result = myAlgo.getResult()
    digits = 5
    print  "event probability=%.6f" % result.getEventProbability()
    print  "generalized reliability index=%.6f" % result.getGeneralisedReliabilityIndex()
    print  "standard space design point=" , printNumericalPoint(result.getStandardSpaceDesignPoint(), digits)
    print  "physical space design point=" , printNumericalPoint(result.getPhysicalSpaceDesignPoint(), digits)

    # Is the standard point origin in failure space?
    print "is standard point origin in failure space? %s" % (result.getIsStandardPointOriginInFailureSpace() and "true" or "false")

    print  "importance factors=" , printNumericalPoint(result.getImportanceFactors(), digits)
    print  "Hasofer reliability index=%.6f" % result.getHasoferReliabilityIndex()

except :
    import sys
    print "t_FORM_std.py", sys.exc_type, sys.exc_value
