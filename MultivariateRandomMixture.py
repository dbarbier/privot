#                                               -*- Python -*-
#
#  @file  MultivariateRandomMixture.py
#  @brief MultivariateRandomMixture Distribution. Generalization of RandomMixture
#
#  Copyright (C) 2013 EADS IW France
#
#  Author(s) : Sofiane Haddad, IMACS
#              Denis Barbier, IMACS
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
    OpenTURNS MultivariateRandomMixture
    ===================================
    Multivariate distribution. Generalization of the OpenTURNS RandomMixture distribution


    Example
    --------
    >>> import openturns as ot

"""
import openturns as ot


class PythonMultivariateRandomMixture(ot.PythonDistribution):
    """
    Multivariate distribution
    Modelization of a multivariate random mixture distribution :
    Y = y_0 + M X, Y of size d
    where M is a dxn matrix, X is a n-random vector with independent components

    The probability density function (p.d.f) is computed using the Poisson summation formula :
    \sum_{j_1 \in \mathbb{Z}}\ldots \sum_{j_d \in \mathbb{Z}} p() = ...

    """

    def __init__(self, collection, matrix, y0 = None):
        """
        Parameters
        ----------
        y0 : 1D array-like
             Either a OpenTURNS NumericalPoint or a Numpy 1D-array
        matrix : Matrix
                 Either OpenTURNS matrix or Numpy matrix
        collection : list/DistributionCollection
                     Either OpenTURNS DistributionCollection of list of OpenTURNS distributions

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> dist = MV.PythonMultivariateRandomMixture.PythonMultivariateRandomMixture(\
                  ot.DistributionCollection([ot.Normal(), ot.Uniform()]), \
                  ot.Matrix([[1,1], [3,4], [2, -1]]), \
                  [0, 5])

        """
        n = len(collection)
        if (n == 0):
            raise ValueError('Expected collection of distributions with non null size')
        # check if all distributions are univariate
        for k in xrange(n):
            if (collection[k].getDimension() != 1):
                raise ValueError("Expected a collection of univariate distributions")
        self.collection = collection
        # matrix cast
        self.matrix = ot.Matrix(matrix)
        # Check matrix dimension
        if(self.matrix.getNbColumns() != n):
            raise ValueError("Matrix number of column is not coherant with collection size.")
        d = self.matrix.getNbRows()
        if (d > 3):
            raise ValueError("Mixture should be of dimension 1,2 or 3")
        if (y0 is None):
            self.y0 = ot.NumericalPoint(d * [0])
        else :
            assert len(y0) == d
            self.y0 = ot.NumericalPoint(y0)
        ot.PythonDistribution.__init__(self, d)
        # compute the mean and covariance
        self.computeMean()
        self.computeCovariance()
        # set the standard deviation
        self.sigma = self.cov.computeCholesky()

        