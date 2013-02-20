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

    tmin = 0.0
    timeStep  = 0.1
    steps = 11

    # Build of the timeGrid
    myTimeGrid = RegularGrid(tmin, timeStep, steps)

    # Creation using a second order model
    # Second order model with parameters
    myModel = SecondOrderModel(ExponentialCauchy(amplitude, scale))
    print  "myModel = " ,  myModel
    myProcess = TemporalNormalProcess(myModel, myTimeGrid)
    print  "myProcess = " ,  repr(myProcess)

    size = 100
    print "mean over", size, "realizations = "
    print myProcess.getSample(size).computeMean()

    # Creation using a stationary covariance model
    # Second order model with parameters
    myCovModel = ExponentialModel(amplitude, scale)
    print  "myCovModel = " ,  myCovModel
    myCovProcess = TemporalNormalProcess(myCovModel, myTimeGrid)
    print  "myCovProcess = " ,  repr(myCovProcess)

    size = 100
    print "mean over", size, "realizations = "
    print myCovProcess.getSample(size).computeMean()

except :
    import sys
    print "t_TemporalNormalProcess_std.py", sys.exc_type, sys.exc_value
