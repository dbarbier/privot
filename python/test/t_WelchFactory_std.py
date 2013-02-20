#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    # Dimension of the input model
    # Size of the TimeGrid
    size = 64
    dimension = 1
    timeGrid = RegularGrid(0.0, 0.1, size)
    amplitude = NumericalPoint(dimension, 2.0)
    scale = NumericalPoint(dimension, 1.0)
    model = ExponentialCauchy(amplitude, scale)
    myProcess = SpectralNormalProcess(model, timeGrid)

    # Create a Process sample
    N = 100
    sample = ProcessSample(myProcess.getSample(N))

    # Filtering Windows
    myFactory = WelchFactory()

    # Build a UserDefinedSpectralModel using the Wellch method
    mySpectralModel = myFactory.build(sample)

    # Get the frequency grid of the model
    myFrequencyGrid = mySpectralModel.getFrequencyGrid()
    for i in range(dimension):
        for j in range(dimension):
            print "Spectre ", i, "-", j
            for k in range(myFrequencyGrid.getN()):
                frequency = myFrequencyGrid.getStart() + k * myFrequencyGrid.getStep()
                estimatedValue = (mySpectralModel.computeSpectralDensity(frequency)[i, j]).real
                modelValue = (model.computeSpectralDensity(frequency)[i, j]).real
                print "Frequency =  %.6f" % frequency, ", evaluation = %.8f" % estimatedValue, " model = %.8f" % modelValue

    # Create a Time Series
    timeSeries = myProcess.getRealization()

    # Build a UserDefinedSpectralModel using the Wellch method
    mySpectralModel2 = myFactory.build(timeSeries)

    # Get the frequency grid of the model
    myFrequencyGrid = mySpectralModel2.getFrequencyGrid()
    for i in range(dimension):
        for j in range(dimension):
            print "Spectre ", i, "-", j
            for k in range(myFrequencyGrid.getN()):
                frequency = myFrequencyGrid.getStart() + k * myFrequencyGrid.getStep()
                estimatedValue = (mySpectralModel2.computeSpectralDensity(frequency)[i, j]).real
                modelValue = (model.computeSpectralDensity(frequency)[i, j]).real
                print "Frequency =  %.6f" %  frequency, ", evaluation = %.8f" % estimatedValue, " model = %.8f" % modelValue

except :
    import sys
    print "t_WelchFactory_std.py", sys.exc_type, sys.exc_value
