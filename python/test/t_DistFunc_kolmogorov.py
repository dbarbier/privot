#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    # Kolmogorov related functions
    # pKolmogorov
    nN = 6
    xMin = 0.1
    xMax = 0.9
    nX = 5
    for n in range(1, nN+1):
        for iX in range(nX):
            x = xMin + (xMax - xMin) * iX / (nX - 1)
            print "pKolmogorov(", n, ", ", x, ")=%.6g" % DistFunc.pKolmogorov(n, x), ", complementary=%.6g" % DistFunc.pKolmogorov(n, x, True)
    # pKolmogorov asymptotic
    xMin = 0.1
    xMax = 0.9
    nX = 5
    for iX in range(nX):
        x = xMin + (xMax - xMin) * iX / (nX - 1)
        print "pKolmogorovAsymptotic(", x, ")=%.6g" % DistFunc.pKolmogorovAsymptotic(x), ", complementary=%.6g" % DistFunc.pKolmogorovAsymptotic(x, True)

except :
    import sys
    print "t_DistFunc_kolmogorov.py", sys.exc_type, sys.exc_value
