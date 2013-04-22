#                                               -*- Python -*-
#
#  @file  test_2D_1D.py
#  @brief MultivariateRandomMixture validation tests
#
#  Copyright (C) 2013 EADS IW France
#
#  Author(s) :  Denis Barbier, IMACS
#               Sofiane Haddad, IMACS
#
#  This program is free software; you can redistribute it and/or
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  along with this library.  If not, see <http://www.gnu.org/licenses/>.
#

"""
    Tests of MultivariateRandomMixture
    ===================================
    R^2-->R case - comparison with a mixture Test for the
    validation of the PythonMultivariateRandomMixture class
    Non gaussian behaviour ==> validation of result

"""

if __name__ == "__main__":
    import openturns as ot
    import MultivariateRandomMixture as MV
    import numpy as np
    from MultivariateGaussianCharacteristicFunction import MGCF as mgcf
    import time

    N = 2000
    maxSize = 0
    blockMin = 3
    blockMax = 16

    """
    Test
    ----

    Linear form ==> Random mixture approach comparison
    """
    collection = ot.DistributionCollection([ot.Uniform(-50, -35), ot.Uniform(35, 50)])
    matrix = ot.Matrix([2 * [1.0]])
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
    random_mixture.setAlpha(distribution.getAlpha())
    random_mixture.setBeta(distribution.getBeta())
    random_mixture.setReferenceBandwidth(distribution.getReferenceBandwidth()[0])

    #distribution.setMaxSize(maxSize)
    distribution.setBlockMin(blockMin)
    distribution.setBlockMax(blockMax)

    #random_mixture.setMaxSize(maxSize)
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
    xmin, xmax, dx = -5.0, 5.0, 0.5
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
