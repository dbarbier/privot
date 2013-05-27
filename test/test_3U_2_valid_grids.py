#                                               -*- Python -*-
#
#  @file  test_3U_2_nondiag.py
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

    Test 2: R^3-->R^2 case
    Validation with respect to the Maple
    use case
"""

if __name__ == "__main__":
    import openturns as ot
    import MultivariateRandomMixture as MV
    import MaxNormMeshGrid
    import numpy as np
    import time

    """
    Test
    ------
    """
    collection = ot.DistributionCollection([ot.Uniform(0,1), ot.Uniform(0,1), ot.Uniform(0,1)])
    matrix = ot.Matrix([[1, -2, 1], [1, 1, -3]])
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix)
    interval = distribution.getRange()
    mean = distribution.getMean()
    cov = distribution.getCovariance()
    sigma = distribution.getStandardDeviation()
    print "range = ", interval
    print "mean = ", mean
    print "cov = ", cov
    print "sigma = ", sigma
    for b in [3, 4, 5, 6]:
        for N in [512, 1024]:
            # filename for the current b and N
            filename = "../validation/pdf_grid/valid_d2_3unif_" + str(b) + "_" + str(N) + ".csv"
            theoritical_values = ot.NumericalSample.ImportFromCSVFile(filename)
            print "Use case : b=%s, N=%s"%(b, N)
            tic = time.time()
            [grid, pdf_values] = distribution.computePDFOn2DGrid(b, N)
            toc = time.time()
            theoritical_values_array = np.array([[theoritical_values[iy + ix *N][2]for iy in xrange(N)] for ix in xrange(N)])
            max_abs_err = max(np.max(theoritical_values_array - pdf_values), np.min(theoritical_values_array - pdf_values))
            l2_err = np.sqrt(np.sum((theoritical_values_array - pdf_values) * (theoritical_values_array - pdf_values))) / (N*N)
            print "L2_err=%s, Linfty_err = %s, CPU time = %s"%(l2_err,max_abs_err,toc-tic)

