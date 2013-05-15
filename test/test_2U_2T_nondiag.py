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
    import time

    # Constants

    blockMin = 3
    blockMax = 6
    n_blockMax = 2**blockMax
    maxSize = 4 * n_blockMax * (n_blockMax + 1)
    b = 5.0
    N = 256
    # Elements of distribution
    collection = ot.DistributionCollection([ot.Uniform(-1, 1), ot.Uniform(-1, 1)])
    matrix = ot.Matrix([[1, 4], [3, 2]])
    constant = [0.0, 0.0]
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
    distribution.setBlockMin(blockMin)
    distribution.setBlockMax(blockMax)
    distribution.setGridMesher(MV.MaxNormMeshGrid.CachedMeshGrid(MV.MaxNormMeshGrid.SkinCube2D(distribution.getReferenceBandwidth(), symmetric=True), size=maxSize))
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
    dx = 0.5
    dy = 0.5
    # 2D grid
    x = np.arange(xmin - 0.5 * dX, xmax + 0.5 * dX, dx)
    y = np.arange(ymin - 0.5 * dY, ymax + 0.5 * dY, dy)
    # Compute inverse of transformation matrix
    grid_x, grid_y = np.meshgrid(x,y)
    shape = grid_x.shape
    values_th = np.ndarray(grid_x.shape)
    for ix in range(shape[0]):
        for iy in range(shape[1]):
            valuex, valuey = grid_x[ix,iy],grid_y[ix,iy]
            u = [valuex, valuey]
            pdf_estimate = distribution.computePDF(u)
            values_th[ix,iy] = pdf_estimate
    # computation on a regular grid
    [regular_grid, values_th_grid]= distribution.computePDFOn2DGrid(b, N)
    # For the validation of the poisson formula, we compute again the pdf
    print "Non analytic estimate"
    distribution.isAnalyticPDF_ = False
    values_app = np.ndarray(grid_x.shape)
    for ix in range(shape[0]):
        for iy in range(shape[1]):
            valuex, valuey = grid_x[ix,iy],grid_y[ix,iy]
            u = [valuex, valuey]
            pdf_estimate = distribution.computePDF(u)
            values_app[ix, iy] = pdf_estimate
            pdf_theo = values_th[ix, iy]
            print "point=%s, pdf_value=%s, pdf_theo=%s"%(u, pdf_estimate, pdf_theo)
            error = abs(float(pdf_theo - pdf_estimate))
            try :
                error /= float(pdf_theo)
                print "relative error=%s"%error
            except ZeroDivisionError:
                print "absolute error=%s"%error
    # computation on a regular grid
    [regular_grid, values_app_grid]= distribution.computePDFOn2DGrid(b, N)
    try :
        import matplotlib.pylab as plt
        fig = plt.figure()
        plt.contour(values_th, 20,  vmin=np.min(values_th), vmax=np.max(values_th), origin='lower', extent=[np.min(x), np.max(x), np.min(y), np.max(y)])
        plt.colorbar()
        plt.title("Estimated PDF with MVRM using exact formula")
        plt.savefig("2Uniform2d_pdf_th.pdf")
        plt.close('all')
        fig = plt.figure()
        plt.contour(values_app, 20,  vmin=np.min(values_th), vmax=np.max(values_th), origin='lower', extent=[np.min(x), np.max(x), np.min(y), np.max(y)])
        plt.colorbar()
        plt.title("Estimated PDF with MVRM using Poisson formula")
        plt.savefig("2Uniform2d_pdf_nmu.pdf")
        plt.close('all')
        fig = plt.figure()
        plt.contour(values_app-values_th, 20,  vmin=np.min(values_th), vmax=np.max(values_th), origin='lower', extent=[np.min(x), np.max(x), np.min(y), np.max(y)])
        plt.colorbar()
        plt.title("Error with PDF estimate")
        plt.savefig("2Uniform2d_pdf_err.pdf")
        plt.close('all')
    except ImportError:
        ot.log.Warn("Matplotlib not found. Could not create iso values graph of pdf")
