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

"""

if __name__ == "__main__":
    import openturns as ot
    import MultivariateRandomMixture as MV
    import numpy as np
    from MultivariateGaussianCharacteristicFunction import MGCF as mgcf

    N = 2000000

    """
    Test 
    ----

    Linear form ==> Random mixture approach comparison
    """
    collection = ot.DistributionCollection([ot.Normal(), ot.Uniform()])
    matrix = ot.Matrix([[4, 1.4]])
    y = [2.0]
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix, y)
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
    random_mixture = ot.RandomMixture(collection, [4.0, 1.4], 2.0)
    print "RandomMixture distribution"
    print "range = ", random_mixture.getRange()
    print "mean = ", random_mixture.getMean()
    print "cov = ", random_mixture.getCovariance()
    print "sigma = ", random_mixture.getStandardDeviation()
    print "sample :"
    print "min = %s\nmax = %s\nmean = %s, cov = %s" %(sample.getMin(),sample.getMax(), sample.computeMean(), sample.computeCovariance())
    # evaluation of the characteristic function between -10 and 10
    xmin, xmax, dx = -5, 5, 0.5
    x = np.arange(xmin, xmax, dx)
    delta = 0.0
    for value in x:
        c1 = distribution.computeCharacteristicFunction([value])
        c2 = random_mixture.computeCharacteristicFunction(value)
        delta += abs((c1 - c2))**2
    # Variation of characteristic function
    delta /= len(x) * len(y)
    print "delta of characteristic function=%s" %(np.sqrt(delta))