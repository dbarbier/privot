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
    myDefautModel = CauchyModel()
    print  "myDefautModel = " , myDefautModel

    # Second order model with parameters
    myModel = CauchyModel(amplitude, scale)
    print  "myModel = " , myModel

    frequencyValue = 1.0
    print  "spectral density matrix at f = " , frequencyValue, " : " , myModel.computeSpectralDensity(frequencyValue)


    # Evaluation at time higher to check the decrease of the cauchy values
    frequencyValueHigh = 10.0
    print  "spectral density matrix at f = "  , frequencyValueHigh , " : " , myModel.computeSpectralDensity(frequencyValueHigh)

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
    mySecondOrderModel = SpectralModel(CauchyModel(amplitude, scale, spatialCorrelation))
    print "mySecondOrderModel = " , mySecondOrderModel
    # checking the cast

    # Second order model  - dimension 10
    myHighModel = CauchyModel(amplitude, scale, spatialCorrelation)
    print  "myHighModel = " , myHighModel
    print  "spectral density matrix at f = " , frequencyValue , " : " , myModel.computeSpectralDensity(frequencyValue)
    print  "spectral density matrix at f = "  , frequencyValueHigh , " : " , myModel.computeSpectralDensity(frequencyValueHigh)

except :
    import sys
    print "t_CauchyModel_std.py", sys.exc_type, sys.exc_value
