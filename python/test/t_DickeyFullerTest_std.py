#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    size = 100

    # ARMA parameters
    ARCoefficients = ARMACoefficients(NumericalPoint([0.3]))
    MACoefficients = ARMACoefficients(0)
    timeGrid = RegularGrid(0.0, 0.1, size)

    # White noise ==> gaussian
    whiteNoise = WhiteNoise(Normal(), timeGrid)
    myARMA = ARMA(ARCoefficients, MACoefficients, whiteNoise)

    # A realization of the ARMA process
    # The realization is supposed to be of a stationnary process
    realization = myARMA.getRealization()

    # In the strategy of tests, one has to detect a trend tendency
    # We check if the time series writes as x_t = a +b * t + c * x_{t-1}
    # H0 = c is equal to one and thus
    # p-value threshold : probability of the H0 reject zone : 1-0.95
    # p-value : probability (test variable decision > test variable decision (statistic) evaluated on data)
    # Test = True <=> p-value > p-value threshold
    test = DickeyFullerTest(realization)
    print  "Drift and linear trend model=" , test.testUnitRootInDriftAndLinearTrendModel(0.95)
    print  "Drift model=" , test.testUnitRootInDriftModel(0.95)
    print  "AR1 model=" , test.testUnitRootInAR1Model(0.95)

except :
    import sys
    print "t_DickeyFullerTest_std.py", sys.exc_type, sys.exc_value
