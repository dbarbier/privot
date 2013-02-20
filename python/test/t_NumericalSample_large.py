#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    # We create an empty NumericalSample
    sample = NumericalSample(0, 2)
    sample.setName("EmptySample")
    print "sample=", repr(sample)

    try :
        # We access the element of the sample
        p = sample[0]
        print "p=", repr(p)

        # We should NEVER go here
        # no TestFailed available
        raise
    #except OutOfBoundException, ex :
    except :
        pass


    #We create an small NumericalSample
    sample = NumericalSample(1, 2)
    sample.setName("SmallSample")
    print "sample=", repr(sample)

    #We access the element of the sample
    p = NumericalPoint(sample[0])
    print "p=", repr(p)
    try :
        #We try to access past the last element of the point
        p0, p1, p2 = p[0], p[1], p[2]
        print "p[0]=", p0, " p[1]=", p1, " p[2]=", p2
        # We should NEVER go here
        raise
    except :
        pass

    #We create a big NumericalSample
    sample = NumericalSample(1000000, 2)
    sample.setName("BigSample")

    #We populate the sample
    for i in range(sample.getSize()):
        sample[i,0] = i
        sample[i,1] = i

    print "sample first point=", repr(sample[0])
    print "sample last  point=", repr(sample[sample.getSize()-1])

#except TestFailed, ex :
except :
    import sys
    print "t_NumericalSample_large.py", sys.exc_type, sys.exc_value
