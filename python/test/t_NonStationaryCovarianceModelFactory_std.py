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

    size = 10
    timeGrid = RegularGrid(0., 0.1, size)

    # Cauchy model
    model = ExponentialCauchy(amplitude, scale)
    myProcess = TemporalNormalProcess(model, timeGrid)

    # Create a Process sample of size N
    N = 10000
    sample = myProcess.getSample(N)

    # NonStationaryCovarianceModelFactory using default parameter - Factory initiate
    myFactory = NonStationaryCovarianceModelFactory()

    # Build a UserDefinedCovarianceModel using the Wellch method
    myCovarianceModel = myFactory.build(sample)

    # Get the time grid of the model

    for i in range(size):
        t = timeGrid.getValue(i)
        for j in range(size):
            s = timeGrid.getValue(j)
            estimatedValue = myCovarianceModel.computeCovariance(t,s)[0, 0]
            modelValue = model.computeCovariance(t,s)[0, 0]
            print  "Covariance C(" , t , ", " , s , ") : ", " evaluation = " , estimatedValue , " model = " , modelValue

except :
    import sys
    print "t_NonStationaryCovarianceModelFactory_std.py", sys.exc_type, sys.exc_value
