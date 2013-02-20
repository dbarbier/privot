#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    # Uncertain parameters
    distribution = Normal(NumericalPoint(3, 1.0), NumericalPoint(3, 2.0), CorrelationMatrix(3))
    # Model
    inputVar = Description(["x", "y", "z"])
    formulas = Description(["x-1.5*y+2*z"])
    outputVar = Description(["out"])
    f = NumericalMathFunction(inputVar, formulas)
    # Must activate the history mechanism if one want to perform sensitivity analysis
    f.enableHistory()
    # Sampling
    size = 100
    inputSample = distribution.getSample(size)
    outputSample = f(inputSample)
    comparisonOperators = [Less(), LessOrEqual(), Greater(), GreaterOrEqual()]
    threshold = 3.0
    ResourceMap.SetAsUnsignedLong("SimulationSensitivityAnalysis-DefaultSampleMargin", 10)
    for i in range(4):
        algo = SimulationSensitivityAnalysis(inputSample, outputSample, distribution.getIsoProbabilisticTransformation(), comparisonOperators[i], threshold)
        print "algo=", algo
        # Perform the analysis
        print "Mean point in event domain=", algo.computeMeanPointInEventDomain()
        print "Importance factors at ", threshold, " =", algo.computeImportanceFactors()
        print "Importance factors at ", threshold / 2, " =", algo.computeImportanceFactors(threshold / 2)
        importanceFactorsGraph = algo.drawImportanceFactors()
        print "importanceFactorsGraph=", importanceFactorsGraph
        importanceFactorsGraph.draw("importanceFactorsGraphSample" + comparisonOperators[i].getClassName())
        print "postscript=", importanceFactorsGraph.getPostscript()
        print "bitmap=", importanceFactorsGraph.getBitmap()
        # Importance factors evolution on probability scale
        importanceFactorsRangeGraphProbability = algo.drawImportanceFactorsRange()
        print "importanceFactorsRangeGraphProbability=", importanceFactorsRangeGraphProbability
        importanceFactorsRangeGraphProbability.draw("importanceFactorsRangeGraphProbabilitySample" + comparisonOperators[i].getClassName())
        print "postscript=", importanceFactorsRangeGraphProbability.getPostscript()
        print "bitmap=", importanceFactorsRangeGraphProbability.getBitmap()
        # Importance factors evolution on threshold scale
        importanceFactorsRangeGraphThreshold = algo.drawImportanceFactorsRange(False)
        print "importanceFactorsRangeGraphThreshold=", importanceFactorsRangeGraphThreshold
        importanceFactorsRangeGraphThreshold.draw("importanceFactorsRangeGraphThresholdSample" + comparisonOperators[i].getClassName())
        print "postscript=", importanceFactorsRangeGraphThreshold.getPostscript()
        print "bitmap=", importanceFactorsRangeGraphThreshold.getBitmap()

        # Reset the history of the model
        f.clearHistory()

        # Analysis based on an event
        X = RandomVector(distribution)
        Y = RandomVector(f, X)
        event = Event(Y, comparisonOperators[i], threshold)
        # Get a sample of the event to simulate a Monte Carlo analysis. We don't care of the result as the interesting values are stored in the model history
        event.getSample(size)
        algo = SimulationSensitivityAnalysis(event)
        print "algo=", algo
        # Perform the analysis
        print "Mean point in event domain=", algo.computeMeanPointInEventDomain()
        print "Importance factors at threshold ", threshold, " =", algo.computeImportanceFactors()
        print "Importance factors at threshold/2 ", threshold / 2, " =", algo.computeImportanceFactors(threshold / 2)
        importanceFactorsGraph = algo.drawImportanceFactors()
        print "importanceFactorsGraph=", importanceFactorsGraph
        importanceFactorsGraph.draw("importanceFactorsGraphEvent" + comparisonOperators[i].getClassName())
        print "postscript=", importanceFactorsGraph.getPostscript()
        print "bitmap=", importanceFactorsGraph.getBitmap()
        # Importance factors evolution on probability scale
        importanceFactorsRangeGraphProbability = algo.drawImportanceFactorsRange()
        print "importanceFactorsRangeGraphProbability=", importanceFactorsRangeGraphProbability
        importanceFactorsRangeGraphProbability.draw("importanceFactorsRangeGraphProbabilityEvent" + comparisonOperators[i].getClassName())
        print "postscript=", importanceFactorsRangeGraphProbability.getPostscript()
        print "bitmap=", importanceFactorsRangeGraphProbability.getBitmap()
        # Importance factors evolution on threshold scale
        importanceFactorsRangeGraphThreshold = algo.drawImportanceFactorsRange(False)
        print "importanceFactorsRangeGraphThreshold=", importanceFactorsRangeGraphThreshold
        importanceFactorsRangeGraphThreshold.draw("importanceFactorsRangeGraphThresholdEvent" + comparisonOperators[i].getClassName())
        print "postscript=", importanceFactorsRangeGraphThreshold.getPostscript()
        print "bitmap=", importanceFactorsRangeGraphThreshold.getBitmap()

except  :
    import sys
    print "t_SimulationSensitivityAnalysis_std.py", sys.exc_type, sys.exc_value
