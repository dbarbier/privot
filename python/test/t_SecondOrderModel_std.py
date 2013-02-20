#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    # Default dimension parameter to evaluate the model
    dimension = 1

    # Amplitude values
    amplitude = NumericalPoint(dimension, 1.0)

    # Scale values
    scale = NumericalPoint(dimension, 1.0)

    # Covariance model
    myCovarianceModel = ExponentialModel(amplitude, scale)
    print  "myCovarianceModel = " , myCovarianceModel

    # Spectral model
    mySpectralModel = CauchyModel(amplitude, scale)
    print  "mySpectralModel = " , mySpectralModel

    # We build the second order model using covariance and spectral models
    myModel = SecondOrderModel(myCovarianceModel, mySpectralModel)
    print  "myModel = " , myModel

    # Some computations ==> call the sub models methods
    instant = 1.0
    frequency = 0.5

    #
    print "covariance matrix at t = " , instant , " : " , myModel.computeCovariance(instant)
    print "covariance matrix at t = " , -1.0 * instant , " : " , myModel.computeCovariance(-1.0 * instant)
    print "spectral density at f = " , frequency , " : " , myModel.computeSpectralDensity(frequency)

    # Discretize the process on a small time grid
    timeGrid = RegularGrid(0.0, 1.0, 11)
    print  "discretized covariance over the time grid = " , timeGrid , " is "
    print myModel.discretizeCovariance(timeGrid)

except :
    import sys
    print "t_SecondOrderModel_std.py", sys.exc_type, sys.exc_value
