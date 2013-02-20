#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try :
    f = LeastSquaresMetaModelSelectionFactory( LAR(), CorrectedLeaveOneOut() )
    print "factory =", f

except :
    import sys
    print "t_LeastSquaresMetaModelSelectionFactory_std.py", sys.exc_type, sys.exc_value
