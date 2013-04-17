#                                               -*- Python -*-
#
#  @file  test_10D_1D.py
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
    R^100-->R case - comparison with a mixture Test for the
    validation of the PythonMultivariateRandomMixture class
    The input dimension is huge, so that the T.L.C might be applied
    for values comparisons

"""

if __name__ == "__main__":
    import openturns as ot
    import MultivariateRandomMixture as MV
    import numpy as np
     
    N = 20000
    try :
        import matplotlib.pylab as plt
        from matplotlib.backends.backend_pdf import PdfPages
        has_pylab = True
    except ImportError:
        has_pylab = False

    """
    Test
    ----

    Linear form of Uniform sum ==> Random mixture approach comparison
    T.L.C ==> Gaussian approach
    """
    collection = ot.DistributionCollection(100 * [ot.Uniform()])
    matrix = ot.Matrix([100 * [1.0]])
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix)
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
    print "RandomMixture distribution"
    print "range = ", random_mixture.getRange()
    print "mean = ", random_mixture.getMean()
    print "cov = ", random_mixture.getCovariance()
    print "sigma = ", random_mixture.getStandardDeviation()
    print "sample :"
    print "min = %s\nmax = %s\nmean = %s, cov = %s" %(sample.getMin(),sample.getMax(), sample.computeMean(), sample.computeCovariance())
    # evaluation of the characteristic function between xmin and xmax
    xmin, xmax, dx = -10.0, 10.0, 1.0
    x = np.arange(xmin, xmax + dx, dx)
    delta = 0.0
    pdf = 0.0
    if has_pylab:
        pdf_file = 'validation_100D_1D_PDF.pdf'
        pdf_plot = PdfPages(pdf_file)
        pdf_mv = []
        pdf_rm = []
    for value in x:
        c1 = distribution.computeCharacteristicFunction([value])
        c2 = random_mixture.computeCharacteristicFunction(value)
        pdf1 = distribution.computePDF([value])
        pdf2 = random_mixture.computePDF([value])
        delta += abs((c1 - c2))**2
        pdf += abs((pdf1 - pdf2))**2
        if has_pylab:
            pdf_mv.append(pdf1)
            pdf_rm.append(pdf2)
        print pdf1, pdf2, abs(pdf2 - pdf1)/pdf2
    # Variation of characteristic function
    delta /= len(x)
    print "delta of characteristic function=%s" %(np.sqrt(delta))
    print "delta of pdf function=%s" %(np.sqrt(pdf))
    if has_pylab:
        fig = plt.figure()
        plt.subplot(211)
        plt.plot(x, pdf_mv, 'r', label = 'MV')
        plt.plot(x, pdf_rm, '-b', label = 'RM')
        plt.xlabel('')
        plt.ylabel('pdf plot')
        plt.title("validation of pdf function")
        plt.subplot(212)
        plt.semilogy(x, (plt.np.array(pdf_rm)-plt.np.array(pdf_mv))/plt.np.array(pdf_rm), 'r', label = 'MV')
        plt.xlabel('')
        plt.ylabel('Relative error on PDF')
        pdf_plot.savefig(fig)
        plt.close()
        pdf_plot.close()
