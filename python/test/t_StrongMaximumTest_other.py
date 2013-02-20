#! /usr/bin/env python

from openturns import *

from math import *
from math import *

def printSample(name, sample) :
    oss =  name + "=["
    for counter in range(sample.getSize()) :
        if (counter != 0) :
            oss += ";"
        oss +=  "["
        point = sample[counter]
        for coordinate in range(point.getDimension()) :
            if (coordinate != 0) :
                oss +=  ","
            oss += "%.6f" % point[coordinate]
        oss += "]"
    oss += "]"
    return oss

TESTPREAMBLE()

try :

    # We create a numerical math function
    myFunction = NumericalMathFunction("TestStrongMaximumTest2")

    dim = myFunction.getInputDimension()
    # We create a normal distribution point of dimension 1
    mean = NumericalPoint(dim, 0.0)
    sigma = NumericalPoint(dim, 1.0)
    R = IdentityMatrix(dim)
    myDistribution = Normal(mean, sigma, R)

    # We create a 'usual' RandomVector from the Distribution
    vect = RandomVector(myDistribution)

    # We create a composite random vector
    output = RandomVector(myFunction, vect)

    # We create a StandardEvent from this RandomVector  : RandomVector > 4.0
    seuil = 40
    myStandardEvent = StandardEvent(output, ComparisonOperator(Greater()), seuil)

    # We create the design point
    designPoint = NumericalPoint(dim, 0.0)
    C = 0.6
    designPoint[0] = - sqrt(seuil) + C

    # We create the "second" design point
    pseudoDesignPoint = NumericalPoint(dim, 0.0)
    pseudoDesignPoint[0] = sqrt(seuil) + C

    importanceLevel = 0.01
    accuracyLevel = 2
    confidenceLevel = 0.999

    myTest = StrongMaximumTest(myStandardEvent, designPoint, importanceLevel, accuracyLevel, confidenceLevel)
    print  "myTest=" , myTest
    myTest.run()
    print  "Beta = %.6f" % designPoint.norm()
    print  "Discretised sphere radius = %6f" % (designPoint.norm() * (1 + myTest.getAccuracyLevel() * myTest.getDeltaEpsilon()))
    print  "PointNumber = " , myTest.getPointNumber()
    print  "DesignPointVicinity Angle (rad)= %.6f" % acos(myTest.getDesignPointVicinity())
    print  "DesignPointVicinity Angle (deg)= %.6f" % (acos(myTest.getDesignPointVicinity())*180/pi)
    print  "Near Design Point Verifying Event Points Number = " , (myTest.getNearDesignPointVerifyingEventPoints()).getSize()
    print  "Near Design Point Violating Event Points Number = " , (myTest.getNearDesignPointViolatingEventPoints()).getSize()
    print  "Far Design Point Verifying Event Points Number = " , (myTest.getFarDesignPointVerifyingEventPoints()).getSize()
    print  "Far Design Point Violating Event Points Number = " , (myTest.getFarDesignPointViolatingEventPoints()).getSize()
    print  "//////"

    # parameters of the test
    print  "importanceLevel=%.6f" % myTest.getImportanceLevel()
    print  "accuracyLevel=%.6f" % myTest.getAccuracyLevel()
    print  "confidenceLevel=%.6f" % myTest.getConfidenceLevel()

    # design point coordinates
    print  "standardSpaceDesignPoint=[%.6f" % myTest.getStandardSpaceDesignPoint()[0], ",%.6f" % myTest.getStandardSpaceDesignPoint()[1] , "]"

    # pseudo design point coordinates
    print  "pseudoStandardSpaceDesignPoint=[%.6f" % pseudoDesignPoint[0] , ",%.6f" % pseudoDesignPoint[1] , "]"

    # cout of the coordinates of the points of the 4 samples
    # NearDesignPointVerifyingEventPoints
    print printSample("NearDesignPointVerifyingEventPointsSample",myTest.getNearDesignPointVerifyingEventPoints())
    # NearDesignPointViolatingEventPoints
    print printSample("NearDesignPointViolatingEventPoints",myTest.getNearDesignPointViolatingEventPoints())
    # FarDesignPointVerifyingEventPoints
    print printSample("FarDesignPointVerifyingEventPoints",myTest.getFarDesignPointVerifyingEventPoints())
    # FarDesignPointViolatingEventPoints
    print printSample("FarDesignPointViolatingEventPoints",myTest.getFarDesignPointViolatingEventPoints())

    #    print  "myTest (after run())=" , myTest

except :
    import sys
    print "t_strongMaxTest_other.py", sys.exc_type, sys.exc_value
