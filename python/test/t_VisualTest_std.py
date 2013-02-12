#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    # Generate a Normal sample
    normal = Normal(1)
    size = 100
    sample = normal.getSample(size)
    sampleCDF = VisualTest.DrawEmpiricalCDF(sample, sample.getMin()[0] - 1.0, sample.getMax()[0] + 1.0)
    sampleCDF.draw("sampleCDF", 640, 480)
    print "bitmap = ", sampleCDF.getBitmap()
    print "postscript = ", sampleCDF.getPostscript()

    sampleCDF = VisualTest.DrawEmpiricalCDF(sample, -0.5, 0.5)
    sampleCDF.draw("sampleCDFZoom", 640, 480)
    print "bitmap = ", sampleCDF.getBitmap()
    print "postscript = ", sampleCDF.getPostscript()


    # Histogram tests
    normal = Normal(1)
    size = 100
    sample = normal.getSample(size)
    sampleHist = VisualTest.DrawHistogram(sample, 10)
    sampleHist.draw("sampleHist", 640, 480)
    print "bitmap = ", sampleHist.getBitmap()
    print "postscript = ", sampleHist.getPostscript()

    sampleHist = VisualTest.DrawHistogram(sample)
    sampleHist.draw("sampleHistOpt", 640, 480)
    print "bitmap = ", sampleHist.getBitmap()
    print "postscript = ", sampleHist.getPostscript()

    # QQPlot tests
    size = 100
    normal = Normal(1)
    sample = normal.getSample(size)
    sample2 = Gamma(3.0, 4.0, 0.0).getSample(size)
    twoSamplesQQPlot = VisualTest.DrawQQplot(sample, sample2)
    twoSamplesQQPlot.draw("twoSamplesQQPlot", 640, 480)
    print "bitmap = ", twoSamplesQQPlot.getBitmap()
    print "postscript = ", twoSamplesQQPlot.getPostscript()

    sampleDistributionQQPlot = VisualTest.DrawQQplot(sample, normal)
    sampleDistributionQQPlot.draw("sampleDistributionQQPlot", 640, 480)
    print "bitmap = ", sampleDistributionQQPlot.getBitmap()
    print "postscript = ", sampleDistributionQQPlot.getPostscript()

    # HenryLine test
    size = 100
    normal = Normal(1)
    sample = normal.getSample(size)
    henryPlot = VisualTest.DrawHenryLine(sample)
    henryPlot.draw("HenryPlot", 640, 480)
    print "bitmap = ", henryPlot.getBitmap()
    print "postscript = ", henryPlot.getPostscript()

    # Clouds tests
    dimension = (2)
    R = CorrelationMatrix(dimension)
    R[0, 1] = 0.8
    distribution = Normal(NumericalPoint(dimension, 3.0), NumericalPoint(dimension, 2.0), R)
    size = 100
    sample2D = distribution.getSample(size)
    firstSample = NumericalSample(size, 1)
    secondSample = NumericalSample(size, 1)
    for i in range(size):
        firstSample[i] = NumericalPoint(1, sample2D[i, 0])
        secondSample[i] = NumericalPoint(1, sample2D[i, 1])

    sampleSampleClouds = VisualTest.DrawClouds(sample2D, Normal(NumericalPoint(dimension, 2.0), NumericalPoint(dimension, 3.0), R).getSample(divmod(size, 2)[0]))
    sampleSampleClouds.draw("sampleSampleClouds", 640, 480)
    print "bitmap = ", sampleSampleClouds.getBitmap()
    print "postscript = ", sampleSampleClouds.getPostscript()

    sampleDistributionClouds = VisualTest.DrawClouds(sample2D, Normal(NumericalPoint(dimension, 2.5), NumericalPoint(dimension, 1.0), R))
    sampleDistributionClouds.draw("sampleDistributionClouds", 640, 480)
    print "bitmap = ", sampleDistributionClouds.getBitmap()
    print "postscript = ", sampleDistributionClouds.getPostscript()

    # LinearModel tests
    dimension = 2
    R = CorrelationMatrix(dimension)
    R[0, 1] = 0.8
    distribution = Normal(NumericalPoint(dimension, 3.0), NumericalPoint(dimension, 2.0), R)
    size = 100
    sample2D = distribution.getSample(size)
    firstSample = NumericalSample(size, 1)
    secondSample = NumericalSample(size, 1)
    for i in range(size):
        firstSample[i] = NumericalPoint(1, sample2D[i, 0])
        secondSample[i] = NumericalPoint(1, sample2D[i, 1])

    lmtest = LinearModelFactory().build(firstSample, secondSample)
    drawLinearModelVTest = VisualTest.DrawLinearModel(firstSample, secondSample, lmtest)
    drawLinearModelVTest.draw("LinearModelV", 640, 480)
    print "bitmap = ", drawLinearModelVTest.getBitmap()
    print "postscript = ", drawLinearModelVTest.getPostscript()

    drawLinearModelResidualTest = VisualTest.DrawLinearModelResidual(firstSample, secondSample, lmtest)
    drawLinearModelResidualTest.draw("LinearModelR", 640, 480)
    print "bitmap = ", drawLinearModelResidualTest.getBitmap()
    print "postscript = ", drawLinearModelResidualTest.getPostscript()

    # CobWeb tests
    size = 100
    inputDimension = 6
    inputSample = Normal(inputDimension).getSample(size)
    inputVar = Description(inputDimension)
    for i in range(inputDimension):
        inputVar[i] = "X" + str(i)
    formula = Description(1)
    expression = ""
    for i in range(inputDimension):
        if i > 0:
            expression += "+"
        expression += "cos(" + str(i + 1) + "*" + inputVar[i] + ")"
    formula[0] = expression
    outputVar = Description(1)
    outputVar[0] = "y"
    model = NumericalMathFunction(inputVar, outputVar, formula)
    outputSample = model(inputSample)
    cobwebValue = VisualTest.DrawCobWeb(inputSample, outputSample, 2.5, 3.0, "red", False)
    cobwebValue.draw("cobwebValue", 640, 480)
    print "bitmap = ", cobwebValue.getBitmap()
    print "postscript = ", cobwebValue.getPostscript()
    cobwebQuantile = VisualTest.DrawCobWeb(inputSample, outputSample, 0.7, 0.9, "red", False)
    cobwebQuantile.draw("cobwebQuantile", 640, 480)
    print "bitmap = ", cobwebQuantile.getBitmap()
    print "postscript = ", cobwebQuantile.getPostscript()

    # KendallPlot tests
    size = 100
    copula1 = FrankCopula(1.5)
    copula2 = GumbelCopula(4.5)
    sample1 = copula1.getSample(size)
    sample1.setName("data 1")
    sample2 = copula2.getSample(size)
    sample2.setName("data 2")
    kendallPlot1 = VisualTest.DrawKendallPlot(sample1, copula2)
    kendallPlot1.draw("KendallPlot1", 640, 480)
    print "bitmap = ", kendallPlot1.getBitmap()
    print "postscript = ", kendallPlot1.getPostscript()
    kendallPlot2 = VisualTest.DrawKendallPlot(sample2, sample1)
    kendallPlot2.draw("KendallPlot2", 640, 480)
    print "bitmap = ", kendallPlot2.getBitmap()
    print "postscript = ", kendallPlot2.getPostscript()

except :
    import sys
    print "t_VisualTest_std.py", sys.exc_type, sys.exc_value
