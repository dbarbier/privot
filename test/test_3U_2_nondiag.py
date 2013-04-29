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
    #ot.Log.Show(ot.Log.ALL)
    collection = ot.DistributionCollection([ot.Uniform(0,1), ot.Uniform(0,1), ot.Uniform(0,1)])
    matrix = ot.Matrix([[1, -2, 1], [1, 1, -3]])
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix)
    distribution.setGridMesher(MaxNormMeshGrid.SkinCube2D(distribution.getReferenceBandwidth(), True))
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
    validation_sample = ot.NumericalSample.ImportFromCSVFile("../validation/valid_d2_3unif.csv")
    # sample for error observation
    estimate_sample = ot.NumericalSample(len(validation_sample), 3)
    delta = 0.0
    dt = []
    for ind in xrange(len(validation_sample)):
        x, y, pdf_theoretical = tuple(validation_sample[ind])
        estimate_sample[ind, 0] = x
        estimate_sample[ind, 1] = y
        u = [x, y]
        tic = time.time()
        pdf_estimate = distribution.computePDF(u)
        toc = time.time()
        estimate_sample[ind, 2] = pdf_estimate
        dt.append(toc - tic)
        print "dt = %s"%(toc-tic)
        delta += abs((pdf_estimate - pdf_theoretical))**2
        print "x=%s, y=%s, pdf_estimate=%s pdf_theoretical=%s"%(x, y, pdf_estimate, pdf_theoretical)
        print "pdf_error=%s" %distribution.getLastPDFError()
    # Variation of characteristic function
    delta /= len(validation_sample)
    print "delta of pdf=%s" %(np.sqrt(delta))
    # Reorganization of data using meshgrid for graphical purposes
    x = np.unique(np.array(validation_sample.getMarginal(0)))
    y = np.unique(np.array(validation_sample.getMarginal(1)))
    grid_x, grid_y = np.meshgrid(x,y)
    shape = grid_x.shape
    pdf_theoretical = np.ndarray(shape)
    pdf_estimate = np.ndarray(shape)
    ind = 0
    for i in range(shape[0]):
        for j in range(shape[1]):
            # grid_x[j,i],grid_y[j,i] respectively equal to validation_sample[ind,0], validation_sample[ind,1]
            pdf_theoretical[j,i] = validation_sample[ind, 2]
            pdf_estimate[j,i] = estimate_sample[ind, 2]
            ind +=1
    try :
        import matplotlib.pylab as plt
        fig = plt.figure()
        plt.contour(pdf_estimate, vmin=np.min(pdf_estimate), vmax=np.max(pdf_estimate), origin='lower', extent=[np.min(x), np.max(x), np.min(y), np.max(y)])
        plt.colorbar()
        plt.title("Estimated PDF with MVRM")
        plt.savefig("3Uniform2d_pdf.pdf")
        plt.close('all')
        fig = plt.figure()
        pdf_error = pdf_estimate - pdf_theoretical
        plt.imshow(pdf_error, vmin=np.min(pdf_error), vmax=np.max(pdf_error), origin='lower', extent=[np.min(x), np.max(x), np.min(y), np.max(y)])
        plt.colorbar()
        plt.title("Error PDF with MVRM")
        plt.savefig("3Uniform2d_error_pdf.pdf")
        plt.close('all')
    except ImportError:
        ot.log.Warn("Matplotlib not found. Could not create iso values graph of pdf")
