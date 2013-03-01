#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try :
    #Log.Show( Log.Flags() | Log.INFO )

      # Problem parameters
    dimension = 8

    # Create the Ishigami function
    # Reference analytical values
    meanTh = 1.0
    covTh = 1.0
    a = NumericalPoint(dimension)
    a[0] = 1.0
    a[1] = 2.0
    a[2] = 5.0
    a[3] = 10.0
    a[4] = 20.0
    a[5] = 50.0
    a[6] = 100.0
    a[7] = 500.0
    inputVariables = Description(dimension)
    outputVariables = Description(1)
    outputVariables[0] = "y"
    formula = Description(1)
    formula[0] = "1.0"
    for i in range(dimension):
        covTh = covTh * (1.0 + 1.0 / (3.0 * (1.0 + a[i])**2))
        inputVariables[i] = "xi"+str(i)
        formula[0] = formula[0] + " * ((abs(4.0 * xi" + str(i) + " - 2.0) + " + str(a[i]) + ") / (1.0 + " + str(a[i]) + "))"
    covTh = covTh - 1.0

    model = NumericalMathFunction(inputVariables, outputVariables, formula)

    # Create the input distribution
    marginalX = DistributionCollection(dimension)
    for i in range(dimension):
        marginalX[i] = Uniform(0, 1.0)
    distribution = ComposedDistribution(marginalX)

    # Create the orthogonal basis
    polynomialCollection = PolynomialFamilyCollection(dimension)
    for i in range(dimension):
        polynomialCollection[i] = OrthogonalUniVariatePolynomialFamily(LegendreFactory())
    q = 0.4
    enumerateFunction = HyperbolicAnisotropicEnumerateFunction(dimension, q)
    productBasis = OrthogonalBasis(OrthogonalProductPolynomialFactory(polynomialCollection, enumerateFunction))


    # design experiment
    samplingSize = 100

    # build basis
    basisSize = 917

    listFittingAlgorithm = list()
    listFittingAlgorithm.append(KFold())
    listFittingAlgorithm.append(CorrectedLeaveOneOut())

    for fittingAlgorithmIndex in range(len(listFittingAlgorithm)):
        fittingAlgorithm = listFittingAlgorithm[fittingAlgorithmIndex]
        adaptiveStrategy = FixedStrategy( productBasis, basisSize )
        projectionStrategy = LeastSquaresStrategy( LowDiscrepancyExperiment( SobolSequence(dimension), samplingSize), LeastSquaresMetaModelSelectionFactory ( LAR(), fittingAlgorithm) );
        algo = FunctionalChaosAlgorithm(model, distribution, adaptiveStrategy, projectionStrategy)
        RandomGenerator.SetSeed(0)
        algo.run()

        result = algo.getResult()
        print "coeffs = ", result.getCoefficients()
        print "residuals = ", result.getResiduals()
        print "relative errors = ", result.getRelativeErrors()



except :
    import sys
    print "t_LeastSquaresMetaModelSelection_std.py", sys.exc_type, sys.exc_value
