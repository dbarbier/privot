#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    size = 10
    dimension = 2
    sample = NumericalSample(size, dimension)
    # Fill-in the sample
    for i in range(size):
        p = NumericalPoint(dimension)
        for j in range(dimension):
            p[j] = i + j * 1.0 / dimension
        sample[i] = p
    print "sample=", repr(sample)
    # History using the Null strategy
    nullStrategy = Null()
    for i in range(size):
        nullStrategy.store(sample[i])
    print "Null strategy sample=", repr(nullStrategy.getSample())
    # History using the Full strategy
    fullStrategy = Full()
    for i in range(size):
        fullStrategy.store(sample[i])
    print "Full strategy sample=", repr(fullStrategy.getSample())
    # History using the Last strategy, large storage
    lastStrategy= Last(3 * size)
    for i in range(size):
        lastStrategy.store(sample[i])
    print "Last strategy sample (large storage)=", repr(lastStrategy.getSample())
    lastStrategy = Last(divmod(size, 3)[0])
    # History using the Last strategy, small storage
    for i in range(size):
        lastStrategy.store(sample[i])
    print "Last strategy sample (small storage)=", repr(lastStrategy.getSample())
    # History using the Compact strategy, large storage
    compactStrategy = Compact(3 * size)
    for i in range(size):
        compactStrategy.store(sample[i])
    print "Compact strategy sample (large storage)=", repr(compactStrategy.getSample())
    compactStrategy = Compact(divmod(size, 3)[0])
    # History using the Compact strategy, small storage
    for i in range(size):
        compactStrategy.store(sample[i])
    print "Compact strategy sample (small storage)=", repr(compactStrategy.getSample())

except :
    import sys
    print "t_HistoryStrategy_std.py", sys.exc_type, sys.exc_value
