#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try :

    try :
        # Function
        sampleSize = 20
        levelFunction = NumericalMathFunction("TestOptimLinear")
        myOperator = LessOrEqual()
        threshold = 2.0
        mySample = NumericalSample(0, levelFunction.getInputDimension())
        random = 0.1

        for index in range(sampleSize) :
            point = NumericalPoint(levelFunction.getInputDimension())
            norm = 0.0
            for coordinate in range(levelFunction.getInputDimension()) :
                point[coordinate] = sqrt(-2.0*log(random))
                random = fmod(random+sqrt(2.0), 1.0)
                point[coordinate] *= cos(2.0*atan(1.0)*random)
                norm += point[coordinate] * point[coordinate]

            for coordinate in range(levelFunction.getInputDimension()) :
                point[coordinate] /= sqrt(norm)

            mySample.add(point)

        myNearestPointChecker = NearestPointChecker(levelFunction, myOperator, threshold, mySample)
        for index in range(sampleSize) :
            print repr(mySample[index])
        myNearestPointChecker.run()
        print  "myNearestPointChecker = " , myNearestPointChecker
    except :
        raise

except :
    import sys
    print "t_NearestPointChecker_std.py", sys.exc_type, sys.exc_value
