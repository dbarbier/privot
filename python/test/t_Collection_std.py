#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    # We try to use the Collection as a Python object (sequence)
    ulc1 = UnsignedLongCollection( (1,2,3,4,5) )
    ulc2 = UnsignedLongCollection( [1,2,3,4,5] )
    t = (1,2,3,4,5)
    l = [1,2,3,4,5]
    ulc3 = UnsignedLongCollection( t )
    ulc4 = UnsignedLongCollection( l )

    ulc1[2] = 100
    print ulc1
    print len( ulc2 ), ulc2[0]
    for ul in ulc1:
        print ul

    # Large collection, to see its length
    ul5 = UnsignedLongCollection(range(12))
    print ul5

    del ul5[6]
    print ul5

except :
    import sys
    print "t_Collection_std.py", sys.exc_type, sys.exc_value
