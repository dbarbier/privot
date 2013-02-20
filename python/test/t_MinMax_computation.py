#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    # Load the physical model
    inputVariables = Description(4)
    inputVariables[0] = "E"
    inputVariables[1] = "F"
    inputVariables[2] = "L"
    inputVariables[3] = "I"
    outputVariables = Description(1)
    outputVariables[0] = "d"
    formula = Description(1)
    formula[0] = "-F*L^3/(3*E*I)"
    model = NumericalMathFunction(inputVariables, outputVariables, formula)
    inputDimension = model.getInputDimension()
    # Define the reference domaine that will be sampled: an hypercube of side length 2, discretized with 9+2=11 points in each dimension
    levels = NumericalPoint(inputDimension)
    levels[0] = 5.
    levels[1] = 5.
    levels[2] = 5.
    levels[3] = 5.
    myPlane = Box(levels)
    sample = myPlane.generate()
    # Then, scale and move the cube to sample around the point of interrest
    point = NumericalPoint(inputDimension)
    point[0] = 2.1e11
    point[1] = 1e3
    point[2] = 1.5
    point[3] = 2.e-6
    sample.scale(point * 0.2)
    sample.translate(point * 0.9)
    # Compute the model over the sample
    response = model(sample)
    # Compute the min and max values taken by the model
    print "Min=%.6f" % response.getMin()[0]
    print "Max=%.6f" % response.getMax()[0]
except :
    import sys
    print "t_MinMax_computation.py", sys.exc_type, sys.exc_value
