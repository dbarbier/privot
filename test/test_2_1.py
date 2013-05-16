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

"""

if __name__ == "__main__":
    import openturns as ot
    import MultivariateRandomMixture as MV
    import numpy as np
    from MultivariateGaussianCharacteristicFunction import MGCF as mgcf

    Nreal = 100000
    blockMin = 3
    blockMax = 10
    comparison_fft_nominal = True
    pdf_precision = 1.e-30
    """
    Test 
    ----

    Linear form ==> Random mixture approach comparison
    """
    collection = ot.DistributionCollection([ot.Normal(), ot.Uniform()])
    matrix = ot.Matrix([[4, 1.4]])
    constant = [2.0]
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix)
    distribution.setBlockMin(blockMin)
    distribution.setBlockMax(blockMax)
    distribution.setPDFPrecision(pdf_precision)
    interval = distribution.getRange()
    mean = distribution.getMean()
    cov = distribution.getCovariance()
    sigma = distribution.getStandardDeviation()
    sample = distribution.getSample(Nreal)
    print "MultivariateRandomMixture distribution"
    print "range = ", interval
    print "mean = ", mean
    print "cov = ", cov
    print "sigma = ", sigma
    # Equivalent random mixture
    random_mixture = ot.RandomMixture(collection, [4.0, 1.4], 2.0)
    print "RandomMixture distribution"
    print "range = ", random_mixture.getRange()
    print "mean = ", random_mixture.getMean()
    print "cov = ", random_mixture.getCovariance()
    print "sigma = ", random_mixture.getStandardDeviation()
    print "sample :"
    print "min = %s\nmax = %s\nmean = %s, cov = %s" %(sample.getMin(),sample.getMax(), sample.computeMean(), sample.computeCovariance())
    # compute the pdf on a grid of form mu +/- b *sigma with N points
    b = 6.0
    N = 2**(blockMax)
    [y, pdf] = distribution.computePDFOn1DGrid(b, N)
    pdf_theorique = np.array([random_mixture.computePDF(el) for el in y])
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
    