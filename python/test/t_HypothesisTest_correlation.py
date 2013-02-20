#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    dim = 10
    R = CorrelationMatrix(dim)
    for i in range(dim) :
        for j in range(i) :
            R[i, j] = (i + j + 1.0) / (2.0 * dim)
    mean = NumericalPoint(dim, 2.0)
    sigma = NumericalPoint(dim, 3.0)
    distribution = Normal(mean, sigma, R)

    size = 100
    sample = distribution.getSample(size)
    sampleX = NumericalSample(size, dim - 1)
    sampleY = NumericalSample(size, 1)
    for i in range(size) :
        sampleY[i, 0] = sample[i, 0]
        for j in range(1,dim) :
            sampleX[i, j - 1] = sample[i, j]

    sampleZ = NumericalSample(size, 1)
    for i in range(size) :
        sampleZ[i, 0] = sampleY[i, 0] * sampleY[i, 0]

    selection = Indices(5)
    for i in range(5) :
        selection[i] = i
    print "selection = ", selection

# PartialPearson : Independence Pearson test between 2 samples : firstSample of dimension n and secondSample of dimension 1. If firstSample[i] is the numerical sample extracted from firstSample (ith coordinate of each point of the numerical sample), PartialPearson performs the Independence Pearson test simultaneously on firstSample[i] and secondSample, for i in the selection. For all i, it is supposed that the couple (firstSample[i] and secondSample) is issued from a gaussian  vector.
# Probability of the H0 reject zone : 1-0.90

    print "PartialPearsonXY=", HypothesisTest().PartialPearson(sampleX, sampleY, selection, 0.90)

    selection2 = Indices(1, 0)
    sampleX0=NumericalSample(size, 1)
    for i in range(size) :
        sampleX0[i, 0] = sampleX[i, 0]

# The three tests must be equal
    print "PartialPearsonX0Y=", HypothesisTest().PartialPearson(sampleX, sampleY, selection2, 0.90)
    print "PearsonX0Y=", HypothesisTest().Pearson(sampleX0, sampleY, 0.90)
    print "FullPearsonX0Y=", HypothesisTest().FullPearson(sampleX0, sampleY, 0.90)


# FullPearson : Independence Pearson test between 2 samples : firstSample of dimension n and secondSample of dimension 1. If firstSample[i] is the numerical sample extracted from firstSample (ith coordinate of each point of the numerical sample), FullPearson performs the Independence Pearson test simultaneously on firstSample[i] and secondSample. For all i, it is supposed that the couple (firstSample[i] and secondSample) is issued from a gaussian  vector.
# Probability of the H0 reject zone : 1-0.90

    print "FullPearsonXY=", HypothesisTest().FullPearson(sampleX, sampleY, 0.90)
    print "FullPearsonYY=", HypothesisTest().FullPearson(sampleY, sampleY, 0.90)

# PartialSpearman test between 2 samples : firstSample of dimension n and secondSample of dimension 1. If firstSample[i] is the numerical sample extracted from firstSample (ith coordinate of each point of the numerical sample), PartialSpearman performs the Independence Spearman test simultaneously on firstSample[i] and secondSample, for i in the selection.
# Probability of the H0 reject zone : 1-0.90

# The three tests must be equal
    print "PartialSpearmanX0Y=", HypothesisTest().PartialSpearman(sampleX, sampleY, selection2, 0.90)
    print "SpearmanX0Y=", HypothesisTest().Spearman(sampleX0, sampleY, 0.90)
    print "FullSpearmanX0Y=", HypothesisTest().FullSpearman(sampleX0, sampleY, 0.90)

    print "PartialSpearmanXY=", HypothesisTest().PartialSpearman(sampleX, sampleY, selection, 0.90)

# FullSpearman : Spearman test between 2 samples : firstSample of dimension n and secondSample of dimension 1. If firstSample[i] is the numerical sample extracted from firstSample (ith coordinate of each point of the numerical sample), FullSpearman performs the Independence Spearman test simultaneously on all firstSample[i] and secondSample.
# Probability of the H0 reject zone : 1-0.90

    print "FullSpearmanYZ=", HypothesisTest().FullSpearman(sampleY, sampleZ, 0.90)
    print "FullSpearmanYY=", HypothesisTest().FullSpearman(sampleY, sampleY, 0.90)

# Regression test between 2 samples : firstSample of dimension n and secondSample of dimension 1. If firstSample[i] is the numerical sample extracted from firstSample (ith coordinate of each point of the numerical sample), PartialRegression performs the Regression test simultaneously on all firstSample[i] and secondSample, for i in the selection. The Regression test tests ifthe regression model between two scalar numerical samples is significant. It is based on the deviation analysis of the regression. The Fisher distribution is used.

# The two tests must be equal
    print "PartialRegressionX0Y=", HypothesisTest().PartialRegression(sampleX, sampleY, selection2, 0.90)
    print "FullRegressionX0Y=", HypothesisTest().FullRegression(sampleX0, sampleY, 0.90)

    print "PartialRegressionXY=", HypothesisTest().PartialRegression(sampleX, sampleY, selection, 0.90)

# Regression test between 2 samples : firstSample of dimension n and secondSample of dimension 1. If firstSample[i] is the numerical sample extracted from firstSample (ith coordinate of each point of the numerical sample), FullRegression performs the Regression test simultaneously on all firstSample[i] and secondSample. The Regression test tests if the regression model between two scalar numerical samples is significant. It is based on the deviation analysis of the regression. The Fisher distribution is used.

    print "FullRegressionXZ=", HypothesisTest().FullRegression(sampleX, sampleY, 0.90)
    print "FullRegressionZZ=", HypothesisTest().FullRegression(sampleZ, sampleZ, 0.90)


except :
    import sys
    print "t_HypothesisTest_correlation.py", sys.exc_type, sys.exc_value
