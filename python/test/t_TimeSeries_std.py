#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    DIM = 2

    # We create an empty TimeSeries
    ts1 = TimeSeries(0, DIM)
    ts1.setName("Ts1")

    # We populate the empty ts
    for p in range(3):
        pt = NumericalPoint(DIM)
        for i in range(DIM):
            pt[i] = 10. * (p + 1) + i
        ts1.add( pt )

    print "ts1=", ts1

    # We get the second element of the ts
    secondElement = ts1[1]
    print "second element=", secondElement

    # We set the third element to a valid new element
    newPoint = NumericalPoint(DIM + 1)
    for i in range(DIM):
        newPoint[i+1] = 1000. * (i + 1)
    ts1[2] = newPoint
    print "ts1=", ts1

    # We get the values of the second element of the ts
    values1 = ts1.getValueAtIndex(1)
    print "values of the second element=", values1

    # We set new values for the second element of the ts
    newValues = values1 * 2.
    ts1.setValueAtIndex(1, newValues)
    values2 = ts1.getValueAtIndex(1)
    print "ts1=", ts1
    print "modified values of the second element=", values2, " should be ", newValues

    # We get the values of the second element of the ts
    values3 = ts1.getValueAtNearestTime( -1.0 )
    print "values at t=-1.0 =", values3
    values4 = ts1.getValueAtNearestTime( 1.45 )
    print "values at t=1.45 =", values4
    values5 = ts1.getValueAtNearestTime( 1.54 )
    print "values at t=1.54 =", values5
    values6 = ts1.getValueAtNearestTime( 14.5 )
    print "values at t=14.5 =", values6

    # We set new values for the third element of the ts
    ts1.setValueAtNearestTime( 1.54, values6 * -1.0 )
    print "ts1=", ts1

    ts2 = TimeSeries(10, DIM)
    print "ts2=", ts2

    # ts2[5] = point2
    # print "ts2=", ts2


    try:
        # We get the tenth element of the ts
        # THIS SHOULD NORMALLY FAIL
        tenthElement = ts1.at(9)
    except:
        print "Expected failure"

    tg1 = RegularGrid( 0.0, 0.1, 11 )
    ts3 = TimeSeries( tg1, DIM )
    print "ts3=", ts3

    tg2 = RegularGrid( 0.0, 0.2, 6 )
    ts4 = TimeSeries( tg2, DIM )
    print "ts4=", ts4


    # We append a sample to a time series
    ts5 = TimeSeries(3, DIM)
    ns1 = NumericalSample(3, NumericalPoint(DIM, 99.9))
    print "ts5=", ts5
    ts5.add( ns1 )
    print "ts5=", ts5

    # We retrieve the values of the time series as a sample
    ns2 = ts5.getSample()
    print "ns2=", ns2

    # We build a time series from a time grid and a sample
    ns3 = ts4.getSample()
    ts6 = TimeSeries( tg2, ns3 )
    print "ts6=", ts6

except :
    import sys
    print "t_TimeSeries_std.py", sys.exc_type, sys.exc_value
