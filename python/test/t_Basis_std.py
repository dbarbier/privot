#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try :
    b = Basis()
    print "basis =", b

except :
    import sys
    print "t_Basis_std.py", sys.exc_type, sys.exc_value
