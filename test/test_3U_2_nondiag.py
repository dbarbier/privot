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
def get_points_on_surface_grid(dist, index):
    """
    """
    assert isinstance(index, int)
    d = dist.getDimension()
    # List of points to be taken into account
    list_points = []
    if d == 1 :
        for ix in [-index, index]:
            step = (ix,)
            yield step
    elif d == 2 :
        # In 2D case, we should take into account 4 faces of a
        # bidimensional square |x| = index, |y| = index
        # for easy reading and taking into account, we follow a
        # curve
        # 1) botton
        ix = float(-index)
        for iy in range(-index, index):
            step = (ix, iy)
            yield step
        # 2) right
        iy = float(index)
        for ix in range(-index, index):
            step = (ix, iy)
            yield step
        # 3) top
        ix = float(index)
        for iy in range(index, -index, -1):
            step = (ix, iy)
            yield step
        # 4) left hand
        iy = float(-index)
        for ix in range(index, -index, -1):
            step = (ix, iy)
            yield step
    elif d == 3 :
        # 1) contour (like 2D case) for each z
        for iz in xrange(-index, index + 1):
            # a) bottom
            iy = float(-index)
            for ix in range(-index, index):
                step = (ix, iy, iz)
                yield step
            # b) right
            ix = float(index)
            for iy in range(-index, index):
                step = (ix, iy, iz)
                yield step
            # c) top
            iy = float(index)
            for ix in range(index, -index, -1):
                step = (ix, iy, iz)
                yield step
            # d) left hand
            ix = float(-index)
            for iy in range(index, -index, -1):
                step = (ix, iy, iz)
                yield step
        # 2) quasi-plain square for |z| = index
        # quasi because we remove corners and contours already in the list
        for iz in [-index, index]:
            for ix in xrange(-index + 1 , index):
                for iy in xrange(-index + 1, index):
                  step = (ix, iy, iz)
                  yield step

if __name__ == "__main__":
    import openturns as ot
    import MultivariateRandomMixture as MV
    import numpy as np
    import time

    """
    Test
    ------
    """
    #ot.Log.Show(ot.Log.ALL)
    MV.PythonMultivariateRandomMixture.get_points_on_surface_grid_ = get_points_on_surface_grid
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
    distribution.setBlockMin(3)
    distribution.setBlockMax(10)
    distribution.setMaxSize(2**10)
    # importing validation sample
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
        print "x=%s, y=%s, pdf_estimate=%s pdf_theoretical=%s"%(x, y, pdf_estimate, theoretical_pdf)
        print "pdf_error =%s" %distribution.getLastPDFError()
    # Variation of characteristic function
    delta /= len(validation_sample)
    print "delta of pdf=%s" %(np.sqrt(delta))
