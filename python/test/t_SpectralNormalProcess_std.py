#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    # Default dimension parameter to evaluate the model
    defaultDimension = 3

    # Amplitude values
    amplitude = NumericalPoint(defaultDimension, 1.0)
    # Scale values
    scale = NumericalPoint(defaultDimension, 1.0)

    # Second order model with parameters
    myModel = ExponentialCauchy(amplitude, scale)

    # checking the copy-cast*/
    mySecondOrderModel = SecondOrderModel(myModel)

    points = 8
    tMin = 0.0
    tStep = 1.0 / (points - 1)

    # RegularGrid --> Build list of frequencies using the RegularGrid
    myTimeGrid = RegularGrid(tMin, tStep, points)

    mySpectralProcess0 = SpectralNormalProcess(myModel, myTimeGrid)

    print "mySpectralProcess0 = ", mySpectralProcess0
    print "Realization = ", mySpectralProcess0.getRealization()

    # Constructor using maximalFrequency value and size of discretization
    maximalFrequency = 10.0
    mySpectralProcess1 = SpectralNormalProcess(myModel, maximalFrequency, points)
    tg = RegularGrid(mySpectralProcess1.getTimeGrid())

    print "mySpectralProcess1 = ", mySpectralProcess1
    print "Realization = ", mySpectralProcess1.getRealization()

    # Second order model with parameters
    mySpecModel = CauchyModel(amplitude, scale)
    print "mySpecModel = ", mySpecModel

    mySpectralProcess2 = SpectralNormalProcess(mySpecModel, myTimeGrid)
    print "mySpectralProcess2 = ", mySpectralProcess2
    print "Realization = ", mySpectralProcess2.getRealization()
    mySpectralProcess3 = SpectralNormalProcess(mySpecModel, maximalFrequency, points)
    print "mySpectralProcess3 = ", mySpectralProcess3
    print "Realization = ", mySpectralProcess3.getRealization()

except :
    import sys
    print "t_SpectralNormalProcess_std.py", sys.exc_type, sys.exc_value
