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
    
    Test 2: R^2-->R^2 case - Non diagonal matrix test for 
    the validation of the PythonMultivariateRandomMixture class

"""

if __name__ == "__main__":
    import openturns as ot
    import MultivariateRandomMixture as MV
    import numpy as np

    """
    Test
    ------
    """
    collection = ot.DistributionCollection([ot.Uniform(), ot.Normal()])
    matrix = ot.Matrix([[1, 4], [3, 2]])
    constant = [0.0, 0.0]
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
    interval = distribution.getRange()
    mean = distribution.getMean()
    cov = distribution.getCovariance()
    sigma = distribution.getStandardDeviation()
    print "range = ", interval
    print "mean = ", mean
    print "cov = ", cov
    print "sigma = ", sigma
    # evaluation of the characteristic function in [xmin,ymin]x[xmax,ymax]
    xmin, ymin = tuple(interval.getLowerBound())
    xmax, ymax = tuple(interval.getUpperBound())
    # length of interval in each dimension
    dX = xmax - xmin
    dY = ymax - ymin
    dx = 0.1
    dy = 0.1
    # 2D grid
    x = np.arange(xmin + 0.25 * dX, xmax - 0.25 * dX, dx)
    y = np.arange(ymin + 0.25 * dY, ymax - 0.25 * dY, dy)
    # Compute inverse of transformation matrix
    grid_x, grid_y = np.meshgrid(x,y)
    shape = grid_x.shape
    values = np.ndarray(grid_x.shape)
    for ix in range(shape[0]):
        for iy in range(shape[1]):
            valuex, valuey = grid_x[ix,iy],grid_y[ix,iy]
            u = [valuex, valuey]
            pdf_estimate = distribution.computePDF(u)
            values[ix,iy] = pdf_estimate
    try :
        import matplotlib.pylab as plt
        fig = plt.figure()
        plt.contour(values, vmin=np.min(values), vmax=np.max(values), origin='lower', extent=[np.min(x), np.max(x), np.min(y), np.max(y)])
        plt.colorbar()
        plt.title("Estimated PDF with MVRM")
        plt.savefig("2Uniform2d_pdf.pdf")
        plt.close('all')
    except ImportError:
        ot.log.Warn("Matplotlib not found. Could not create iso values graph of pdf")
