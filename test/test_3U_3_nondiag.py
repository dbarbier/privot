#                                               -*- Python -*-
#
#  @file  test_3U_3_nondiag.py
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

    Test 2: R^3-->R^3 case - Non diagonal matrix test

"""

if __name__ == "__main__":
    import openturns as ot
    import MultivariateRandomMixture as MV
    import numpy as np
    blockMin = 3
    blockMax = 7
    n_blockMax = 2**blockMax
    maxSize = 4 * n_blockMax * (n_blockMax + 1) * (2 * n_blockMax + 1) + 2 * n_blockMax
    b = 6.0
    N = 128

    """
    Test
    ------
    """
    collection = ot.DistributionCollection([ot.Uniform(), ot.Uniform(), ot.Uniform()])
    matrix = ot.Matrix([[1, 4, 5], [3, 2, -1], [0,0,1]])
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix)
    distribution.setBlockMin(blockMin)
    distribution.setBlockMax(blockMax)
    distribution.setGridMesher(MV.MaxNormMeshGrid.CachedMeshGrid(MV.MaxNormMeshGrid.SkinCube3D(distribution.getReferenceBandwidth(), symmetric=True), size=maxSize))
    interval = distribution.getRange()
    mean = distribution.getMean()
    cov = distribution.getCovariance()
    sigma = distribution.getStandardDeviation()
    print "range = ", interval
    print "mean = ", mean
    print "cov = ", cov
    print "sigma = ", sigma
    # evaluation of the characteristic function in [xmin,xmax]x[ymin,ymax]x[zmin,zmax]
    xmin, ymin, zmin = tuple(interval.getLowerBound())
    xmax, ymax, zmax = tuple(interval.getUpperBound())
    # length of interval in each dimension
    dX = xmax - xmin
    dY = ymax - ymin
    dZ = zmax - zmin
    # 3D grid
    dx = 1.0
    dy = 1.0
    dz = 1.0
    x = slice(xmin + 0.2 * dX, xmax - 0.2 * dX, dx)
    y = slice(ymin + 0.2 * dY, ymax - 0.2 * dY, dy)
    z = slice(zmin + 0.2 * dZ, zmax - 0.2 * dZ, dz)
    grid_x, grid_y, grid_z = np.mgrid[x, y, z]
    shape = grid_x.shape
    values_th = np.ndarray(shape)
    values_app = np.ndarray(shape)
    # Compute inverse of transformation matrix
    print "Analytic estimate"
    for ix in range(shape[0]):
        for iy in range(shape[1]):
            for iz in range(shape[2]):
                u = [grid_x[ix, iy, iz], grid_y[ix, iy, iz], grid_z[ix, iy, iz]]
                pdf_estimate = distribution.computePDF(u)
                values_th[ix, iy, iz]=  pdf_estimate
    # computation on a regular grid
    [regular_grid, values_th_grid]= distribution.computePDFOn3DGrid(b, N)
    print "Analytic estimate done"
    # For the validation of the poisson formula, we compute again the pdf
    print "Non analytic estimate"
    distribution.isAnalyticPDF_ = False
    for ix in range(shape[0]):
        for iy in range(shape[1]):
            for iz in range(shape[2]):
                u = [grid_x[ix, iy, iz], grid_y[ix, iy, iz], grid_z[ix, iy, iz]]
                pdf_estimate = distribution.computePDF(u)
                values_app[ix, iy, iz] =  pdf_estimate
                pdf_theo = values_th[ix, iy, iz]
                print "point=%s, pdf_theoritical=%s, pdf_estimate=%s"%(u, pdf_theo, pdf_estimate)
    # computation on a regular grid
    [regular_grid, values_app_grid]= distribution.computePDFOn3DGrid(b, N)
    print "Non analytic estimate done"
