#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    # TEST NUMBER ZERO : DEFAULT CONSTRUCTOR AND CREATION OF LM
    print  "test number zero : default constructor and creation of linear model"

    # Default constructor
    lmfact = LinearModelFactory()

    # lm build
    size = 20
    oneSample = NumericalSample(size, 1)
    twoSample = NumericalSample(size, 1)
    for i in range(size):
        oneSample[i] = NumericalPoint(1, 7.0 * sin(-3.5 + (6.5 * i) / (size - 1.0)) + 2.0)
        twoSample[i] = NumericalPoint(1, -2.0 * oneSample[i, 0] + 3.0 + 0.05 * sin(oneSample[i, 0]))
    test = lmfact.build(oneSample,twoSample)
    print  "test = " , repr(test )

except  :
    import sys
    print "t_LinearModelFactory_std_std.py", sys.exc_type, sys.exc_value
