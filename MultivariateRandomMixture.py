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
import scipy

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
        # Set alpha and beta values
        self.alpha = 5.0 #ot.ResourceMap.GetAsNumericalScalar( "RandomMixture-DefaultAlpha" )
        self.beta = 6.0
        # compute the mean and covariance
        self.computeMean()
        self.computeCovariance()
        # set the standard deviation
        self.sigma = self.cov.computeCholesky()
        # compute the range
        self.computeRange()

    def computeH(self):
        """
        Compute h parameters
        """
        self.h = [2.0 * scipy.pi / ((self.beta + 4.0 * self.alpha) * self.sigma[l,l]) for l in xrange(self.getDimension())]

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
        Returns the covariance matrix of the mixutr
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

    def computeCharacteristicFunction(self, point):
        """
        Return the characteristic function evaluated on point.

        Parameters
        ----------
        criterion : point
                    1D array-like (np array, python list, OpenTURNS NumericalPoint)

        Returns
        -------
        out : Complex
              The characteristic function evaluated on point

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
        raise RuntimeError( 'You must define a method computeCharacteristicFunction(x) -> cfx, where cfx is a float' )

    def getRealization(self):
        """
        Get a realization of the distribution
        """
        realization = [dist.getRealization()[0] for dist in self.collection]
        realization = self.matrix * realization + self.y0
        return realization

    def getSample(self, n):
        """
        Get a realization of the distribution
        """
        assert isinstance(n, int)
        sample = ot.ComposedDistribution(self.collection).getSample(n)
        # product matrix * realization
        # using scipy for scalability (matrix * sample not available)
        sample = scipy.array(sample) * scipy.matrix(self.matrix).transpose()
        # scipy.matrix could not be casted into ot.NumericalSample
        sample = scipy.array(sample)
        sample = ot.NumericalSample(sample)
        # Do not forget the constant term
        sample.translate(self.y0)
        return sample

    def computePDF(self, point, xMin, xMax, pointNumber, precision):
        raise RuntimeError( 'You must define a method computePDF(x) -> pdf, where pdf is a float' )

    def computeCDF(self, X):
        raise RuntimeError( 'You must define a method computePDF(x) -> cdf, where cdf is a float' )

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        return 'PythonMultivariateRandomMixture distribution. Dimension =  %d' % self.getDimension()

class MultivariateRandomMixture(ot.Distribution):
    """
    MultivariateRandomMixture allows to build an OpenTURNS distribution

    """
    def __new__(self, collection, matrix, y0):
        instance = PythonMultivariateRandomMixture(collection, matrix, y0)
        return ot.Distribution(instance)
