#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    size = 10
    for dimension in range(1, 4):
        f = LinearEnumerateFunction(dimension)
        print "First", size, " values for dimension", dimension
        for index in range(size):
            multiIndex = f(index)
            print "index=", index, ", multi-index=", multiIndex, ", linear index=", f.inverse(multiIndex)
except :
    import sys
    print "t_LinearEnumerateFunction_std.py", sys.exc_type, sys.exc_value
