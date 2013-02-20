#! /usr/bin/env python

from openturns import *

from math import *

TESTPREAMBLE()

try :
    dim = 4
    optimizer = NumericalPoint(dim, 1.0)
    optimalValue = 5.0
    optimization = BoundConstrainedAlgorithmImplementationResult.MINIMIZATION
    evaluationsNumber = 10
    absoluteError = 1e-6
    relativeError = 1e-7
    objectiveError = 1e-8
    constraintError =1e-10
    result = BoundConstrainedAlgorithmImplementationResult(optimizer, optimalValue, optimization, evaluationsNumber, absoluteError, relativeError, objectiveError, constraintError)
    print "result=", repr(result)
    print "minimizer=", result.getOptimizer()
    print "optimal value=", result.getOptimalValue()
    print "optimization problem=", result.getOptimizationProblem()
    print "evaluations number=", result.getEvaluationsNumber()
    print "absolute error=", result.getAbsoluteError()
    print "relativeError=", result.getRelativeError()
    print "objective error=", result.getObjectiveError()
    print "constraint error=", result.getConstraintError()

except :
    import sys
    print "t_BoundConstrainedAlgorithmImplementationResult_std.py", sys.exc_type, sys.exc_value
