#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)


aCollection = DistributionCollection(0);
R = CorrelationMatrix(2)
R[0, 1] = -0.99
aCollection.add( Normal([-1.0, 1.0], [1.0, 1.0], R) )
R[0, 1] = 0.99
aCollection.add( Normal([1.0, 1.0], [1.0, 1.0], R) )

# Instanciate one distribution object
distribution = Mixture(aCollection, NumericalPoint(aCollection.getSize(), 1.0))

# Create a mixture classifier
classifier = MixtureClassifier(distribution)

# Create local experts
experts = Basis()
experts.add(NumericalMathFunction("x", "-x"))
experts.add(NumericalMathFunction("x", "x"))

# Create a mixture of experts
moe = ExpertMixture(experts, classifier)
moeNMF = NumericalMathFunction(moe)

print "Mixture of experts=", moe

# Evaluate the mixture of experts on some points
for i in range(5):
    p = NumericalPoint(1, -0.3 + 0.8 * i / 4.0)
    print "moe   ( %.6g )=" % p[0], moe(p)
    print "moeNMF( %.6g )=" % p[0], moeNMF(p)
