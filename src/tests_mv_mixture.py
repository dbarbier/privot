#                                               -*- Python -*-
#
#  @file  tests_mv_mixture.py
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
    Tests for the validation of the PythonMultivariateRandomMixture class
    1) Test 1: R^2-->R case - comparison with a mixture
    1) Test 2: R^2-->R^2 case - Diagonal matrix


    Example
    --------
    >>> import openturns as ot

"""

if __name__ == "__main__":
    import openturns as ot
    import MultivariateRandomMixture as MV
    import numpy as np
    from MultivariateGaussianCharacteristicFunction import MGCF as mgcf

    # Storing all distribution results
    distribution_collection = []
    N = 2000000

    """
    Test 1
    ------
    Linear form ==> Random mixture approach
    """
    collection = ot.DistributionCollection([ot.Normal(), ot.Uniform()])
    matrix = ot.Matrix([[4, 1.4]])
    y = [2.0]
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix, y)
    distribution_collection.append(distribution)
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
    for value in x:
        c1 = distribution.computeCharacteristicFunction([value])
        c2 = random_mixture.computeCharacteristicFunction(value)
        print "x = %s, mv=%s, rm=%s" %(value, c1, c2)


    """
    Test 2
    ------
    The matrix is assumed to be diagonal for validation purposes
    """
    collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Normal(0.0, 1.0)])
    matrix = ot.Matrix([[4, 0], [0, 1.4]])
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix)
    # Equivalent Normal2D distribution
    normal2D = ot.Normal(distribution.getMean(), distribution.getCovariance())
    interval = distribution.getRange()
    mean = distribution.getMean()
    cov = distribution.getCovariance()
    sigma = distribution.getStandardDeviation()
    sample = distribution.getSample(N)
    print "range = ", interval
    print "mean = ", mean
    print "cov = ", cov
    print "sigma = ", sigma
    print "sample :"
    print "min = %s\nmax = %s\nmean = %s, cov = %s" %(sample.getMin(),sample.getMax(), sample.computeMean(), sample.computeCovariance())
    distribution_collection.append(distribution)
    # evaluation of the characteristic function in [xmin,ymin]x[xmax,ymax]
    xmin = -1.0
    ymin = -1.0
    xmax = 1.0
    ymax = 1.0
    dx = 0.2
    dy = 0.2
    # 2D grid
    x = np.arange(xmin, xmax + dx, dx)
    y = np.arange(ymin, ymax + dy, dy)
    for valuex in x:
        for valuey in y:
            c1 = distribution.computeCharacteristicFunction([valuex, valuey])
            c2 = mgcf(normal2D, [valuex, valuey])
            print "x=%s, y=%s"  %(valuex, valuey)
            print "mv=%s, rm=%s" %(c1, c2)
