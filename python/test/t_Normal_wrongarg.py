#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    try :

        # Instanciate one distribution object
        meanPoint = NumericalPoint(1)
        meanPoint[0] = 1.0
        sigma = NumericalPoint(1)
        sigma[0] = 1.0
        R = CorrelationMatrix(1)
        distribution = Normal(meanPoint, sigma, R)
        print "Distribution " , repr(distribution)

        # We try to set an erroneous covariance matrix (wrong dimension) into distribution
        newR = CorrelationMatrix(2)
        distribution.setCorrelationMatrix(newR)

        # Normally, we should never go here
        raise

    except :
        pass

#except TestFailed, ex :
except :
    import sys
    print "t_Normal_wrongarg.py", sys.exc_type, sys.exc_value
