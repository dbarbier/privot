#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try :
    x = NumericalSample(1,1)
    y = NumericalSample(1,1)
    psi = Basis()
    psi.add(NumericalMathFunction(Description("x"), Description("x")))

    algo = Lasso( x, y, psi )
    print "algo =", algo

except :
    import sys
    print "t_Lasso_std.py", sys.exc_type, sys.exc_value
