#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    size = 4
    indices = Indices(size, 0)
    for i in range(size):
        indices[i] = i
    print "indices=", repr(indices)
    print "are indices valid with bound=", size, "? ", indices.check(size)
    print "are indices valid with bound=", divmod(size, 2)[0], "? ", indices.check(divmod(size, 2)[0])
    indices[0] = indices[size-1]
    print "indices after transformation=", repr(indices)
    print "are indices valid with bound=", size, "? ", indices.check(size)


    # slicing
    indices2 = Indices( (1,2,3,4,5) )
    print 'indices2[2] = ', indices2[2]
    print 'indices2[1:3] = ', indices2[1:3]
    print 'indices2[:] = ', indices2[:]
    indices2[1:3] = (88,99)
    print 'indices2 = ', indices2

except :
    import traceback
    traceback.print_exc()
