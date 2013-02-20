#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    try :
        # Test function operator ()
        levelFunction = NumericalMathFunction("TestOptimLinear")
        specific = CobylaSpecificParameters()
        startingPoint = NumericalPoint(4, 1.0)
        myAlgorithm = Cobyla(specific, levelFunction)
        myAlgorithm.setStartingPoint(startingPoint)
        myAlgorithm.setLevelValue(3.0)
        myAlgorithm.setMaximumIterationsNumber(100)
        myAlgorithm.setMaximumAbsoluteError(1.0e-10)
        myAlgorithm.setMaximumRelativeError(1.0e-10)
        myAlgorithm.setMaximumResidualError(1.0e-10)
        myAlgorithm.setMaximumConstraintError(1.0e-10)
        print "myAlgorithm = ", repr(myAlgorithm)
    except :
        raise

except :
    import sys
    print "t_Cobyla_std.py", sys.exc_type, sys.exc_value
