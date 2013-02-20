#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    # Default dimension parameter to evaluate the model
    defaultDimension = 1

    # Amplitude values
    amplitude = NumericalPoint(defaultDimension, 1.0)

    # Scale values
    scale = NumericalPoint(defaultDimension, 1.0)

    # Default constructor
    myDefautModel = ExponentialCauchy()
    print  "myDefautModel = " , myDefautModel

    # Second order model with parameters
    myModel = ExponentialCauchy(amplitude, scale)
    print  "myModel = " , myModel

    timeValueOne = 1.
    print  "covariance matrix at t = " , timeValueOne , " : " , myModel.computeCovariance(timeValueOne)
    print  "covariance matrix at t = " , -1.0 * timeValueOne , " : " , myModel.computeCovariance(-1.0 * timeValueOne)

    # Evaluation at time higher to check the decrease of the exponential values
    timeValueHigh = 15.
    print  "covariance matrix at t = "  , timeValueHigh , " : " , myModel.computeCovariance(timeValueHigh).__str__()

    timeGrid = RegularGrid(0.0, 1.0 / 3.0, 4)
    print "discretized covariance over the time grid=", timeGrid, "is=", myModel.discretizeCovariance(timeGrid)

    # Default dimension parameter to evaluate the model
    highDimension = 3

    # Reallocation of adequate sizes
    amplitude.resize(highDimension)
    scale.resize(highDimension)
    spatialCorrelation = CorrelationMatrix(highDimension)
    for index in range(highDimension):
        amplitude[index] = 1.0
        scale[index] = (index + 1.0) / (defaultDimension * defaultDimension)
        if index > 0:
            spatialCorrelation[index, index - 1] = 1.0 / (index * index)

    # check the cast
    mySecondOrderModel = SecondOrderModel(ExponentialCauchy(amplitude, scale, spatialCorrelation))
    print "mySecondOrderModel = " , mySecondOrderModel

    # Second order model  - dimension 10
    myHighModel = ExponentialCauchy(amplitude, scale, spatialCorrelation)
    print  "myHighModel = " , myHighModel

    print "covariance matrix at t = " , timeValueOne , " : " , myHighModel.computeCovariance(timeValueOne)
    print "covariance matrix at t = " , -1.0 * timeValueOne , " : " , myHighModel.computeCovariance(-1.0 * timeValueOne)
    print "covariance matrix at t = "  , timeValueHigh , " : " , myHighModel.computeCovariance(timeValueHigh)

    print "discretized covariance over the time grid=", timeGrid, "is=", myHighModel.discretizeCovariance(timeGrid)

except :
    import sys
    print "t_ExponentialCauchy_std.py", sys.exc_type, sys.exc_value
