#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    # Dimension of the input model
    # Size of the TimeGrid
    # dimension parameter
    dimension = 1

    # Amplitude values
    amplitude = NumericalPoint(dimension, 1.00)

    # Scale values
    scale = NumericalPoint(dimension, 1.0)

    size = 128
    timeGrid = RegularGrid(0., 0.1, size)

    # Cauchy model
    model = ExponentialCauchy(amplitude, scale)
    myProcess = SpectralNormalProcess(model, timeGrid)

    # Create a sample of size N = 1000
    sample = myProcess.getSample(1000)

    # StationaryCovarianceModelFactory using default parameter - Factory initiate
    myFactory = StationaryCovarianceModelFactory()

    # Build a UserDefinedCovarianceModel using the Wellch method
    myCovarianceModel = myFactory.build(sample)
    tg = myCovarianceModel.getTimeGrid()

    # Get the time grid of the model
    for i in range(tg.getN()):
        t = tg.getValue(i)
        estimatedValue = myCovarianceModel.computeCovariance(t)[0, 0]
        modelValue = model.computeCovariance(t)[0, 0]
        print  "Covariance C(" , t , ") : ", " evaluation = " , estimatedValue , " model = " , modelValue

except :
    import sys
    print "t_StationaryCovarianceModelFactory_std.py", sys.exc_type, sys.exc_value
