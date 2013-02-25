#! /usr/bin/env python

from openturns import *
from math import *

#TESTPREAMBLE()

try :
    dimension = 2
    sampleSize = 1000

    # we create an analytical function
    input = Description(dimension)
    input[0] = "x0"
    input[1] = "x1"

    foutput = Description("f")

    formulas = Description("10+3*x0+x1")

    analytical = NumericalMathFunction(input, foutput, formulas)

    # we create a collection of uniform distributions over [-Pi Pi[
    aCollection = DistributionCollection()
    for i in range(dimension):
        aCollection.add(Normal())

    # we create one distribution object
    aDistribution = ComposedDistribution(aCollection, IndependentCopula(dimension))

    randomVector = RandomVector(aDistribution)
    composite = RandomVector(analytical, randomVector)
    print "composite=", composite

    # we create two input samples for the function
    inputSample = randomVector.getSample(sampleSize)
    outputSample = analytical(inputSample)

    src = CorrelationAnalysis.SRC(inputSample, outputSample)
    print "src=", src
except :
    import sys
    print "t_CorrelationAnalysis_sobol.py", sys.exc_type, sys.exc_value
