#                                               -*- Python -*-
#
#  @file  test_4_3_nondiag.py
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
    MV.PythonMultivariateRandomMixture.get_points_on_surface_grid_ = get_points_on_surface_grid
    randomMixture = ot.RandomMixture(ot.DistributionCollection([ot.Normal(2, 1), ot.Normal(-2, 1)]))
    collection = ot.DistributionCollection([ot.Normal(0.0,1.0), randomMixture, ot.Uniform(0,1), ot.Uniform(0,1)])
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
    blockMin = 3
    blockMax = 9
    maxSize = 1 << blockMax
    distribution.setBlockMin(blockMin)
    distribution.setBlockMax(blockMax)
    distribution.setMaxSize(maxSize)
    distribution.setPDFPrecision(1.e-6)
    # importing validation sample
    validation_sample = ot.NumericalSample.ImportFromCSVFile("../validation/valid_d3_4dists.csv")
    # sample for error observation
    estimate_sample = ot.NumericalSample(len(validation_sample), 4)
    delta = 0.0
    dt = []
    for ind in xrange(len(validation_sample)):
        x, y, z, pdf_theoretical = tuple(validation_sample[ind])
        estimate_sample[ind, 0] = x
        estimate_sample[ind, 1] = y
        estimate_sample[ind, 2] = y
        u = [x, y, z]
        tic = time.time()
        pdf_estimate = distribution.computePDF(u)
        toc = time.time()
        estimate_sample[ind, 3] = pdf_estimate
        dt.append(toc - tic)
        print "dt = %s"%(toc-tic)
        delta += abs((pdf_estimate - pdf_theoretical))**2
        print "x=%s, y=%s, z=%s, pdf_estimate=%s, pdf_theoretical=%s"%(x, y, z,  pdf_estimate, pdf_theoretical)
        print "pdf_error=%s" %distribution.getLastPDFError()
    # Variation of characteristic function
    delta /= len(validation_sample)
    print "delta of pdf=%s" %(np.sqrt(delta))
