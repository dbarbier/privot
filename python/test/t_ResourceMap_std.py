#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    resourceMap = ResourceMap.GetInstance()
    print resourceMap
    print "Extract from ResourceMap : R-executable-command -> ", resourceMap.get("R-executable-command")
except :
    import sys
    print "t_ResourceMap_std.py", sys.exc_type, sys.exc_value
