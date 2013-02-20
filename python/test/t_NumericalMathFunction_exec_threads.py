#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    # Instance creation
    external = NumericalMathFunction("external_code_threads")

    size = 20
    dimension = external.getInputDimension()
    sample = NumericalSample(size, dimension)
    for i in range(size):
        p = NumericalPoint(dimension)
        for j in range(dimension):
            p[j] = float(i + j) / (size + dimension)
        sample[i] = p
    print "external code(sample) = ", repr(external(sample))

except  :
    import sys
    print "t_NumericalMathFunction_exec_sample.py", sys.exc_type, sys.exc_value
