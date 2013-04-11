#                                               -*- Python -*-
#
#  @file  MultivariateRandomMixture.py
#  @brief MultivariateRandomMixture Distribution. Generalization of RandomMixture
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
    OpenTURNS MultivariateRandomMixture
    ===================================
    Multivariate distribution. Generalization of the OpenTURNS RandomMixture distribution


    Example
    --------
    >>> import openturns as ot

"""
import openturns as ot
import cmath

# Dictionary equivalent to a resource map
mvrm_resource_map = {"MultivariateRandomMixture-DefaultAlpha" : 4,\
                     "MultivariateRandomMixture-DefaultBeta": 8}

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
        collection : list/DistributionCollection
                     Either OpenTURNS DistributionCollection or a list of OpenTURNS distributions
        matrix : Matrix
                 Either OpenTURNS matrix or Numpy matrix
        y0 : 1D array-like
             Either a python list, an OpenTURNS NumericalPoint or a Numpy 1D-array

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
        # Use of setDistributionCollection method
        self.setDistributionCollection(collection)
        # matrix cast
        self.matrix = ot.Matrix(matrix)
        # Check matrix dimension
        if(self.matrix.getNbColumns() != n):
            raise ValueError("Matrix number of columns is not coherant with collection size.")
        d = self.matrix.getNbRows()
        if (d > 3):
            raise ValueError("Mixture should be of dimension 1, 2 or 3")
        if (y0 is None):
            self.y0 = ot.NumericalPoint(d * [0])
        else :
            assert len(y0) == d
            self.y0 = ot.NumericalPoint(y0)
        ot.PythonDistribution.__init__(self, d)
        # Set alpha and beta values
        self.alpha = mvrm_resource_map['MultivariateRandomMixture-DefaultAlpha']
        self.beta = mvrm_resource_map['MultivariateRandomMixture-DefaultBeta']
        # compute the mean and covariance
        self.computeMean()
        self.computeCovariance()
        # set the standard deviation
        self.sigma = self.cov.computeCholesky()
        # compute the range
        self.computeRange()

    def setDistributionCollection(self, collection):
        """
        Set the distribution collection
        This method should not be used, except by the __init__ method
        """
        for k in xrange(len(collection)):
            # check if distribution is univariate
            if (collection[k].getDimension() != 1):
                raise ValueError("Expected a collection of univariate distributions")
        self.collection = collection

    def getDistributionCollection(self):
        """
        Returns the distribution collection
        """
        return self.collection

    def getMatrix(self):
        """
        Returns the matrix of the affine transform
        """
        return self.matrix

    def getConstant(self):
        """
        Returns the constant vector
        """
        return self.y0

    def computeH(self):
        """
        Compute h parameters
        """
        self.h = [2.0 * cmath.pi / ((self.beta + 4.0 * self.alpha) * self.sigma[l,l]) for l in xrange(self.getDimension())]

    def computeMean(self):
        """
        Compute the mean of the multivariate mixture
        This method is implicite. Use the getMean to get the mean value
        """
        mu = [dist.getMean()[0] for dist in self.collection]
        self.mu = self.matrix * ot.NumericalPoint(mu) + self.y0

    def getMean(self):
        """
        Returns the mean vector of the mixture
        """
        return self.mu

    def computeCovariance(self):
        """
        Returns the covariance of the mixture
        This method is implicite. Use the getCovariance to get the mean value
        """
        cov = ot.ComposedDistribution(self.collection).getCovariance()
        m1 = cov *  self.matrix.transpose()
        cov = self.matrix * m1
        self.cov = ot.CovarianceMatrix(cov.getImplementation())

    def getCovariance(self):
        """
        Returns the covariance matrix of the mixutre
        """
        return self.cov

    def getStandardDeviation(self):
        """
        Returns the standard deviation
        """
        return self.sigma

    def computeRange(self):
        """
        Compute the range of the distribution
        """
        interval_collection = []
        for i in xrange(self.getDimension()):
            interval = ot.Interval(self.y0[i], self.y0[i])
            for j in xrange(len(self.collection)):
                interval += self.collection[j].getRange() * self.matrix[i, j]
            interval_collection.append(interval)
        # Build the interval from the collection of interval
        lowerbound = [interval.getLowerBound()[0] for interval in interval_collection]
        finitelowerbound = [interval.getFiniteLowerBound()[0] for interval in interval_collection]
        upperbound = [interval.getUpperBound()[0] for interval in interval_collection]
        finiteupperbound = [interval.getFiniteUpperBound()[0] for interval in interval_collection]
        self.interval = ot.Interval(lowerbound, upperbound, ot.BoolCollection(finitelowerbound), ot.BoolCollection(finiteupperbound))
        # get the diagonal of the std matrix
        s = ot.NumericalPoint([self.sigma[k, k] for k in xrange(self.getDimension())])
        interval = ot.Interval(self.getMean() - s * self.beta, self.getMean() + s * self.beta)
        self.interval = self.interval.intersect(interval)

    def getRange(self):
        """
        Returns the range of the distribution
        """
        return self.interval

    def computeCharacteristicFunction(self, u):
        """
        Return the characteristic function evaluated on u.

        Parameters
        ----------
        u :  vector of size d
             1D array-like (np array, python list, OpenTURNS NumericalPoint)

        Returns
        -------
        out : Complex
              The characteristic function evaluated on u

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> dist = MV.PythonMultivariateRandomMixture.PythonMultivariateRandomMixture(\
                  ot.DistributionCollection([ot.Normal(), ot.Uniform()]), \
                  ot.Matrix([[1,1], [3,4], [2, -1]]), \
                  [0, 5])
        >>> cfx = dist.computeCharacteristicFunction( [0.3, 0.9] )

        """
        assert len(u) == self.getDimension()
        somme = 0
        # The characteristic function is given by the following formula:
        # \phi(u) = \prod_{j=1}^{d} (exp(i * u_j * y0_j) * \prod_{k=1}^{n} \phi_{X_k}(Mjk u_j))
        n = len(self.collection)
        d = self.getDimension()
        mt = self.matrix.transpose()
        # compute M^t * u
        mt_u = mt * u
        # compute the deterministic term
        for j in xrange(d):
            somme += u[j] * self.y0[j] * 1j
        # compute the random part
        # The variables are independent
        for k in xrange(n):
            somme += self.collection[k].computeLogCharacteristicFunction(mt_u[k])
        return cmath.exp(somme)

    def getRealization(self):
        """
        Get a realization of the distribution
        """
        realization = [dist.getRealization()[0] for dist in self.collection]
        realization = self.matrix * realization + self.y0
        return realization

    def getSample(self, n):
        """
        Get a sample of size n of the distribution
        """
        assert isinstance(n, int)
        sample = ot.ComposedDistribution(self.collection).getSample(n)
        # product matrix * realization
        # using np for scalability (matrix * sample not available)
        try :
            import numpy as np
            sample = np.array(sample) * np.matrix(self.matrix).transpose()
            # np.matrix could not be casted into ot.NumericalSample
            sample = np.array(sample)
            sample = ot.NumericalSample(sample)
        except ImportError :
            #
            sample = ot.NumericalSample([self.matrix * sample_element for sample_element in sample])
        # Do not forget the constant term
        sample.translate(self.y0)
        return sample

    def computePDF(self, point, xMin, xMax, pointNumber, precision):
        raise RuntimeError( 'You must define a method computePDF(x) -> pdf, where pdf is a float' )

    def computeCDF(self, X):
        raise RuntimeError( 'You must define a method computeCDF(x) -> cdf, where cdf is a float' )

    def __repr__(self):
        return 'PythonMultivariateRandomMixture distribution. Dimension =  %d' % self.getDimension()

    def __str__(self):
        """
        pretty print of the distribution
        """
        s = ''
        for k in xrange(self.getDimension()):
            s += "Y_%d: %1.3e + " %(k+1, self.y0[k])
            for j in xrange(len(self.collection)):
                s += "%1.3e * %s" %(self.matrix[k, j], str(self.collection[j]))
                if j < len(self.collection) - 1:
                    s += ' + '
                else :
                    s += '\n'
        return s

class MultivariateRandomMixture(ot.Distribution):
    """
    MultivariateRandomMixture allows to build an OpenTURNS distribution

    """
    def __new__(self, collection, matrix, y0):
        instance = PythonMultivariateRandomMixture(collection, matrix, y0)
        return ot.Distribution(instance)
