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
mvrm_resource_map = {}
mvrm_resource_map.setdefault("MultivariateRandomMixture-DefaultAlpha", 4)
mvrm_resource_map.setdefault("MultivariateRandomMixture-DefaultBeta",  8)
mvrm_resource_map.setdefault("MultivariateRandomMixture-DefaultPDFEpsilon",\
ot.ResourceMap.GetAsNumericalScalar("DistributionImplementation-DefaultPDFEpsilon"))

class PythonMultivariateRandomMixture(ot.PythonDistribution):
    """
    Multivariate distribution
    Modelization of a multivariate random mixture distribution of form:
    Y = y_0 + M X, Y of size d
    where M is a d x n matrix, X is a n-random vector with independent components and y_0 a deterministic vector.
    This is a generalization of the RandomMixture distribution.

    Its moments either some realizations are easy to get. The characteristic function could be deduced easily using the
    independance of X's marginals.
    The probability density function (p.d.f) is computed using the Poisson summation formula :
    \sum_{j_1 \in \mathbb{Z}}... \sum_{j_d \in \mathbb{Z}} p(y_1 + \frac{2\pi j_1}{h_1},...,y_d + \frac{2\pi j_d}{h_d}) =
    \frac{h_1 x... x h_d}{2^d \pi^d} \sum_{k_1 \in \mathbb{Z}}... \sum_{k_d \in \mathbb{Z}} \phi(k_1 h_1,...,k_d h_d) * exp(-i(k_1 h_1+...+k_d h_d))

    """

    def __init__(self, collection, matrix, constant = None):
        """
        Parameters
        ----------
        collection : list/DistributionCollection
                     Either OpenTURNS DistributionCollection or a list of OpenTURNS distributions
        matrix : Matrix
                 Either OpenTURNS matrix or Numpy matrix
        constant : 1D array-like
             Either a python list, an OpenTURNS NumericalPoint or a Numpy 1D-array

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(), ot.Uniform()])
        >>> matrix = ot.Matrix([[1,1], [3,4], [2, -1]])
        >>> constant = [0, 5]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)

        """
        n = len(collection)
        if (n == 0):
            raise ValueError('Expected collection of distributions with non null size')
        # Use of setDistributionCollection method
        self.setDistributionCollection(collection)
        # matrix cast
        # As object might be a numpy class, we convert into
        # ot type before checking
        # Also, if the object is of type SquareMatrix,
        # the getNbColumns/getNbRows methods are not available
        self.matrix_ = ot.Matrix(matrix)
        # Check matrix dimension
        if(self.matrix_.getNbColumns() != n):
            raise ValueError("Matrix number of columns is not coherant with collection size.")
        d = self.matrix_.getNbRows()
        if (d > 3):
            raise ValueError("Mixture should be of dimension 1, 2 or 3")
        if (constant is None):
            self.constant_ = ot.NumericalPoint(d * [0.0])
        else :
            assert len(constant) == d
            self.constant_ = ot.NumericalPoint(constant)
        # Set the distribution dimension
        ot.PythonDistribution.__init__(self, d)
        # Set alpha and beta values from the resource map
        self.alpha_ = mvrm_resource_map['MultivariateRandomMixture-DefaultAlpha']
        self.beta_ = mvrm_resource_map['MultivariateRandomMixture-DefaultBeta']
        self.pdfEpsilon_ = mvrm_resource_map["MultivariateRandomMixture-DefaultPDFEpsilon"]
        # compute the mean and covariance
        # as mean is easy, no need to use isComputedMean attributs
        self.computeMean()
        self.computeCovariance()
        # set the standard deviation
        self.sigma_ = ot.NumericalPoint([cmath.sqrt(self.cov_[k, k]).real for k in xrange(d)])
        # compute the range
        self.computeRange()
        # compute h parameters for the evaluation of the density function
        self.computeH()
        # set equivalent Normal distribution, i.e a normal distribution with mean = self.mean_
        # and covariance = self.cov_
        self.computeEquivalentNormal()

    def __repr__(self):
        """
        Resume print of the distribution
        """
        return 'PythonMultivariateRandomMixture distribution. Dimension =  %d' % self.getDimension()

    def __str__(self):
        """
        pretty print of the distribution
        """
        s = str()
        for k in xrange(self.getDimension()):
            s += "Y_%d: %1.3e + " %(k+1, self.constant_[k])
            for j in xrange(len(self.collection_)):
                s += "%1.3e * %s" %(self.matrix_[k, j], str(self.collection_[j]))
                if j < len(self.collection_) - 1:
                    s += " + "
                else :
                    s += "\n"
        return s

    def computeCovariance(self):
        """
        Returns the covariance of the mixture
        This method is implicite. Use the getCovariance to get the mean value
        """
        cov = ot.ComposedDistribution(self.collection_).getCovariance()
        m1 = cov *  self.matrix_.transpose()
        cov = self.matrix_ * m1
        self.cov_ = ot.CovarianceMatrix(cov.getImplementation())

    def computeEquivalentNormal(self):
        """
        Compute the equivalent Normal distribution,
        i.e a normal distribution with mean = self.mean
        and covariance = self.covariance
        """
        self.equivalentNormal_ = ot.Normal(self.getMean(), self.getCovariance())

    def computeEquivalentNormalPDFSum(self, point):
        """
        Compute the left-hand sum in Poisson's summation formula
        for the equivalent normal
        """
        gaussian_pdf = self.equivalentNormal_.computePDF(point)
        i = 0
        delta = 0.0
        pi = cmath.pi
        condition = True
        d = self.getDimension()
        # corners to be taken into account
        corners = []
        if d == 3 :
            for ix in [-1, 1]:
                for iy in [-1, 1]:
                    for iz in [-1, 1]:
                        hx, hy, hz = tuple(self.h)
                        step = [2.0 * pi * ix / hx, 2.0 * pi * iy / hy, 2.0 * pi * iz / hz]
                        corners.append(ot.NumericalPoint(step))
        elif d == 2 :
            for ix in [-1, 1]:
                for iy in [-1, 1]:
                    hx, hy = tuple(self.h)
                    step = [2.0 * pi * ix / hx, 2.0 * pi * iy / hy]
                    corners.append(ot.NumericalPoint(step))
        elif d == 1 :
            for ix in [-1, 1]:
                  hx = self.h[0]
                  step = [2.0 * pi * ix / hx]
                  corners.append(ot.NumericalPoint(step))
        while (condition):
            i = i + 1
            for step in corners:
                delta = self.equivalentNormal_.computePDF(ot.NumericalPoint(point) + (step * float(i)))
                gaussian_pdf += delta
            error = delta > gaussian_pdf * self.pdfEpsilon_
            condition = error
        return gaussian_pdf

    def computeH(self):
        """
        Compute h parameters
        """
        self.h = [2.0 * cmath.pi / ((self.beta_ + 4.0 * self.alpha_) * self.sigma_[l]) for l in xrange(self.getDimension())]

    def computeMean(self):
        """
        Compute the mean of the multivariate mixture
        This method is implicite. Use the getMean to get the mean value
        """
        mu = [dist.getMean()[0] for dist in self.collection_]
        self.mean_ = self.matrix_ * ot.NumericalPoint(mu) + self.constant_

    def computeRange(self):
        """
        Compute the range of the distribution
        """
        # Elements of the bounds
        lower_bounds = []
        finite_lower_bounds = []
        upper_bounds = []
        finite_upper_bounds = []
        for i in xrange(self.getDimension()):
            interval = ot.Interval(self.constant_[i], self.constant_[i])
            for j in xrange(len(self.collection_)):
                interval += self.collection_[j].getRange() * self.matrix_[i, j]
            # Set the i-th element of the interval
            lower_bounds.append(interval.getLowerBound()[0])
            finite_lower_bounds.append(bool(interval.getFiniteLowerBound()[0]))
            upper_bounds.append(interval.getUpperBound()[0])
            finite_upper_bounds.append(bool(interval.getFiniteUpperBound()[0]))
        # The d-interval
        self.interval_ = ot.Interval(lower_bounds, upper_bounds, finite_lower_bounds, finite_upper_bounds)
        # We build an "equivalent" gaussian with mean, sigma values
        # We take into account the intersect of the interval computed and mu -/+ beta * sigma
        # Diagonal elements of the sigma matrix
        s = ot.NumericalPoint([self.sigma_[k] for k in xrange(self.getDimension())])
        gaussian_interval = ot.Interval(self.getMean() - s * self.beta_, self.getMean() + s * self.beta_)
        self.interval_ = self.interval_.intersect(gaussian_interval)

    def setDistributionCollection(self, collection):
        """
        Set the distribution collection
        This method should not be used, except by the __init__ method
        """
        for k in xrange(len(collection)):
            # check if distribution is univariate
            if (collection[k].getDimension() != 1):
                raise ValueError("Expected a collection of univariate distributions")
        self.collection_ = collection

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
        >>> collection = ot.DistributionCollection([ot.Normal(), ot.Uniform()])
        >>> matrix = ot.Matrix([[1,1], [3,4], [2, -1]])
        >>> constant = [0, 5]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> cfx = dist.computeCharacteristicFunction( [0.3, 0.9] )

        """
        assert len(u) == self.getDimension()
        somme = float()
        # The characteristic function is given by the following formula:
        # \phi(u) = \prod_{j=1}^{d} (exp(i * u_j * constant_j) * \prod_{k=1}^{n} \phi_{X_k}(Mjk u_j))
        n = len(self.collection_)
        d = self.getDimension()
        mt = self.matrix_.transpose()
        # compute M^t * u
        mt_u = mt * u
        # compute the deterministic term
        for j in xrange(d):
            somme += u[j] * self.constant_[j] * 1j
        # compute the random part
        # The variables are independent
        for k in xrange(n):
            somme += self.collection_[k].computeLogCharacteristicFunction(mt_u[k])
        return cmath.exp(somme)

    def computePDF(self, u):
        """
        Return the probability density function evaluated on u.

        Parameters
        ----------
        u :  vector of size d
             1D array-like (np array, python list, OpenTURNS NumericalPoint)

        Returns
        -------
        out : Scalar
              The density function evaluated on u

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(), ot.Uniform()])
        >>> matrix = ot.Matrix([[1,1], [3,4], [2, -1]])
        >>> constant = [0, 5]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> pdf = dist.computePDF( [0.3, 0.9] )

        """
        assert len(u) == self.getDimension()
        # Check if u is in domain
        if not self.interval_.contains(u):
            return 0.0
        # General case : two different steps
        # 1) Compute a gaussian pdf approximation
        value = self.computeEquivalentNormalPDFSum(u)
        # 2) Compute a difference of characteristic functions on the point u
        # TODO The method is not 
        return value

    def getConstant(self):
        """
        Returns the constant vector
        """
        return self.constant_

    def getDistributionCollection(self):
        """
        Returns the distribution collection
        """
        return self.collection_

    def getMatrix(self):
        """
        Returns the matrix of the affine transform
        """
        return self.matrix_

    def getMean(self):
        """
        Returns the mean vector of the mixture
        """
        return self.mean_

    def getCovariance(self):
        """
        Returns the covariance matrix of the mixutre
        """
        return self.cov_

    def getRange(self):
        """
        Returns the range of the distribution
        """
        return self.interval_

    def getRealization(self):
        """
        Get a realization of the distribution

        Parameters
        ----------
        None

        Returns
        -------
        out : OpenTURNS NumericalPoint
              Realization of the distribution

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(), ot.Uniform()])
        >>> matrix = ot.Matrix([[1,1], [3,4], [2, -1]])
        >>> constant = [0, 5]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> realization = dist.getRealization()

        """
        realization = [dist.getRealization()[0] for dist in self.collection_]
        realization = self.matrix_ * realization + self.constant_
        return realization

    def getSample(self, n):
        """
        Get a sample of size n of the distribution

        Parameters
        ----------
        n : integer
            Number of realizations

        Returns
        -------
        out : OpenTURNS NumericalSample
              Sample of realization of the distribution, of size n

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(), ot.Uniform()])
        >>> matrix = ot.Matrix([[1,1], [3,4], [2, -1]])
        >>> constant = [0, 5]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> sample = dist.getSample(100)
        """
        assert isinstance(n, int)
        sample = ot.ComposedDistribution(self.collection_).getSample(n)
        # product matrix * realization
        # using np for scalability (matrix * sample not available)
        try :
            import numpy as np
            sample = np.array(sample) * np.matrix(self.matrix_).transpose()
            # np.matrix could not be casted into ot.NumericalSample
            sample = np.array(sample)
            sample = ot.NumericalSample(sample)
        except ImportError :
            # matrix * np for each point
            sample = ot.NumericalSample([self.matrix_ * sample_element for sample_element in sample])
        # Do not forget the constant term
        # optimization : constant size is usually negligible compared to the sample size
        if self.constant_.norm() != 0:
            sample.translate(self.constant_)
        return sample

    def getStandardDeviation(self):
        """
        Returns the standard deviation
        """
        return self.sigma_

class MultivariateRandomMixture(ot.Distribution):
    """
    MultivariateRandomMixture allows to build an OpenTURNS distribution

    """
    def __new__(self, collection, matrix, constant):
        instance = PythonMultivariateRandomMixture(collection, matrix, constant)
        return ot.Distribution(instance)
