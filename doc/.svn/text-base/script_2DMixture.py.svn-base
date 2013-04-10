# -*- coding: iso-8859-1 -*-
from openturns import *
from otmixmod import *

draw = True 

#####################################################################
# Create a multidimensional sample from a mixture of Normal
dim = 2
size = 20000
coll = DistributionCollection(0)
R = CorrelationMatrix(dim)


# First atom
for i in range(dim-1):
    R[i,i+1] = -0.2
mean = NumericalPoint(dim, -2.2)
sigma = NumericalPoint(dim, 1.2)
d = Distribution(Normal(mean, sigma, R))
coll.add(d)


# Second atom
R = CorrelationMatrix(dim)
for i in range(dim-1):
    R[i,i+1] = 0.1
mean = NumericalPoint(dim, 2.0)
sigma = NumericalPoint(dim, 0.8)
d = Distribution(Normal(mean, sigma, R))
coll.add(d)


# Third atom
mean = NumericalPoint( (-5.0, 5.0) )
sigma = NumericalPoint(dim, 1.4)
R = CorrelationMatrix(dim)
d = Distribution(Normal(mean, sigma, R))
coll.add(d)


coll[0].setWeight(0.5)
coll[1].setWeight(0.25)
coll[2].setWeight(0.25)


# Reference mixture
mixture = Mixture(coll)

# Creation of the numerical Sample from which we will estimate
# the parameters of the mixture.
sample = mixture.getNumericalSample(size)


#####################################################################
# Creation of the mixture factory
myAtomsNumber = 3
myCovModel = Gaussian_pk_L_Dk_A_Dk()

bestLL = -1e100
bestMixture = Mixture()
bestNbClusters = 0
stop = False
nbClusters = 1
while not stop:
    factory = MixtureFactory(nbClusters, myCovModel)
    # Estimation of the parameters of the mixture
    labels = Indices(0)
    logLikelihood = NumericalPoint(0)
    estimatedDistribution = factory.build(sample, labels, logLikelihood)
    stop = logLikelihood[1] <= bestLL
    if not stop:
        bestLL = logLikelihood[1]
        bestNbClusters = nbClusters
        bestMixture = estimatedDistribution
    nbClusters += 1
print "best number of atoms=", bestNbClusters
myAtomsNumber = bestNbClusters
estimatedDistribution = bestMixture
# Some printings to show the result
print "Covariance Model used=",myCovModel.convertToMixmod()

print ""
print "Estimated distribution. Mixture composed of :"
for i in xrange(myAtomsNumber):
    d = estimatedDistribution.getDistributionCollection()[i]
    print i,"-  Mean = ",d.getMean()
    print i,"-  Sigma = ",d.getStandardDeviation()
    print i,"-  CorrelationMatrix = ",d.getCorrelation()
    print i,"-  Weight = ",d.getWeight()
    print ""

# Some drawings
if draw & (sample.getDimension() == 2):
g = estimatedDistribution.drawPDF()
c = Cloud(sample)
c.setColor("red")
c.setPointStyle("bullet")
ctmp = g.getDrawable(0)
g.setDrawable(Drawable(c), 0)
g.add(ctmp)
g.draw("testMixMod")
