#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    # Compossite test
    RandomGenerator.SetSeed(0)

    # Time grid creation and White Noise
    Tmin = 0.0
    deltaT = 0.1
    steps = 11

    # Initialization of the TimeGrid timeGrid
    timeGrid = RegularGrid(Tmin, deltaT, steps)

    # Creation of the Antecedent
    myARMAProcess = ARMA()
    myARMAProcess.setTimeGrid(timeGrid)
    print  "myAntecedentProcess = " ,  myARMAProcess

    # Creation of a function
    inputDescription = Description(1, "x")
    formula = Description(1, "2 * x + 5.0")
    f = NumericalMathFunction(inputDescription, formula)

    # We build a dynamical function
    myDynamicalFunction =  DynamicalFunction(SpatialFunction(f))

    # finally we get the compsite process
    myCompositeProcess = CompositeProcess(myDynamicalFunction, myARMAProcess)
    print  "myCompositeProcess = " ,  repr(myCompositeProcess)

    # Test realization
    print "One realization= "
    print myCompositeProcess.getRealization()

except :
    import sys
    print "t_CompositeProcess_std.py", sys.exc_type, sys.exc_value
