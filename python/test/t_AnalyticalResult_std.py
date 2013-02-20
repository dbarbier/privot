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
    myEvent = Event(output, ComparisonOperator(Less()), -3.0)

    # We create an AnalyticalResult based on fictive results
    result = AnalyticalResult(sigma, myEvent, False)

    print "result=", result

    digits = 5
    print "standard space design point=", printNumericalPoint(result.getStandardSpaceDesignPoint(), digits)
    print "physical space design point=", printNumericalPoint(result.getPhysicalSpaceDesignPoint(), digits)
    print "is standard point origin in failure space? ", result.getIsStandardPointOriginInFailureSpace()
    print "importance factors=", printNumericalPoint(result.getImportanceFactors(), digits)
    print "Hasofer reliability index=%.5f" % result.getHasoferReliabilityIndex()
    print "graph importance factors=", result.drawImportanceFactors()
    print "graph sensitivity=", result.drawHasoferReliabilityIndexSensitivity()
except :
    import sys
    print "t_AnalyticalResult_std.py", sys.exc_type, sys.exc_value
