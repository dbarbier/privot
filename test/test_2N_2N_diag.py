#                                               -*- Python -*-
#
#  @file  test_2D_2D_diag.py
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
    
    Test 2: R^2-->R^2 case - Diagonal matrix test for 
    the validation of the PythonMultivariateRandomMixture class

"""

if __name__ == "__main__":
    import openturns as ot
    import MultivariateRandomMixture as MV
    import numpy as np
    from MultivariateGaussianCharacteristicFunction import MGCF as mgcf
    N = 200000

    """
    Test
    ------
    The matrix is assumed to be diagonal for validation purposes
    """
    collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Normal(0.0, 1.0)])
    matrix = ot.Matrix([[4, 0], [0, 1.4]])
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix)
    interval = distribution.getRange()
    mean = distribution.getMean()
    cov = distribution.getCovariance()
    sigma = distribution.getStandardDeviation()
    sample = distribution.getSample(N)
    print "range = ", interval
    print "mean = ", mean
    print "cov = ", cov
    print "sigma = ", sigma
    # Equivalent Normal2D distribution
    normal2D = ot.Normal(distribution.getMean(), distribution.getCovariance())
    print "Gaussian distribution"    
    print "range = ", normal2D.getRange()
    print "mean = ", normal2D.getMean()
    print "cov = ", normal2D.getCovariance()
    print "sigma = ", normal2D.getStandardDeviation()
    print "sample :"
    print "min = %s\nmax = %s\nmean = %s, cov = %s" %(sample.getMin(),sample.getMax(), sample.computeMean(), sample.computeCovariance())
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
    # Compute delta(cf)
    delta = 0.0
    for valuex in x:
        for valuey in y:
            c1 = distribution.computeCharacteristicFunction([valuex, valuey])
            c2 = mgcf(normal2D, [valuex, valuey])
            delta += abs((c1 - c2))**2
    # Variation of characteristic function
    delta /= (len(x) * len(y))
    print "delta of characteristic function=%s" %(np.sqrt(delta))
