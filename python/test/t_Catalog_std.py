#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    catalog = Catalog.GetInstance()
    print catalog

except :
    import sys
    print "t_Catalog_std.py", sys.exc_type, sys.exc_value
