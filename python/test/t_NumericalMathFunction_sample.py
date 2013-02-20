#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    #Instance creation
    deviation = NumericalMathFunction("poutre")

    inPoint1 = NumericalPoint(4)
    inPoint1[0] = 210.e9
    inPoint1[1] = 1000
    inPoint1[2] = 1.4
    inPoint1[3] = 2.e-6

    inPoint2 = NumericalPoint(4)
    inPoint2[0] = 210.e9
    inPoint2[1] = 1000
    inPoint2[2] = 1.5
    inPoint2[3] = 2.e-6

    inPoint3 = NumericalPoint(4)
    inPoint3[0] = 210.e9
    inPoint3[1] = 1000
    inPoint3[2] = 1.6
    inPoint3[3] = 2.e-6

    inSample = NumericalSample(3, deviation.getInputDimension())
    inSample[0] = inPoint1
    inSample[1] = inPoint2
    inSample[2] = inPoint3

    outSample = deviation( inSample )

    print "deviation sample = ", repr(outSample)

except :
    import sys
    print "t_NumericalMathFunction_sample.py", sys.exc_type, sys.exc_value
