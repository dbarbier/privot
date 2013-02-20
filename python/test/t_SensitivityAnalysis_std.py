#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    RandomGenerator.SetSeed(0)
    inputDimension = 3
    outputDimension = 1

    inputName = Description(inputDimension)
    inputName[0] = "X1"
    inputName[1] = "X2"
    inputName[2] = "X3"
    outputName = Description(outputDimension)
    outputName[0] = "Y"
    formula = Description(outputDimension)
    formula[0] = "sin(_pi*X1)+7*sin(_pi*X2)*sin(_pi*X2)+0.1*((_pi*X3)*(_pi*X3)*(_pi*X3)*(_pi*X3))*sin(_pi*X1)"

    model = NumericalMathFunction(inputName, outputName, formula)

    marginals = DistributionCollection()
    marginals.add(Uniform(-1.0,1.0))
    #marginals[0].setDescription("Marginal 1");
    marginals.add(Uniform(-1.0,1.0))
    #marginals[0].setDescription("Marginal 2");
    marginals.add(Uniform(-1.0,1.0))
    #marginals[0].setDescription("Marginal 3");
    maDistribution = ComposedDistribution(marginals, IndependentCopula(inputDimension))

    size = 10000
    sample1 = maDistribution.getSample(size)
    sample2 = maDistribution.getSample(size)

    #for i in range(size):
        #for j in range(dimension):
            #sample1[i, j] = (i-0.75)/size
            #sample2[i, j] = (i-0.25)/size


    sensitivityAnalysis = SensitivityAnalysis(sample1, sample2, model)
    sensitivityAnalysis.setBlockSize(17)
    secondOrderIndices = sensitivityAnalysis.getSecondOrderIndices()
    firstOrderIndices = sensitivityAnalysis.getFirstOrderIndices()
    totalOrderIndices = sensitivityAnalysis.getTotalOrderIndices()

    print "First order Sobol indice of Y|X1 = %.6f" % firstOrderIndices[0]
    print "Total order Sobol indice of Y|X3 = %.6f" % totalOrderIndices[2]
    print "Second order Sobol indice of Y|X1,X3 = %.6f" % secondOrderIndices[0,2]
except :
    import sys
    print "t_SensitivityAnalysis_std.py", sys.exc_type, sys.exc_value
