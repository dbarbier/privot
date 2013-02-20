#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    # Fix the realization as a Normal
    # Parameters are done such as the values are positive
    epsilon = 0.01
    noiseDistribution = Normal(0, epsilon)

    # TimeGrid parameters
    n = 101
    timeStart = 0.0
    timeStep = 0.1
    timeGrid = RegularGrid(timeStart, timeStep, n)

    # White noise
    whiteNoise = WhiteNoise(noiseDistribution, timeGrid)

    # Now instantiation of a ARMA process
    arParameters = NumericalPoint(1, 0.1)
    maParameters = NumericalPoint(1, 5)
    arCoefficients = ARMACoefficients(arParameters)
    maCoefficients = ARMACoefficients(maParameters)

    # Create the ARMA process
    armaProcess = ARMA(arCoefficients, maCoefficients, whiteNoise)

    # 1 realization of the process
    realization = armaProcess.getRealization()

    # We have to translate manually in order to use the BoxCox evaluation on positive values
    sample = realization.getSample()

    alpha = -sample.getMin()[0] + 1.0e-4

    # Now we build the factory using the alpha parameter
    factory = BoxCoxFactory(alpha)

    # Evaluation of the BoxCoxTransfotm
    myBoxCox = factory.build(realization)

    print "myBoxCox = ", myBoxCox

    # We translate the input sample using the alpha value
    sample.translate(NumericalPoint(1, alpha))

    # We build a new TimeSeries
    translateTimeSerie = TimeSeries(timeGrid, sample)

    # Evaluation of the factory result on the new time series
    PlatformInfo.SetNumericalPrecision(5)
    print "input time series=", translateTimeSerie
    print "f(time series) = ", myBoxCox(translateTimeSerie)

except :
    import sys
    print "t_BoxCoxTransform_std.py", sys.exc_type, sys.exc_value
