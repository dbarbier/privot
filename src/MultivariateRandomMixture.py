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
import MultivariateGaussianCharacteristicFunction as mvgc

# Dictionary equivalent to a resource map
mvrm_resource_map = {}
mvrm_resource_map.setdefault( "MultivariateRandomMixture-DefaultBlockMin", 3 )
mvrm_resource_map.setdefault( "MultivariateRandomMixture-DefaultBlockMax", 16 )
mvrm_resource_map.setdefault( "MultivariateRandomMixture-DefaultMaxSize", 65536 )
mvrm_resource_map.setdefault( "MultivariateRandomMixture-DefaultAlpha", 4.0 )
mvrm_resource_map.setdefault("MultivariateRandomMixture-DefaultBeta",  8.0)
mvrm_resource_map.setdefault( "MultivariateRandomMixture-DefaultPDFEpsilon", 1.0e-10 )
mvrm_resource_map.setdefault( "MultivariateRandomMixture-DefaultCDFEpsilon", 1.0e-10 )
mvrm_resource_map.setdefault( "MultivariateRandomMixture-SmallSize", 100 )

class PythonMultivariateRandomMixture(ot.PythonDistribution):
    """
    Multivariate distribution
    The objective of the python class is the modelization of a multivariate random vector as a
    multivariate mixture of form:
    Y = y_0 + M X
    where: Y of size d
           X is a n-random vector with independent components, i.e. a collection of univariate distributions,
           M is a (d x n) deterministic matrix, i.e. the linear operator of the affine transformation,
           y_0 a constant and deterministic vector,  i.e the constant part of the affine transformation.
    The distribution is a generalization of the unidimensional RandomMixture distribution.

    As the purposes of the distribution is mainly the evaluation of the density function and its visualization,
    the dimension may not exceed 3. This probability density function (p.d.f) is computed using the Poisson summation formula :
    \sum_{j_1 \in \mathbb{Z}}... \sum_{j_d \in \mathbb{Z}} p(y_1 + \frac{2\pi j_1}{h_1},...,y_d + \frac{2\pi j_d}{h_d}) =
    \frac{h_1 x... x h_d}{2^d \pi^d} \sum_{k_1 \in \mathbb{Z}}... \sum_{k_d \in \mathbb{Z}} \phi(k_1 h_1,...,k_d h_d) * exp(-i(k_1 h_1+...+k_d h_d))
    Using small values of h, such as p(x+h) << p(x), we could evaluate the density function with few terms.
    The characteristic function could be deduced easily using the independance of X's marginals:
      phi(y_1,...,y_d) = \prod_{j=1}^d {\imath y_j {y_0}_j} \prod_{k=1}^n (\phi_{X_k})((M^t y)_j)
    The first moments are analytically given as follows:
      - E(Y) = M * E(X)
      - Cov(Y) = M * Cov(X) * M^t
    As Cov(X) is diagonal:
      - Cov(Y)_{i,j} = \sum_{k=1}^n M_{i,k} M_{j,k} Cov(X_k, X_k)

    """

    def __init__(self, collection, matrix, constant = None):
        """
        Parameters
        ----------
        collection : list/DistributionCollection
                     Either OpenTURNS DistributionCollection or a list of OpenTURNS distributions
        matrix : Matrix
                 Either OpenTURNS matrix or Numpy matrix
                 It corresponds to the matrix operator of the affine transformation
        constant : 1D array-like
             Either a python list, an OpenTURNS NumericalPoint or a Numpy 1D-array
             It corresponds to the constant vector of the affine transformation

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)

        """
        size = len(collection)
        if (size == 0):
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
        # the operator of the transformation should have the same number of
        # columns as the collection size
        if(self.matrix_.getNbColumns() != size):
            raise ValueError("Matrix number of columns is not coherant with collection size.")
        d = self.matrix_.getNbRows()
        if (d > 3):
            raise ValueError("Mixture should be of dimension 1, 2 or 3")
        if (constant is None):
            # setting the y_0 term (constant term)
            self.constant_ = ot.NumericalPoint(d * [0.0])
        else :
            assert len(constant) == d
            self.constant_ = ot.NumericalPoint(constant)
        # Set the distribution dimension
        ot.PythonDistribution.__init__(self, d)
        # Set constants values using default parameters
        # alpha and beta are used for the range
        self.alpha_ = mvrm_resource_map["MultivariateRandomMixture-DefaultAlpha"]
        self.beta_ = mvrm_resource_map["MultivariateRandomMixture-DefaultBeta"]
        # pdfEpsilon, blockMax, blockMin and maxSize are used for the evaluation of the density function
        self.pdfEpsilon_ = mvrm_resource_map["MultivariateRandomMixture-DefaultPDFEpsilon"]
        self.blockMin_ = mvrm_resource_map["MultivariateRandomMixture-DefaultBlockMin"]
        self.blockMax_ = mvrm_resource_map["MultivariateRandomMixture-DefaultBlockMax"]
        self.maxSize_ = mvrm_resource_map["MultivariateRandomMixture-DefaultMaxSize"]
        # Cache for the characteristic function values
        self.characteristicValuesCache_ = []
        # compute the mean and covariance
        # as mean is easy, no need to use isComputedMean attributs
        self.computeMean()
        self.computeCovariance()
        # set the standard deviation
        self.sigma_ = ot.NumericalPoint([cmath.sqrt(self.cov_[k, k]).real for k in xrange(d)])
        # compute the range
        self.computeRange()
        # compute h parameters for the evaluation of the density function
        self.computeReferenceBandwidth()
        # set equivalent Normal distribution, i.e a normal distribution with mean = self.mean_
        # and covariance = self.cov_
        self.computeEquivalentNormal()

    def __repr__(self):
        """
        Resume print of the distribution
        """
        s = 'class=PythonMultivariateRandomMixture,dimension=%d,' % self.getDimension()
        s += 'collection=%s, matrix=%s, constant=%s' %(repr(self.collection_), repr(self.matrix_), repr(self.constant_))
        return s

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
        This method is implicite. Use the getCovariance to get the covariance value
        The covariance is given by
          Cov(Y) = M * Cov(X) * M^t
        As Cov(X) is diagonal:
          Cov(Y)_{i,j} = \sum_{k=1}^n M_{i,k} M_{j,k} Cov(X_k, X_k)

        """
        d = self.getDimension()
        self.cov_ = ot.CovarianceMatrix(d)
        for i in xrange(d):
            for j in xrange(i + 1):
                s = 0.0
                for k in xrange(len(self.collection_)):
                    s += self.matrix_[i, k] * self.matrix_[j, k] * self.collection_[k].getCovariance()[0, 0]
                self.cov_[i, j] = s

    def computeDeltaCharacteristicFunction(self, x):
        """
        Returns the differences of characteristic functions
        This method is implicite and should not be used outside the interla methods
        The current method does not implement the cache mechanism
        The method uses also an external class for the evaluation of the characteristic function
        for multivariate gaussian distribution
        """
        return self.computeCharacteristicFunction(x) - mvgc.MGCF(self.equivalentNormal_, x)

    def computeEquivalentNormal(self):
        """
        Compute the equivalent Normal distribution,
        i.e a normal distribution with mean = self.mean
        and covariance = self.covariance
        """
        self.equivalentNormal_ = ot.Normal(self.getMean(), self.getCovariance())

    def computeEquivalentNormalPDFSum(self, y):
        """
        Compute the left-hand sum in Poisson's summation formula for the equivalent normal.
        The interest is to compute :
          \sum_{i_1 \in \mathbb{Z}^d} q(y + i * h) with :
            q = the equivalent gaussian density function (gaussian in \mathbb{R}^d with
            \mu = self.mean and \sigma_ = self.sigma
            i = (i_1,...,i_d) : the multi-indices
            y = (y_1,...,y_d) : point on which we want to compute the pdf
            h = (h_1,...,h_d) : the reference bandwidth
            i*h = (i_1 * h_1,...,i_d * h_d)
         We start with i = (0,...,0) and at each iteration #it, we add the points such as
         all elements of i are not lower as the iteration #it
         The point should not have been taken into account in #it - 1, that is to say
         there are (2j+1)^d -(2j-1)^d points to add, d is the dimension of the distribution.
         We add the evaluation of the gaussian density in these points into the current
         sum. We stop the algorithm if the added value is negligible compared to the current density
         relative value (pdf * precision)

        """
        gaussian_pdf = self.equivalentNormal_.computePDF(y)
        i = 0
        two_pi = 2.0 * cmath.pi
        d = self.getDimension()
        two_pi_on_h = [two_pi / element for element in self.referenceBandwidth_]
        condition = True
        while (condition):
            i = i + 1
            delta = 0.0
            walker = self.get_points_on_surface_grid_(i)
            try:
                while True:
                    point = walker.next()
                    x = [y[k] + two_pi_on_h[k] * point[k] for k in range(d)]
                    delta += self.equivalentNormal_.computePDF(ot.NumericalPoint(x))
            except StopIteration:
                pass
            gaussian_pdf += delta
            error = delta > gaussian_pdf * self.pdfEpsilon_
            condition = error
        return gaussian_pdf

    def computeReferenceBandwidth(self):
        """
        Compute the reference bandwidth h. It is defined as the maximum bandwidth
        that allow a precise computation of the PDF over the range
        [mean +/- beta * sigma_]
        """
        self.referenceBandwidth_ = [2.0 * cmath.pi / ((self.beta_ + 4.0 * self.alpha_) * self.sigma_[l]) for l in xrange(self.getDimension())]

    def computeMean(self):
        """
        Compute the mean of the multivariate mixture
        This method is implicite. Use the getMean to get the mean value
        The mean is given by:
          E(Y) = M * E(X)

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

    def get_points_on_surface_grid_(self, index):
        """
        The method helps to get the points on which some functions should be evaluated
        These functions are respectively:
         1) The evaluation of the equivalent normal pdf sum
         2) The evaluation of delta characteristic functions
        The points corresponds to :
        \cup_{(i_1,..,i_d) \in [-index, index]^d} (i_1,...,i_d) \ \cup_{(i_1,..,i_d) \in [-index+1, index-1]^d} (i_1,...,i_d)
        """
        assert isinstance(index, int)
        d = self.getDimension()
        if d == 1 :
            # list_points should contains 2 points
            for ix in [-index, index]:
                yield (ix,)
        elif d == 2 :
            # In 2D case, we should take into account the contour line of a
            # bidimensional square, i.e we should take into account
            # all points with |x| = index, |y| = index
            # list_points should contains:
            # (2j+1)^2 - (2j-1)^2 = 8j^2 points
            for ix in xrange(-index, index + 1):
                inner_x = (abs(ix) < index)
                for iy in xrange(-index, index+1):
                    inner_y = (abs(iy) < index)
                    if not(inner_x and inner_y):
                        yield (ix, iy)
        elif d == 3 :
            # In 3D case, we should take into account the 6 faces of a
            # cube, i.e we should take into account
            # all points with |x| = index, |y| = index and |z| = index
            # list_points should contains:
            # (2j+1)^3 - (2j-1)^3 = 24j^2 + 2 points
            for ix in xrange(-index, index+1):
                inner_x = (abs(ix) < index)
                for iy in xrange(-index, index+1):
                    inner_y = (abs(iy) < index)
                    for iz in xrange(-index, index+1):
                        inner_z = (abs(iz) < index)
                        if not(inner_x and inner_y and inner_z):
                            yield (ix, iy, iz)

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

    def computeLogCharacteristicFunction(self, y):
        """
        Return the Log-characteristic function evaluated on y.
        The Log-characteristic function depends on the constant vector, the matrix and the log-characteristic
        functions of the input distribution collection like this:
          log\(phi)(y_1,...,y_d) = \sum_{j=1}^d {\imath y_j {y_0}_j} + \sum_{k=1}^n log(\phi_{X_k})((M^t y)_j)
        with \phi_{X_k}: the characteristic function of the k-th input distribution

        Parameters
        ----------
        y :  vector of size d
             1D array-like (np array, python list, OpenTURNS NumericalPoint)
             point on which the log-characteristic function will be evaluated

        Returns
        -------
        out : Complex
              The characteristic function evaluated on y

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> log_cf = dist.computeLogCharacteristicFunction( [0.3, 0.9] )

        """
        assert len(y) == self.getDimension()
        # The characteristic function is given by the following formula:
        # \phi(y) = \prod_{j=1}^{d} (exp(i * u_j * constant_j) * \prod_{k=1}^{n} \phi_{X_k}(Mjk u_j))
        n = len(self.collection_)
        d = self.getDimension()
        mt = self.matrix_.transpose()
        # compute M^t * u
        mt_y = mt * y
        # compute the deterministic term
        somme = ot.dot(y, self.constant_) * 1j
        # compute the random part
        # The variables are independent
        for k in xrange(n):
            somme += self.collection_[k].computeLogCharacteristicFunction(mt_y[k])
        return somme

    def computeCharacteristicFunction(self, y):
        """
        Return the characteristic function evaluated on y.
        The Log-characteristic function depends on the constant vector, the matrix and the characteristic
        functions of the input distribution collection like this:
          (phi)(y_1,...,y_d) = \prod_{j=1}^d {\imath y_j {y_0}_j} \prod_{k=1}^n (\phi_{X_k})((M^t y)_j)
        with \phi_{X_k}: the characteristic function of the k-th input distribution

        Parameters
        ----------
        y :  vector of size d
             1D array-like (np array, python list, OpenTURNS NumericalPoint)
             point on which the characteristic function will be evaluated

        Returns
        -------
        out : Complex
              The characteristic function evaluated on u

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> cf = dist.computeCharacteristicFunction( [0.3, 0.9] )

        """
        return cmath.exp(self.computeLogCharacteristicFunction(y))

    def computePDF(self, y):
        """
        Return the probability density function evaluated on y.
        It uses the Poisson inversion formula as described in the reference:
        "Abate, J. and Whitt, W. (1992). The Fourier-series method for inverting
        transforms of probability distributions. Queueing Systems 10, 5--88., 1992",
        formula 5.5.

        Here, we recall the Poisson summation formula:
        \sum_{j_1 \in \mathbb{Z}}... \sum_{j_d \in \mathbb{Z}} p(y_1 + 2 pi j_1/h_1,...,y_d+ 2 \pi j_d/h_d) =
        h_1...h_d/(2^d \pi^d) \sum_{k_1 \in \mathbb{Z}}... \sum_{k_d \in \mathbb{Z}} \phi(k_1 h_1,...,k_d h_d) * exp(-i(k_1 h_1+...+k_d h_d))

        We can rewrite this formula as:
        \sum_{j_1 \in \mathbb{Z}}... \sum_{j_d \in \mathbb{Z}} p(y_1 + 2 pi j_1/h_1,...,y_d+ 2 \pi j_d/h_d) =
        \sum_{j_1 \in \mathbb{Z}}... \sum_{j_d \in \mathbb{Z}} q(y_1 + 2 pi j_1/h_1,...,y_d+ 2 \pi j_d/h_d) +
        h_1...h_d/(2^d \pi^d) \sum_{k_1 \in \mathbb{Z}}... \sum_{k_d \in \mathbb{Z}} (\phi(k_1 h_1,...,k_d h_d) -
        \psi(k_1 h_1,...,k_d h_d)) * exp(-i(k_1 h_1+...+k_d h_d))
        where q is the PDF and \psi the characteristic function of the multivariate normal distribution with the same mean and
        the same variance as the curernt multivariate mixture.
        We take h such as p(x_1+2k_1\pi/h_1,...,x_d+2k_d\pi/h_d) << p(x_1,...,x_d) for k\neq 0, then:
        p(x) \simeq \sum_{j_1 \in \mathbb{Z}}... \sum_{j_d \in \mathbb{Z}} q(y_1 + 2 pi j_1/h_1,...,y_d+ 2 \pi j_d/h_d) +
        h_1...h_d/(2^d \pi^d) \sum_{k_1 \in \mathbb{Z}}... \sum_{k_d \in \mathbb{Z}} (\phi(k_1 h_1,...,k_d h_d) -
        \psi(k_1 h_1,...,k_d h_d)) * exp(-i(k_1 h_1+...+k_d h_d))

        The first sum \sum_{k\in Z^d}q(x+2k\pi/h) will be approximated using only few terms, as the condition on h will almost
        gives q(x_1+2k_1\pi/h_1, x_d+2k_d\pi/h_d) << q(x) for k\neq 0.
        Call this sum Q(x, h), and define \delta as delta(u) = \phi(u) - \psi(u), u \in \mathbb{R}^d.
        The first term is computed thanks to the internal computeEquivalentNormalPDFSum method whereas the second term (delta)
        is partially computed with the computeDeltaCharacteristicFunction method

        Parameters
        ----------
        y :  vector of size d
             1D array-like (np array, python list, OpenTURNS NumericalPoint)
             The point on which we want the PDF

        Returns
        -------
        out : postivie or null scalar
              The density function evaluated on y

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> pdf = dist.computePDF( [0.3, 0.9] )

        """
        assert len(y) == self.getDimension()
        # Check if y is in domain
        if not self.interval_.contains(y):
            return 0.0
        # General case : two different steps
        # 1) Compute a gaussian pdf approximation
        value = self.computeEquivalentNormalPDFSum(y)
        # 2) Compute a difference of characteristic functions on the point y
        # TODO The method is not yet implemented
        return value

    # alpha get-accessor
    def getAlpha(self):
        """
        Returns the alpha parameter used for the evaluation
        of the range
        """
        return self.alpha_

    # beta get-accessor
    def getBeta(self):
        """
        Returns the beta parameter used for the evaluation
        of the range
        """
        return self.beta_

    # blockMax get-accessor
    def getBlockMax(self):
        """
        Returns the blockMax parameter used for the evaluation
        of the pdf using the gaussian approximation
        """
        return self.blockMax_

    # blockMin get-accessor
    def getBlockMin(self):
        """
        Returns the blockMin parameter used for the evaluation
        of the pdf using the gaussian approximation
        """
        return self.blockMin_

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

    # maxSize get-accessor
    def getMaxSize(self):
        """
        Returns the maximum size of the cache for the CharacteristicFunction values
        """
        return self.maxSize_

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
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> realization = dist.getRealization()

        """
        realization = [dist.getRealization()[0] for dist in self.collection_]
        realization = self.matrix_ * realization + self.constant_
        return realization

    # bandwidth get-accessor
    def getReferenceBandwidth(self):
        """
        Returns the bandwidth parameter used for the evaluation
        of the pdf using the gaussian approximation
        """
        return self.referenceBandwidth_

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
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
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

    # alpha set-accessor
    def setAlpha(self, alpha):
        """
        Returns the alpha parameter used for the evaluation
        of the range
        """
        self.alpha_ = alpha

    # beta set-accessor
    def setBeta(self, beta):
        """
        Returns the beta parameter used for the evaluation
        of the range
        """
        self.beta_ = beta

    # BlockMax accessors
    def setBlockMax(self, blockMax):
        """
        Set the blockMax parameter used for the evaluation
        of the pdf using the gaussian approximation
        """
        self.blockMax_ = blockMax

    # BlockMin accessors
    def setBlockMin(self, blockMin):
        """
        Set the blockMin parameter used for the evaluation
        of the pdf using the gaussian approximation
        """
        self.blockMin_ = blockMin

    # MaxSize set-accessor
    def setMaxSize(self, maxSize):
        """
        Returns the maximum size of the cache for the CharacteristicFunction values
        """
        self.maxSize_ = maxSize

    # Reference bandwidth set-accessor
    def setReferenceBandwidth(self, bandwidth):
        """
        Set the blockMin parameter used for the evaluation
        of the pdf using the gaussian approximation
        """
        self.referenceBandwidth_ = bandwidth


class MultivariateRandomMixture(ot.Distribution):
    """
    MultivariateRandomMixture allows to build an OpenTURNS distribution

    """
    def __new__(self, collection, matrix, constant):
        instance = PythonMultivariateRandomMixture(collection, matrix, constant)
        return ot.Distribution(instance)
