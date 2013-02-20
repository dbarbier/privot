#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try :
    b = Basis()
    seq = BasisSequence( b )
    print "sequence =", seq

except :
    import sys
    print "t_BasisSequence_std.py", sys.exc_type, sys.exc_value
