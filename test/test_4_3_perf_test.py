#                                               -*- Python -*-
#
#  @file  test_4_3_perf_test.py
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
    import matplotlib.pyplot as plt
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
    for b in [6]:
        dt_parallel_computations = []
        for N in [64, 128]:
            # filename for the current b and N
            filename = "../validation/pdf_grid/valid_test_4_3/" + str(b) + "_" + str(N) + "/" + \
                       "valid_d3_4dists_" + str(b) + "_" + str(N) + ".csv"
            theoritical_values = ot.NumericalSample.ImportFromCSVFile(filename)
            theoritical_values_array = np.array([[[theoritical_values[iz + iy *N + ix * N * N][3]for iz in xrange(N)] for iy in xrange(N)] for ix in xrange(N)])
            print "Use case : b=%s, N=%s"%(b, N)
            tic = time.time()
            [grid, pdf_values] = distribution.computePDFOn3DGrid(b, N)
            toc = time.time()
            dt0 = toc - tic
            print "Error evaluation - standard version"
            max_abs_err = max(np.max(theoritical_values_array - pdf_values), np.min(theoritical_values_array - pdf_values))
            l2_err = np.sqrt(np.sum((theoritical_values_array - pdf_values) * (theoritical_values_array - pdf_values)))  / (N*N*N)
            print "L2_err=%s, Linfty_err = %s, CPU time=%s"%(l2_err,max_abs_err, dt0)
            
            for nproc in range(8):
                tic = time.time()
                [grid, pdf_values] = distribution.parallel_compute_pdf_on_3d_grid(b,N, nproc+1)
                toc = time.time()
                dt = toc - tic
                dt_parallel_computations.append(dt)
                print "Error evaluation - parallel version with %s cores"%(nproc+1)
                max_abs_err = max(np.max(theoritical_values_array - pdf_values), np.min(theoritical_values_array - pdf_values))
                l2_err = np.sqrt(np.sum((theoritical_values_array - pdf_values) * (theoritical_values_array - pdf_values)))  / (N*N*N)
                print "L2_err=%s, Linfty_err = %s, CPU time=%s"%(l2_err,max_abs_err, dt)

            fig = plt.figure()
            plt.subplot(211)
            plt.plot(np.arange(8) + 1, dt_parallel_computations, 'b', label = 'parallel version')
            plt.plot(np.arange(8) + 1, 8 * [dt0], 'r', label = 'standard version')
            #plt.xlabel('Number of cores')
            plt.ylabel('CPU time')
            plt.title('Performance of PDF evaluation on 3D grid with N = %s' %N)
            plt.legend(loc=0)
            plt.grid('on')
            plt.subplot(212)
            plt.plot(np.arange(8) + 1, dt / np.array(dt_parallel_computations), 'b', label = 'performance')
            plt.xlabel('Number of cores')
            plt.ylabel('Speed up')
            plt.legend(loc=0)
            plt.grid('on')
            plt.savefig('test_4_3_perf_tests_' + str(N) + '.png')
            plt.close()
