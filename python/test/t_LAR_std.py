#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try :
    #Log.Show( Log.Flags() | Log.INFO )

    # Problem parameters
    dimension = 3
    a = 7.0
    b = 0.1
    # Reference analytical values
    meanTh = a/2
    covTh = (b**2 * pi**8) / 18.0 + (b * pi**4) / 5.0 + (a**2) / 8.0 + 1.0 / 2.0
    sob_1 = [(b * pi**4 / 5.0 + b**2 * pi**8 / 50.0 + 1.0/2.0) / covTh, (a**2 / 8.0) / covTh, 0.0]
    sob_2 = [0.0, (b**2 * pi**8 / 18.0 - b**2 * pi**8 / 50.0) / covTh, 0.0]
    sob_3 = [0.0]
    sob_T1 = [sob_1[0] + sob_2[0] + sob_2[1] + sob_3[0], sob_1[1] + sob_2[0] + sob_2[2] + sob_3[0], sob_1[2] + sob_2[1] + sob_2[2] + sob_3[0]]
    sob_T2 = [sob_2[0] + sob_2[1] + sob_3[0], sob_2[0] + sob_2[2] + sob_3[0], sob_2[1] + sob_2[2] + sob_3[0]]
    sob_T3 = [sob_3[0]]
    # Create the Ishigami function
    inputVariables = Description(dimension)
    inputVariables[0] = "xi1"
    inputVariables[1] = "xi2"
    inputVariables[2] = "xi3"
    outputVariables = Description(1)
    outputVariables[0] = "y"
    formula = Description(1)
    formula[0] = "sin(xi1) + (" + str(a) + ") * (sin(xi2)) ^ 2 + (" + str(b) + ") * xi3^4 * sin(xi1)"
    model = NumericalMathFunction(inputVariables, outputVariables, formula)

    # Create the input distribution
    marginalX = DistributionCollection(dimension)
    for i in range(dimension):
        marginalX[i] = Uniform(-pi, pi)
    distribution = Distribution(ComposedDistribution(marginalX))

    # Create the orthogonal basis
    polynomialCollection = PolynomialFamilyCollection(dimension)
    for i in range(dimension):
        polynomialCollection[i] = LegendreFactory()
    enumerateFunction = LinearEnumerateFunction(dimension)
    productBasis = OrthogonalProductPolynomialFactory(polynomialCollection, enumerateFunction)

    # design experiment
    samplingSize = 75
    experiment = Experiment(LowDiscrepancyExperiment(LowDiscrepancySequence(SobolSequence(dimension)), distribution, samplingSize))

    # iso transfo
    algo = FunctionalChaosAlgorithm(model, distribution, AdaptiveStrategy(FixedStrategy(productBasis, enumerateFunction.getStrataCumulatedCardinal(1))))
    algo.run()
    result = algo.getResult()
    xToU = result.getTransformation()

    # generate samples
    x = experiment.generate()
    u = xToU(x)
    y = model(x)

    # build basis
    degree = 10
    basisSize = enumerateFunction.getStrataCumulatedCardinal(degree)
    psi = Basis()
    for i in range(basisSize):
        psi.add(productBasis.build(i))

    # run algorithm
    factory = BasisSequenceFactory(LAR())
    factory.setVerbose(True)
    print "factory = ", factory

    seq = factory.build( u, y, psi )

    first = 20
    if ( seq.getSize() >= first ):
        print "first ",first," indices = ", seq.getIndices(first-1)

except :
    import sys
    print "t_LAR.py", sys.exc_type, sys.exc_value
