#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
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
    marginals = DistributionCollection(dimension)
    marginals[0] = Uniform(-pi, pi)
    marginals[1] = Uniform(-pi, pi)
    marginals[2] = Uniform(-pi, pi)
    distribution = ComposedDistribution(marginals)

    # Create the orthogonal basis
    polynomialCollection = PolynomialFamilyCollection(dimension)
    polynomialCollection[0] = OrthogonalUniVariatePolynomialFamily(LegendreFactory())
    polynomialCollection[1] = OrthogonalUniVariatePolynomialFamily(LegendreFactory())
    polynomialCollection[2] = OrthogonalUniVariatePolynomialFamily(LegendreFactory())
    enumerateFunction = EnumerateFunction(dimension)
    productBasis = OrthogonalBasis(OrthogonalProductPolynomialFactory(polynomialCollection, enumerateFunction))

    # Create the projection strategy
    samplingSize = 250
    listProjectionStrategy = list()
    # Monte Carlo sampling
    inputSample = LowDiscrepancyExperiment(LowDiscrepancySequence(SobolSequence()), distribution, samplingSize).generate()
    outputSample = model(inputSample)
    # From here, the model is no more needed
    listProjectionStrategy.append(LeastSquaresStrategy())
    listProjectionStrategy.append(LeastSquaresStrategy(LeastSquaresMetaModelSelectionFactory ( LAR(), CorrectedLeaveOneOut())))
    listProjectionStrategy.append(IntegrationStrategy())
    # Create the adaptive strategy
    # We can choose amongst several strategies
    # First, the most efficient (but more complex!) strategy
    degree = 6
    listAdaptiveStrategy = list()
    indexMax = enumerateFunction.getStrataCumulatedCardinal(degree)
    basisDimension = enumerateFunction.getStrataCumulatedCardinal(divmod(degree, 2)[0])
    threshold = 1.0e-6
    listAdaptiveStrategy.append(CleaningStrategy(productBasis, indexMax, basisDimension, threshold, False))
    # Second, the most used (and most basic!) strategy
    listAdaptiveStrategy.append(FixedStrategy(productBasis, enumerateFunction.getStrataCumulatedCardinal(degree)))
    # Third, a slight enhancement with respect to the basic strategy
    listAdaptiveStrategy.append(SequentialStrategy(productBasis, enumerateFunction.getStrataCumulatedCardinal(divmod(degree, 2)[0]), False))

    for adaptiveStrategyIndex in range(len(listAdaptiveStrategy)):
        adaptiveStrategy = listAdaptiveStrategy[adaptiveStrategyIndex]
        for projectionStrategyIndex in range(len(listProjectionStrategy)):
            projectionStrategy = listProjectionStrategy[projectionStrategyIndex]
            # Create the polynomial chaos algorithm
            maximumResidual = 1.0e-10
            algo = FunctionalChaosAlgorithm(inputSample, outputSample, distribution, adaptiveStrategy, projectionStrategy)
            algo.setMaximumResidual(maximumResidual)
            RandomGenerator.SetSeed(0)
            algo.run()

            # Examine the results
            result = FunctionalChaosResult(algo.getResult())
            print "###################################"
            print AdaptiveStrategy(adaptiveStrategy)
            print ProjectionStrategy(projectionStrategy)
            #print "coefficients=", result.getCoefficients()
            residuals = result.getResiduals()
            print "residuals=", residuals
            relativeErrors = result.getRelativeErrors()
            print "relativeErrors=", relativeErrors

            # Post-process the results
            vector = FunctionalChaosRandomVector(result)
            mean = vector.getMean()[0]
            print "mean=%.8f" % mean, "absolute error=%.10f" % fabs(mean - meanTh)
            variance = vector.getCovariance()[0, 0]
            print "variance=%.8f" % variance, "absolute error=%.10f" % fabs(variance - covTh)
            for i in range(dimension):
                value = vector.getSobolIndex(i)
                print "Sobol index", i, "= %.8f" % value, "absolute error=%.10f" % fabs(value - sob_1[i])
            indices = Indices(2)
            k = 0
            for i in range(dimension):
                indices[0] = i
                for j in range(i+1, dimension):
                    indices[1] = j
                    value = vector.getSobolIndex(indices)
                    print "Sobol index", indices, "=%.8f" % value, "absolute error=%.10f" % fabs(value - sob_2[k])
                    k = k+1
            indices = Indices(3)
            indices[0] = 0
            indices[1] = 1
            indices[2] = 2
            value = vector.getSobolIndex(indices)
            print "Sobol index", indices, "=%.8f" % value, "absolute error=%.10f" % fabs(value - sob_3[0])
            for i in range(dimension):
                value = vector.getSobolTotalIndex(i)
                print "Sobol total index", i, "=%.8f" % value, "absolute error=%.10f" % fabs(value - sob_T1[i])
            indices = Indices(2)
            k = 0
            for i in range(dimension):
                indices[0] = i
                for j in range(i+1, dimension):
                    indices[1] = j
                    value = vector.getSobolIndex(indices)
                    print "Sobol total index", indices, "=%.8f" % value, "absolute error=%.10f" % fabs(value - sob_2[k])
                    k = k+1
            indices = Indices(3)
            indices[0] = 0
            indices[1] = 1
            indices[2] = 2
            value = vector.getSobolTotalIndex(indices)
            print "Sobol total index ", indices, "=%.8f" % value, "absolute error=%.10f" % fabs(value - sob_3[0])

except :
    import sys
    print "t_FunctionalChaos_ishigami.py", sys.exc_type, sys.exc_value
