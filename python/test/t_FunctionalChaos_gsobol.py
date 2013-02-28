#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

def sobol(indice, ai):
    val = 1.0
    for i in range(indice.getSize()):
        val = val * 1.0 / (3.0 * (1.0 + ai[indice[i]])**2.0)
    return val

try :
    # Problem parameters
    dimension = 3

    # Create the Sobol function
    # Reference analytical values
    meanTh = 1.0
    a = NumericalPoint(dimension)
    inputVariables = Description(dimension)
    outputVariables = Description(1)
    outputVariables[0] = "y"
    formula = Description(1)
    formula[0] = "1.0"
    covTh = 1.0
    for i in range(dimension):
        a[i] = 0.5 * i
        covTh = covTh * (1.0 + 1.0 / (3.0 * (1.0 + a[i])**2))
        inputVariables[i] = "xi"+str(i)
        formula[0] = formula[0] + " * ((abs(4.0 * xi" + str(i) + " - 2.0) + " + str(a[i]) + ") / (1.0 + " + str(a[i]) + "))"
    covTh = covTh - 1.0

    model = NumericalMathFunction(inputVariables, outputVariables, formula)
    # Create the input distribution
    marginals = DistributionCollection(dimension)
    for i in range(dimension):
        marginals[i] = Uniform(0.0, 1.0)
    distribution = ComposedDistribution(marginals)

    # Create the orthogonal basis
    polynomialCollection = PolynomialFamilyCollection(dimension)
    for i in range(dimension):
        polynomialCollection[i] = LegendreFactory()
    enumerateFunction = EnumerateFunction(dimension)
    productBasis = OrthogonalProductPolynomialFactory(polynomialCollection, EnumerateFunction(enumerateFunction))

    # Create the adaptive strategy
    # We can choose amongst several strategies
    # First, the most efficient (but more complex!) strategy
    listAdaptiveStrategy = list()
    degree = 4
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
        # Create the projection strategy
        samplingSize = 250
        listProjectionStrategy = list()
        # Monte Carlo sampling
        listProjectionStrategy.append(LeastSquaresStrategy(MonteCarloExperiment(samplingSize)))
        # LHS sampling
        listProjectionStrategy.append(LeastSquaresStrategy(LHSExperiment(samplingSize)))
        # Low Discrepancy sequence
        listProjectionStrategy.append(LeastSquaresStrategy(LowDiscrepancyExperiment(LowDiscrepancySequence(SobolSequence()),samplingSize)))
        for projectionStrategyIndex in range(len(listProjectionStrategy)):
            projectionStrategy = listProjectionStrategy[projectionStrategyIndex]
            # Create the polynomial chaos algorithm
            maximumResidual = 1.0e-10
            algo = FunctionalChaosAlgorithm(model, distribution, adaptiveStrategy, projectionStrategy)
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
            print "relative errors=", relativeErrors

            # Post-process the results
            vector = FunctionalChaosRandomVector(result)
            mean = vector.getMean()[0]
            print "mean=%.8f" % mean, "absolute error=%.8f" % fabs(mean - meanTh)
            variance = vector.getCovariance()[0, 0]
            print "variance=%.8f" % variance, "absolute error=%.8f" % fabs(variance - covTh)
            indices = Indices(1)
            for i in range(dimension):
                indices[0] = i
                value = vector.getSobolIndex(i)
                print "Sobol index", i, "= %.8f" % value, "absolute error=%.8f" % fabs(value - sobol(indices, a) / covTh)
            indices = Indices(2)
            k = 0
            for i in range(dimension):
                indices[0] = i
                for j in range(i+1, dimension):
                    indices[1] = j
                    value = vector.getSobolIndex(indices)
                    print "Sobol index", indices, "=%.8f" % value, "absolute error=%.8f" % fabs(value - sobol(indices, a) / covTh)
                    k = k+1
            indices = Indices(3)
            indices[0] = 0
            indices[1] = 1
            indices[2] = 2
            value = vector.getSobolIndex(indices)
            print "Sobol index", indices, "=%.8f" % value, "absolute error=%.8f" % fabs(value - sobol(indices, a) / covTh)

except :
    import sys
    print "t_FunctionalChaos_gsobol.py", sys.exc_type, sys.exc_value
