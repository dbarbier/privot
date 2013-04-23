

import openturns as ot
import MultivariateRandomMixture as MV
import numpy as np
import time

N = 100
maxSize = 0
blockMin = 3
blockMax = 16


collection = ot.DistributionCollection(50 * [ot.Normal(1,5)] + 21 * [ot.Normal(0,7)] + 30 * [ot.Uniform()])
matrix = ot.Matrix([101 * [1.0]])
distribution = MV.PythonMultivariateRandomMixture(collection, matrix)

interval = distribution.getRange()
mean = distribution.getMean()
cov = distribution.getCovariance()
sigma = distribution.getStandardDeviation()
sample = distribution.getSample(N)
print "MultivariateRandomMixture distribution"
print "range = ", interval
print "mean = ", mean
print "cov = ", cov
print "sigma = ", sigma
# Equivalent random mixture
random_mixture = ot.RandomMixture(collection)
# same attributs
random_mixture.setAlpha(distribution.getAlpha())
random_mixture.setBeta(distribution.getBeta())
random_mixture.setReferenceBandwidth(distribution.getReferenceBandwidth()[0])

distribution.setBlockMin(blockMin)
distribution.setBlockMax(blockMax)

random_mixture.setBlockMin(blockMin)
random_mixture.setBlockMax(blockMax)

print "RandomMixture distribution"
print "range = ", random_mixture.getRange()
print "mean = ", random_mixture.getMean()
print "cov = ", random_mixture.getCovariance()
print "sigma = ", random_mixture.getStandardDeviation()
print "sample :"
print "min = %s\nmax = %s\nmean = %s, cov = %s" %(sample.getMin(),sample.getMax(), sample.computeMean(), sample.computeCovariance())
# evaluation of the characteristic function between xmin and xmax
xmin, xmax, dx = -20.0, 20.0, 1.0
x = np.arange(xmin, xmax + dx, dx)
delta = 0.0
pdf = 0.0
t1 = []
t2 = []
for value in x:
    tic = time.time()
    c1 = distribution.computePDF([value])
    toc = time.time()
    dt1 = toc - tic
    t1.append(dt1)
    tic = time.time()
    c2 = random_mixture.computePDF(value)
    toc = time.time()
    dt2 = toc - tic
    t2.append(dt2)
    print dt1, dt2, dt1/dt2
    print "pdf eps : dist=%e, rm=%e"%(distribution.pdfEpsilon_,  random_mixture.getPDFEpsilon())
    print "values comparison : dist_pdf=%s, rm_pdf=%s, error=%s" %(c1, c2, (c1 - c2)/c2)


size = len(t1)
import matplotlib.pylab as plt
plt.loglog(x, t1, 'r', label = 'MV')
plt.loglog(x, t2, 'b', label = 'RM')
plt.legend()
plt.ion()
plt.show()
