#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    dim = 2
    transformation = NatafIndependentCopulaHessian(dim)
    print "transformation=", repr(transformation)
    point = NumericalPoint(dim, 0.75)
    print "transformation(", point, ")=", repr(transformation.hessian(point))
    print "input dimension=", transformation.getInputDimension()
    print "output dimension=", transformation.getOutputDimension()

except :
    import sys
    print "t_NatafIndependentCopulaHessian_std.py", sys.exc_type, sys.exc_value
