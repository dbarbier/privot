#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    # TimeGrid parameters
    n = 101
    timeStart = 0.0
    timeStep = 0.1
    timeGrid = RegularGrid(timeStart, timeStep, n)

    # White noise
    whiteNoise = WhiteNoise(Uniform(), timeGrid)

    # Composite process
    process = CompositeProcess(SpatialFunction(NumericalMathFunction("x", "x+2")), whiteNoise)
    # A realization of the process
    timeSeries = process.getRealization()
    sample = timeSeries.getSample()
    
    # Now we build the factory
    factory = BoxCoxFactory()
    
    # Creation of the BoxCoxTransform
    myBoxCox = factory.build(timeSeries)
    
    print "myBoxCox (time-series)=", myBoxCox
    print "myBoxCox (sample)     =", factory.build(sample)
    
    # Creation of the BoxCoxTransform using shift
    shift = NumericalPoint(1, 1.0)
    myBoxCoxShift = factory.build(timeSeries, shift)
    
    print "myBoxCox with shift (time-series)=", myBoxCoxShift
    print "myBoxCox with shift (sample)     =", factory.build(sample, shift)

    # Creation of the BoxCoxTransform using shift with graph
    graph = Graph()
    myBoxCoxShiftGraph = factory.build(timeSeries, shift, graph)

    print "BoxCox graph (time-series)=", graph
    graph.draw("BoxCoxGraph")


except :
    import sys
    print "t_BoxCoxFactory_std.py", sys.exc_type, sys.exc_value
