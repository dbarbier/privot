#                                               -*- Python -*-
#
#  @file  test_3N_2_nondiag.py
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
    
    def computeTheoriquePDF(x, y):
        """
        The theoretical density has been computed thanks to the Maple software
        """
        factor = np.sqrt(2)/(20 * np.pi)
        return factor * np.exp(-3.0/50 * y * y - 2.0/25 * x * y - 11.0/100 * x * x)

    """
    Test
    ------
    """
    collection = ot.DistributionCollection( 3 * [ot.Normal()])
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
    distribution.setMaxSize(2**8)
    # importing validation sample
    nx = 100
    ny = 100
    xmin = interval.getLowerBound()[0]
    xmax = interval.getUpperBound()[0]
    dx = xmax - xmin
    ymin = interval.getLowerBound()[1]
    ymax = interval.getUpperBound()[1]
    dy = ymax - ymin
    x = np.linspace(xmin + 0.25 * dx, xmax - 0.25 * dx, nx)
    y = np.linspace(ymin + 0.25 * dy, ymax - 0.25 * dy, ny)
    grid_x, grid_y = np.meshgrid(x, y)
    # Compute inverse of transformation matrix
    delta = 0.0
    dt = []
    shape = grid_x.shape
    pdf_values = np.ndarray(shape)
    for ix in range(shape[0]):
        for iy in range(shape[1]):
            x, y = grid_x[ix, iy], grid_y[ix, iy]
            theoretical_pdf = computeTheoriquePDF(x, y)
            u = [x, y]
            tic = time.time()
            pdf_estimate = distribution.computePDF(u)
            toc = time.time()
            pdf_values[ix, iy] = pdf_estimate
            dt.append(toc - tic)
            print "dt = %s"%(toc-tic)
            delta += abs((pdf_estimate - theoretical_pdf))**2
            error = pdf_estimate - theoretical_pdf
            abs_error = abs(error) / theoretical_pdf
            print "pdf_estimate=%s pdf_theoretical=%s, error=%s, abs_error=%s"%(pdf_estimate, theoretical_pdf, error, abs_error)
    # Variation of characteristic function
    delta /= (shape[0] * shape[1])
    print "delta of pdf=%s" %(np.sqrt(delta))

    try :
        import matplotlib.pylab as plt
        fig = plt.figure()
        plt.imshow(pdf_values, vmin=np.min(pdf_values), vmax=np.max(pdf_values), origin='lower', extent=[xmin+0.25*dx, xmax-0.25*dx, ymin+0.25*dy, ymax-0.25*dy])
        plt.colorbar()
        plt.savefig("3Normals2d_pdf.pdf")
        plt.close('all')
    except ImportError:
        ot.log.Warn("Matplotlib not found. Could not create iso values graph of pdf")

