#                                               -*- Python -*-
#
#  @file  test_2D_2D_nondiag.py
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
    import numpy as np
    import time

    """
    Test
    ------
    """
    collection = ot.DistributionCollection([ot.Uniform(), ot.Uniform(), ot.Uniform()])
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
    distribution.setBlockMin(3)
    distribution.setBlockMax(10)
    # importting validation sample
    validation_sample = ot.NumericalSample.ImportFromCSVFile("../validation/valid_d2_3unif.csv")
    # Compute inverse of transformation matrix
    delta = 0.0
    dt = []
    for element in validation_sample:
        x, y, theoretical_pdf = tuple(element)
        u = [x, y]
        tic = time.time()
        pdf_estimate = distribution.computePDF(u)
        toc = time.time()
        dt.append(toc - tic)
        print "dt = %s"%(toc-tic)
        #theoretical_pdf = 
        delta += abs((pdf_estimate - theoretical_pdf))**2
        print "pdf_estimate=%s pdf_theoretical=%s"%(pdf_estimate, theoretical_pdf)
    # Variation of characteristic function
    delta /= (len(x) * len(y))
    print "delta of pdf=%s" %(np.sqrt(delta))
