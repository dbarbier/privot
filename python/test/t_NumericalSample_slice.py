#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    # We create an empty NumericalSample
    N=10
    d=5
    sample = NumericalSample(N, d)

    # We populate the empty sample with numerous data
    for i in range(N):
        for j in range(d):
            sample[i,j] = 100.*(j+1)+i
    print 'sample=', str(sample)

    ### We try to slice the sample

    ## Reading slicing

    # case #0 [slice]
    print 'rslice[:]=', sample[:]
    print 'rslice[2:4]=', sample[2:4]
    print 'rslice[1:8:2]=', sample[1:8:2]

    # case #1 [slice/slice]
    print 'rslice[:,:]=', sample[:,:]
    print 'rslice[2:4,2:4]=', sample[2:4,2:4]
    print 'rslice[1:8:2,1:5:3]=', sample[1:8:2,1:5:3]

    # case #2 [slice/index]
    print 'rslice[:,3]=', sample[:,3]
    print 'rslice[2:7:2,3]=', sample[2:7:2,3]

    # case #3 [index/slice]
    print 'rslice[3,:]=', sample[3,:]
    print 'rslice[3,1:4:2]=', sample[3,1:4:2]

    # case #4 [index/index]
    print 'rslice[3,4]=', sample[3,4]

    ## Writing slice
    sample2 = NumericalSample(sample)
    sample2.scale(NumericalPoint(d,10.))

    # case #0 [slice]
    sample3 = NumericalSample(sample)
    sample3[:] = sample2
    print "wrslice[:]=", sample3
    sample3 = NumericalSample(sample)
    sample3[2:4] = sample2[2:4]
    print 'wslice[2:4]=', sample3
    sample3 = NumericalSample(sample)
    sample3[1:8:2] = sample2[1:8:2]
    print 'wslice[1:8:2]=', sample3

    # case #1 [slice/slice]
    sample3 = NumericalSample(sample)
    sample3[:,:] = sample2
    print "wrslice[:,:]=", sample3
    sample3 = NumericalSample(sample)
    sample3[2:4,2:4] = sample2[2:4,2:4]
    print 'wslice[2:4,2:4]=', sample3
    sample3 = NumericalSample(sample)
    sample3[1:8:2,1:5:3] = sample2[1:8:2,1:5:3]
    print 'wslice[1:8:2,1:5:3]=', sample3

    # case #2 [slice/index]
    sample3 = NumericalSample(sample)
    sample3[:,3] = sample2[:,3]
    print 'wslice[:,3]=', sample3
    sample3 = NumericalSample(sample)
    sample3[2:7:2,3] = sample2[2:7:2,3]
    print 'wslice[2:7:2,3]=', sample3

    # case #3 [index/slice]
    sample3 = NumericalSample(sample)
    sample3[3,:] = sample2[3,:]
    print 'wslice[3,:]=', sample3
    sample3 = NumericalSample(sample)
    sample3[3,1:4:2] = sample2[3,1:4:2]
    print 'wslice[3,1:4:2]=', sample3

    # case #4 [index/index]
    sample3 = NumericalSample(sample)
    sample3[3,4] = sample2[3,4]
    print 'wslice[3,4]=',sample3

except :
    import sys
    print "t_NumericalSample_slice.py", sys.exc_type, sys.exc_value
