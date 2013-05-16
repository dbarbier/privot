#                                               -*- Python -*-
#
#  @file  test_2D_1D.py
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
    R^2-->R case - comparison with a mixture Test for the
    validation of the PythonMultivariateRandomMixture class
    Non gaussian behaviour ==> validation of result

"""

if __name__ == "__main__":
    import openturns as ot
    import MultivariateRandomMixture as MV
    import numpy as np
    from MultivariateGaussianCharacteristicFunction import MGCF as mgcf
    import time

    N = 10000
    blockMin = 3
    blockMax = 10
    comparison_fft_nominal = True
    pdf_precision = 1.e-30

    try :
        import matplotlib.pylab as plt
        from matplotlib.backends.backend_pdf import PdfPages
        has_pylab = True
    except ImportError:
        has_pylab = False

    """
    Test
    ----

    Linear form ==> Random mixture approach comparison
    """
    collection = ot.DistributionCollection([ot.Uniform(-50, -35), ot.Uniform(35, 50)])
    matrix = ot.Matrix([2 * [1.0]])
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix)
    distribution.setBlockMin(blockMin)
    distribution.setBlockMax(blockMax)
    distribution.setPDFPrecision(pdf_precision)
    interval = distribution.getRange()
    mean = distribution.getMean()
    cov = distribution.getCovariance()
    sigma = distribution.getStandardDeviation()
    sample = distribution.getSample(N)
    print "MultivariateRandomMixture distribution"
    print "range = ", interval
    print "mean = ", mean
    print "cov = ", cov
    print "sigma = ", sigma

    # Equivalent random mixture
    random_mixture = ot.RandomMixture(collection)

    random_mixture.setBlockMin(blockMin)
    random_mixture.setBlockMax(blockMax)

    print "RandomMixture distribution"
    print "range = ", random_mixture.getRange()
    print "mean = ", random_mixture.getMean()
    print "cov = ", random_mixture.getCovariance()
    print "sigma = ", random_mixture.getStandardDeviation()

    # Reference distribution
    ref_dist = ot.Triangular(-15.0,0,15.0)

    print "RandomMixture distribution"
    print "range = ", random_mixture.getRange()
    print "mean = ", random_mixture.getMean()
    print "cov = ", random_mixture.getCovariance()
    print "sigma = ", random_mixture.getStandardDeviation()

    print "sample :"
    print "min = %s\nmax = %s\nmean = %s, cov = %s" %(sample.getMin(),sample.getMax(), sample.computeMean(), sample.computeCovariance())
    # evaluation of the characteristic function between xmin and xmax
    xmin, xmax, dx = -14.5, 14.5, 0.5
    x = np.arange(xmin, xmax + dx, dx)
    t1 = []
    t2 = []
    if has_pylab:
        pdf_file = 'validation_2U_1T_PDF.pdf'
        pdf_plot = PdfPages(pdf_file)
        pdf_mv = []
        pdf_theorique = []    
    for value in x:
        tic = time.time()
        c1 = distribution.computePDF([value])
        toc = time.time()
        dt1 = toc - tic
        t1.append(dt1)
        tic = time.time()
        c2 = random_mixture.computePDF(value)
        toc = time.time()
        dt2 = toc - tic
        t2.append(dt2)
        pdf_th = ref_dist.computePDF(value)
        if has_pylab:
            pdf_mv.append(c1)
            pdf_theorique.append(pdf_th)
        print "time of computation : distribution=%s, rm=%s, ratio=%s" %(dt1, dt2, dt1/dt2)
        print "value=%s, dist_pdf=%s, rm_pdf=%s, pdf_th=%s, error=%s" %(value, c1, c2, pdf_th, abs(c1 - pdf_th)/pdf_th)

    # Variation of characteristic function
    if has_pylab:
        fig = plt.figure()
        plt.subplot(211)
        plt.plot(x, pdf_mv, 'r', label = 'MV')
        plt.plot(x, pdf_theorique, '-b', label = 'RM')
        plt.xlabel('')
        plt.ylabel('pdf plot')
        plt.title("validation of pdf function")
        plt.legend()
        plt.subplot(212)
        plt.semilogy(x, abs(plt.np.array(pdf_th)-plt.np.array(pdf_mv))/plt.np.array(pdf_th), 'r', label = 'MV')
        plt.xlabel('')
        plt.ylabel('Relative error on PDF')
        plt.legend()
        pdf_plot.savefig(fig)
        plt.close()

    # compute the pdf on a grid of form mu +/- b *sigma with N points
    b = 6.0
    N = 2**(blockMax)
    [y, pdf] = distribution.computePDFOn1DGrid(b, N)
    pdf_theorique = np.array([ref_dist.computePDF(el) for el in y])
    for m in xrange(len(y)):
        ym = y[m]
        pdf_t = pdf_theorique[m]
        pdf_v = pdf[m]
        print "value=%s, pdf_fft=%s, pdf_th=%s, rel_error=%s" %(ym, pdf_v, pdf_t, abs(pdf_v - pdf_t)/pdf_t)
    if comparison_fft_nominal:
        distribution.setReferenceBandwidth(list(np.pi / np.array(sigma * b)))
        pdf_evaluation = np.array([distribution.computePDF([el]) for el in y])
        for m in xrange(len(y)):
            ym = y[m]
            pdf_v = pdf[m]
            pdf_value = pdf_evaluation[m]
            print "value=%s, pdf_without_fft=%s, pdf_fft=%s, rel_error=%s" %(ym, pdf_value ,pdf_v, abs(pdf_v - pdf_value)/pdf_value)

    if has_pylab:
        fig = plt.figure()
        plt.subplot(211)
        plt.plot(y, pdf_theorique, 'r', label = 'RM')
        plt.plot(y, pdf, 'g-', label = 'MV')
        plt.legend()
        plt.xlabel('')
        plt.ylabel('pdf plot')
        plt.title("validation of pdf function on a regular grid")
        plt.legend()
        plt.subplot(212)
        plt.semilogy(y, abs(pdf_theorique- pdf)/pdf_theorique, 'r', label = 'MV')
        plt.xlabel('')
        plt.ylabel('Relative error on PDF')
        plt.legend()
        pdf_plot.savefig(fig)
        plt.close()
        pdf_plot.close()
