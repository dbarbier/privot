#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    levelFunction = NumericalMathFunction("TestOptimLinear")
    specific = TNCSpecificParameters()
    startingPoint = NumericalPoint(4, 1.0)
    bounds = Interval(NumericalPoint(4, -3.0), NumericalPoint(4, 5.0))
    myAlgorithm = TNC(specific, levelFunction, bounds, startingPoint, BoundConstrainedAlgorithmImplementationResult.MINIMIZATION)
    myAlgorithm.setMaximumEvaluationsNumber(100)
    myAlgorithm.setMaximumAbsoluteError(1.0e-10)
    myAlgorithm.setMaximumRelativeError(1.0e-10)
    myAlgorithm.setMaximumObjectiveError(1.0e-10)
    myAlgorithm.setMaximumConstraintError(1.0e-10)
    print "myAlgorithm = ", myAlgorithm
except :
    import sys
    print "t_TNC_std.py", sys.exc_type, sys.exc_value
