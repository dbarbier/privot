#                                               -*- Python -*-
#
#  @file  test_4_3_valid_grids.py
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

    Test 2: R^4-->R^3 case
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
    mixture = ot.Mixture(ot.DistributionCollection([ot.Normal(2,1), ot.Normal(-2,1)]))
    collection = ot.DistributionCollection([ot.Normal(0.0,1.0), mixture, ot.Uniform(0,1), ot.Uniform(0,1)])
    matrix = ot.Matrix([[1, -0.05, 1, -0.5], [0.5, 1, -0.05, 0.3], [-0.5, -0.1, 1.2, -0.8]])
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
        for N in [64, 128]:
            # filename for the current b and N
            filename = "../validation/pdf_grid/valid_test_4_3/" + str(b) + "_" + str(N) + "/" + \
                       "valid_d3_4dists_" + str(b) + "_" + str(N) + ".csv"
            theoritical_values = ot.NumericalSample.ImportFromCSVFile(filename)
            print "Use case : b=%s, N=%s"%(b, N)
            tic = time.time()
            [grid, pdf_values] = distribution.computePDFOn3DGrid(b, N)
            toc = time.time()
            theoritical_values_array = np.array([[[theoritical_values[iz + iy *N + ix * N * N][3]for iz in xrange(N)] for iy in xrange(N)] for ix in xrange(N)])
            max_abs_err = max(np.max(theoritical_values_array - pdf_values), np.min(theoritical_values_array - pdf_values))
            l2_err = np.sqrt(np.sum((theoritical_values_array - pdf_values) * (theoritical_values_array - pdf_values)))  / (N*N*N)
            print "L2_err=%s, Linfty_err = %s, CPU time=%s"%(l2_err,max_abs_err, toc - tic)
            outfile = "out_valid_d3_4dists_" + str(b) + "_" + str(N) + ".csv"
            print "Writing values in "+outfile
            xgrid_values, ygrid_values, zgrid_values = tuple(grid)
            f = open(outfile, 'w')
            f.write("x,y,z,pdf\n")
            for i in xrange(len(xgrid_values)):
                for j in xrange(len(ygrid_values)):
                    for k in xrange(len(zgrid_values)):
                        f.write("{0:.16g},{1:.16g},{2:.16g},{3:.16g}\n".format(xgrid_values[i], ygrid_values[j], zgrid_values[k], pdf_values[i][j][k]))
            f.close()

