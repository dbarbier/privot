#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try :
    f = LassoFactory()
    print "factory =", f

except :
    import sys
    print "t_LassoFactory_std.py", sys.exc_type, sys.exc_value
