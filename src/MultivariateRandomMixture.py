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
import numpy as np
import MaxNormMeshGrid

# ResourceMap : setting different numerical parameters useful for the distribution
ot.ResourceMap.SetAsUnsignedLong("MultivariateRandomMixture-DefaultBlockMin", 3)
ot.ResourceMap.SetAsUnsignedLong("MultivariateRandomMixture-DefaultBlockMax", 16)
ot.ResourceMap.SetAsUnsignedLong("MultivariateRandomMixture-DefaultCacheSize", 4000000)
ot.ResourceMap.SetAsNumericalScalar("MultivariateRandomMixture-DefaultAlpha", 4.0)
ot.ResourceMap.SetAsNumericalScalar("MultivariateRandomMixture-DefaultBeta",  8.0)
ot.ResourceMap.SetAsNumericalScalar("MultivariateRandomMixture-DefaultPDFPrecision", 1.0e-10)
ot.ResourceMap.SetAsNumericalScalar("MultivariateRandomMixture-DefaultCDFPrecision", 1.0e-10)
ot.ResourceMap.SetAsNumericalScalar("MultivariateRandomMixture-DefaultPDFEpsilon", 1.0e-14)
ot.ResourceMap.SetAsNumericalScalar("MultivariateRandomMixture-DefaultCDFEpsilon", 1.0e-14)
ot.ResourceMap.SetAsNumericalScalar("MultivariateRandomMixture-SmallSize", 100)


class PythonMultivariateRandomMixture(ot.PythonDistribution):
    """
    Multivariate distribution
    The objective of the python class is the modelization of a multivariate random vector as a
    multivariate mixture of form:
    Y = y_0 + M X
    where: Y of size d, d\in {1,2,3}
           X is a n-random vector with independent components, i.e. a collection of univariate distributions,
           M is a (d x n) deterministic matrix, i.e. the linear operator of the affine transformation,
           y_0 a constant and deterministic vector,  i.e the constant part of the affine transformation.
    The distribution is a generalization of the unidimensional RandomMixture distribution.

    Its probability density function (PDF) is computed using the Poisson summation formula :
    \sum_{j_1 \in \mathbb{Z}}... \sum_{j_d \in \mathbb{Z}} p(y_1 + 2\pi j_1/h_1,...,y_d + 2\pi j_d/h_d) =
    (h_1...h_d)/(2^d \pi^d) \sum_{k_1 \in \mathbb{Z}}... \sum_{k_d \in \mathbb{Z}} \phi(k_1 h_1,...,k_d h_d) * exp(-i(k_1 h_1+...+k_d h_d))
    The characteristic function could be deduced easily using the independance of X's marginals:
      phi(y_1,...,y_d) = \prod_{j=1}^d {\imath y_j {y_0}_j} \prod_{k=1}^n (\phi_{X_k})((M^t y)_j)
    The algorithm could be performed using a gaussian approximation. Indeed, the Poisson formula is linear so by substracting the
    multivariate normal distribution with the same mean and the same variance as the curernt multivariate mixture density and
    characteristic functions (denoted respectively by q and \psi), the formula becomes:
    \sum_{j_1 \in \mathbb{Z}}... \sum_{j_d \in \mathbb{Z}} p(y_1 + 2 pi j_1/h_1,...,y_d+ 2 \pi j_d/h_d) =
    \sum_{j_1 \in \mathbb{Z}}... \sum_{j_d \in \mathbb{Z}} q(y_1 + 2 pi j_1/h_1,...,y_d+ 2 \pi j_d/h_d) +
    h_1...h_d/(2^d \pi^d) \sum_{k_1 \in \mathbb{Z}}... \sum_{k_d \in \mathbb{Z}} (\phi(k_1 h_1,...,k_d h_d) -
    \psi(k_1 h_1,...,k_d h_d)) * exp(-i(k_1 h_1+...+k_d h_d))
    Using small values of h, such as p(x+h) << p(x), we could evaluate the density function with few terms.

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
        collection : Either an OpenTURNS DistributionCollection or a list of OpenTURNS distributions
                     The modelization of the input random vector X. The collection should contains
                     n univariate distribution

        matrix : Either OpenTURNS matrix or Numpy matrix
                 It corresponds to the matrix operator of the affine transformation

        constant : 1D array-like (either a python list, an OpenTURNS NumericalPoint
                   or a Numpy 1D-array)
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
        # Use of the setMatrix method
        self.setMatrix(matrix)
        # Use of the setConstant method
        self.setConstant(constant)
        # Set the distribution dimension
        self.dimension_ = len(self.constant_)
        ot.PythonDistribution.__init__(self, self.dimension_)
        # Set constants values using default parameters
        # alpha and beta are used for the range
        self.alpha_ = ot.ResourceMap.GetAsNumericalScalar("MultivariateRandomMixture-DefaultAlpha")
        self.beta_ = ot.ResourceMap.GetAsNumericalScalar("MultivariateRandomMixture-DefaultBeta")
        # pdfEpsilon, blockMax, blockMin and maxSize are used for the evaluation of the density function
        self.pdfEpsilon_ = ot.ResourceMap.GetAsNumericalScalar("MultivariateRandomMixture-DefaultPDFEpsilon")
        self.pdfPrecision_ = ot.ResourceMap.GetAsNumericalScalar("MultivariateRandomMixture-DefaultPDFPrecision")
        # PDF error : the last error obtained with the calculation of the PDF
        self.pdfError_ = 0.0
        self.blockMin_ = ot.ResourceMap.GetAsUnsignedLong("MultivariateRandomMixture-DefaultBlockMin")
        self.blockMax_ = ot.ResourceMap.GetAsUnsignedLong("MultivariateRandomMixture-DefaultBlockMax")
        # compute the mean and covariance
        # as mean is easy, no need to use isComputedMean attributs
        self.computeMean()
        self.computeCovariance()
        # compute the standard deviation
        self.computeStandardDeviation()
        # compute the correlation
        self.computeCorrelation()
        # compute the range
        self.computeRange()
        # compute h parameters for the evaluation of the density function
        self.computeReferenceBandwidth()
        # set equivalent Normal distribution, i.e a normal distribution with mean = self.mean_
        # and covariance = self.cov_
        self.computeEquivalentNormal()
        if len(self.referenceBandwidth_) == 1:
            gridMesherNd = MaxNormMeshGrid.Cube1D(self.referenceBandwidth_, symmetric=True)
        elif len(self.referenceBandwidth_) == 2:
            gridMesherNd = MaxNormMeshGrid.SkinCube2D(self.referenceBandwidth_, symmetric=True)
        elif len(self.referenceBandwidth_) == 3:
            gridMesherNd = MaxNormMeshGrid.SkinCube3D(self.referenceBandwidth_, symmetric=True)
        cacheSize = ot.ResourceMap.GetAsUnsignedLong("MultivariateRandomMixture-DefaultCacheSize")
        if cacheSize > 0:
            self.setGridMesher(MaxNormMeshGrid.CachedMeshGrid(gridMesherNd, size=cacheSize))
        else:
            self.setGridMesher(gridMesherNd)

    def __repr__(self):
        """
        Resume print of the distribution
        """
        s = 'class=PythonMultivariateRandomMixture,dimension=%d,' % self.dimension_
        s += 'collection=%s, matrix=%s, constant=%s' %(repr(self.collection_), repr(self.matrix_), repr(self.constant_))
        return s

    def __str__(self):
        """
        pretty print of the distribution
        """
        s = str()
        for k in xrange(self.dimension_):
            s += "Y_%d: %1.3e + " %(k+1, self.constant_[k])
            for j in xrange(len(self.collection_)):
                s += "%1.3e * %s" %(self.matrix_[k, j], str(self.collection_[j]))
                if j < len(self.collection_) - 1:
                    s += " + "
                else :
                    s += "\n"
        return s

    def computeCorrelation(self):
        """
        Compute the correlation matrix of the mixture.
        This method is private. Use the getCovariance to get the covariance value.
        The covariance is given by
          Cov(Y) = M * Cov(X) * M^t
        As Cov(X) is diagonal:
          Cov(Y)_{i,j} = \sum_{k=1}^n M_{i,k} M_{j,k} Cov(X_k, X_k)

        """
        self.corr_ = ot.CorrelationMatrix(self.dimension_)
        for i in xrange(self.dimension_):
            for j in xrange(0, i):
                self.corr_[i, j] = self.cov_[i, j] / (self.sigma_[i] * self.sigma_[j])

    def computeCovariance(self):
        """
        Compute the covariance of the mixture.
        This method is private. Use the getCovariance to get the covariance value.
        The covariance is given by
          Cov(Y) = M * Cov(X) * M^t
        As Cov(X) is diagonal:
          Cov(Y)_{i,j} = \sum_{k=1}^n M_{i,k} M_{j,k} Cov(X_k, X_k)

        """
        self.cov_ = ot.CovarianceMatrix(self.dimension_)
        for i in xrange(self.dimension_):
            for j in xrange(i + 1):
                s = 0.0
                for k in xrange(len(self.collection_)):
                    s += self.matrix_[i, k] * self.matrix_[j, k] * self.collection_[k].getCovariance()[0, 0]
                self.cov_[i, j] = s

    def computeDeltaCharacteristicFunction(self, x):
        """
        Returns the differences of characteristic functions.
        This method is private and should not be called directly.
        """
        return self.computeCharacteristicFunction(x) - self.equivalentNormal_.computeCharacteristicFunction(x)

    def computeEquivalentNormal(self):
        """
        Compute the equivalent Normal distribution, i.e a normal distribution with
        mean = self.mean and covariance = self.covariance
        """
        self.equivalentNormal_ = ot.Normal(self.getMean(), self.getCovariance())

    def computeEquivalentNormalPDFSum(self, y):
        """
        Compute the left-hand sum in Poisson's summation formula for the equivalent normal.
        The goal is to compute:
          \sum_{i \in \mathbb{Z}^d} q(y + i * h) with :
            y = (y_1,...,y_d) point on which the pdf is requested
            q = the density function of the distribution computed by computeEquivalentNormal
            h = (h_1,...,h_d) the reference bandwidth
            i*h = (i_1 * h_1,...,i_d * h_d)
         The sum above is rewritten as:
           \sum_{s \in \mathbb{N}} \sum_{x such as \norm{x-y}_\infinity=s} q(x)
         We start with s=0 and at each iteration, we add the points which are exactly at
         distance s with norm L^\infinity.
         If s>0, there are (2s+1)^d - (2s-1)^d points to add at iteration s.
         The evaluation of the gaussian density at these points are added into the current sum.
         The summation halts when the added value at iteration s is negligible relative to
         the current density value.

        """
        gaussian_pdf = self.equivalentNormal_.computePDF(y)
        i = 0
        condition = True
        while (condition):
            i = i + 1
            delta = 0.0
            iterator = self.meshAltGrid_.get_skin_iterator(i)
            try:
                while True:
                    point = iterator.next()
                    x = [y[k] + point[k] for k in xrange(self.dimension_)]
                    delta += self.equivalentNormal_.computePDF(x)
            except StopIteration:
                pass
            gaussian_pdf += delta
            error = delta > gaussian_pdf * self.pdfEpsilon_
            condition = error
        return gaussian_pdf

    def computeMean(self):
        """
        Compute the mean of the multivariate mixture.
        This method is private. Use the getMean method to get the mean value.
        It is given by:
          E(Y) = M * E(X)

        """
        mu = [dist.getMean()[0] for dist in self.collection_]
        self.mean_ = self.matrix_ * ot.NumericalPoint(mu) + self.constant_

    def computeReferenceBandwidth(self):
        """
        The method is private.
        It computes the reference bandwidth. It is defined as the maximum bandwidth
        that allows a precise computation of the PDF over the range
        [mean +/- beta * sigma_].

        """
        if (len(self.collection_) <= ot.ResourceMap.GetAsUnsignedLong( "MultivariateRandomMixture-SmallSize" )):
            self.referenceBandwidth_ = [2.0 * cmath.pi / (self.getRange().getUpperBound()[k] - self.getRange().getLowerBound()[k]) for k in xrange(self.dimension_)]
            # Shrink a little bit the bandwidth if the range is finite
            isFinite = [self.getRange().getFiniteLowerBound()[k] and self.getRange().getFiniteUpperBound()[k] for k in xrange(self.dimension_)]
            if (all(isFinite)):
                self.referenceBandwidth_ = [self.referenceBandwidth_[k] * 0.5 for k in xrange(self.dimension_)]
        # Else use a kind of Normal approximation
        else:
            self.referenceBandwidth_ = [2.0 * cmath.pi / ((self.beta_ + 4.0 * self.alpha_) * self.sigma_[l]) for l in xrange(self.dimension_)]

    def computeRange(self):
        """
        This methods is private.
        It computes the range of the distribution.

        """
        # Elements of the bounds
        lower_bounds = []
        finite_lower_bounds = []
        upper_bounds = []
        finite_upper_bounds = []
        for i in xrange(self.dimension_):
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
        s = ot.NumericalPoint([self.sigma_[k] for k in xrange(self.dimension_)])
        gaussian_interval = ot.Interval(self.getMean() - s * self.beta_, self.getMean() + s * self.beta_)
        self.interval_ = self.interval_.intersect(gaussian_interval)

    def computeStandardDeviation(self):
        """
        This methods is private.
        It computes the standard deviation of the distribution.

        """
        self.sigma_ = ot.NumericalPoint([cmath.sqrt(self.cov_[k, k]).real for k in xrange(self.dimension_)])

    def setConstant(self, constant):
        """
        This method is private.
        It sets the constant term of the affine transformation.

        """
        d = self.matrix_.getNbRows()
        if (constant is None):
            # setting the y_0 term (constant term)
            self.constant_ = ot.NumericalPoint(d * [0.0])
        else :
            assert len(constant) == d
            self.constant_ = ot.NumericalPoint(constant)

    def setMatrix(self, matrix):
        """
        This method is private.
        It sets the linear term of the affine transformation.

        """
        # matrix cast
        # As object might be a numpy class, we convert into
        # ot type before checking
        # Also, if the object is of type SquareMatrix,
        # the getNbColumns/getNbRows methods are not available
        self.matrix_ = ot.Matrix(matrix)
        self.matrixT_ = self.matrix_.transpose()
        # Check matrix dimension
        # the operator of the transformation should have the same number of
        # columns as the collection size
        size = len(self.collection_)
        if(self.matrix_.getNbColumns() != size):
            raise ValueError("Matrix number of columns is not coherant with collection size.")
        dimension = self.matrix_.getNbRows()
        if (dimension > 3):
            raise ValueError("Mixture should be of dimension 1, 2 or 3")
        # Checking valid size/dimension
        if dimension > size:
            raise ValueError("Dimension is greater than the collection of distributions: incoherant matrix's rank")
        # Checking rank of the matrix
        lam = self.matrix_.computeSingularValues()
        lam = list(lam)
        if lam.count(0):
            raise ValueError("Incomplete matrix's rank")
        # check if the matrix if square, we may get analytically the pdf estimate
        self.isAnalyticPDF_ = False
        self.matrixInverse_ = ot.Matrix()
        self.detMatrixInverse_ = 0.0
        if size == dimension:
            self.isAnalyticPDF_ = True
            self.matrixInverse_ = self.matrix_.solveLinearSystem(ot.IdentityMatrix(dimension))
            self.detMatrixInverse_ = self.matrixInverse_.getImplementation().computeDeterminant()

    def setDistributionCollection(self, collection):
        """
        Set the distribution collection.
        This method should not be used, except by the __init__ method.
        """
        distributions = []
        for k in xrange(len(collection)):
            # check if distribution is univariate
            if (collection[k].getDimension() != 1):
                raise ValueError("Expected a collection of univariate distributions")
            distributions.append(collection[k])
        self.collection_ = distributions

    def computeLogCharacteristicFunction(self, y):
        """
        Return the Log-characteristic function evaluated on y.
        The Log-characteristic function depends on the constant vector, the matrix and the log-characteristic
        functions of the input distribution collection like this:
          log\(phi)(y_1,...,y_d) = \sum_{j=1}^d {\imath y_j {y_0}_j} + \sum_{k=1}^n log(\phi_{X_k})((M^t y)_k)
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
        assert len(y) == self.dimension_
        # The characteristic function is given by the following formula:
        # \phi(y) = \prod_{j=1}^{d} (exp(i * u_j * constant_j) * \prod_{k=1}^{n} \phi_{X_k}((M^t u)_k))
        n = len(self.collection_)
        # compute M^t * y
        mt_y = self.matrixT_ * y
        # compute the deterministic term
        somme = ot.dot(y, self.constant_) * 1j
        smallScalar = 0.5 * ot.SpecFunc.LogMinNumericalScalar
        # compute the random part
        # The variables are independent
        for k in xrange(n):
            somme += self.collection_[k].computeLogCharacteristicFunction(mt_y[k])
            if somme.real < smallScalar:
                break
        return somme

    def computeCharacteristicFunction(self, y):
        """
        Return the characteristic function evaluated on y.
        The Log-characteristic function depends on the constant vector, the matrix and the characteristic
        functions of the input distribution collection like this:
          (phi)(y_1,...,y_d) = \prod_{j=1}^d {\imath y_j {y_0}_j} \prod_{k=1}^n (\phi_{X_k})((M^t y)_k)
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
        h_1...h_d/(2^d \pi^d) \sum_{k_1 \in \mathbb{Z}}... \sum_{k_d \in \mathbb{Z}} \phi(k_1 h_1,...,k_d h_d) exp(-i(k_1 h_1+...+k_d h_d))

        We can rewrite this formula as:
        \sum_{j_1 \in \mathbb{Z}}... \sum_{j_d \in \mathbb{Z}} p(y_1 + 2 \pi j_1/h_1,...,y_d+ 2 \pi j_d/h_d) =
        \sum_{j_1 \in \mathbb{Z}}... \sum_{j_d \in \mathbb{Z}} q(y_1 + 2 \pi j_1/h_1,...,y_d+ 2 \pi j_d/h_d) +
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
        Call this sum Q(x, h), and define \delta as \delta(u) = \phi(u) - \psi(u), u \in \mathbb{R}^d.
        The first term is computed thanks to the internal computeEquivalentNormalPDFSum method whereas the second term (\delta)
        is partially computed with the computeDeltaCharacteristicFunction method

        In case of square matrix, the density is analytical:
            pdf(x_1,...,x_d) = |det (M^(-1))| \prod_{j=1}^{n} pdf_j((M^(-1) * x)_j)

        Parameters
        ----------
        y :  vector of size d
             1D array-like (np array, python list, OpenTURNS NumericalPoint)
             The point on which we want the PDF

        Returns
        -------
        out : non-negative scalar
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
        assert len(y) == self.dimension_
        if self.isAnalyticPDF_:
            # compute analytically the pdf
            u = ot.NumericalPoint(y) - self.constant_
            Qu = self.matrixInverse_ * u
            pdf = abs(self.detMatrixInverse_)
            for j in xrange(self.dimension_):
                pdf *= self.collection_[j].computePDF(Qu[j])
            return pdf

        # Check if y is in domain
        if not self.interval_.contains(y):
            return 0.0
        # General case : two different steps
        # 1) Compute a gaussian pdf approximation
        value = self.computeEquivalentNormalPDFSum(y)
        # Compute the factor \prod_{k=1}^{d} h_k/'2\pi
        # h_k are supposed to be small values, we must care about
        # numerical troubles
        factor = 1.0
        for k in xrange(self.dimension_):
            factor *= self.referenceBandwidth_[k] / (2.0 * cmath.pi)
        # 2) Compute a difference of characteristic functions on the point y
        # sum of delta functions
        k = 1
        precision = self.pdfPrecision_
        # kmin is set to 2**self.blockMin_
        kmin = 1 << self.blockMin_
        # kmax is set to 2**self.blockMax_
        kmax = 1 << self.blockMax_
        # error is fixed here only for the while condition
        error = 2.0 * precision
        # The computation of delta function (\phi - \psi)
        while ((k < kmin) or ((k < kmax) and (error > precision))):
            # error fixed to 0
            error = 0.0
            # At each iteration of the while condition,
            # k calculations are done
            for m in xrange(k, 2*k):
                iterator = self.meshGrid_.get_skin_iterator_evaluate(m, self.computeDeltaCharacteristicFunction)
                try :
                    while True:
                        h, cfValue = iterator.next()
                        h_y = self.meshGrid_.dot(h, y)
                        cos_hy = cmath.cos(h_y).real
                        sin_hy = cmath.sin(h_y).real
                        error += cfValue.real * cos_hy + cfValue.imag * sin_hy
                except StopIteration:
                  pass
            error *= factor
            if self.meshGrid_.isSymmetric():
                error *= 2.0
            value += error
            error = abs(error)
            k *= 2
            # end of while
        # For very low levels of PDF, the computed value can be slightly negative. Round it up to zero.
        if (value < 0.0):
            value = 0.0
        self.pdfError_ = error
        ot.Log.Debug("Current pdf epsilon : %s" %self.pdfError_)
        return value

    def computePDFOn1DGrid(self, b, N):
        """
        The interest is to compute the density function on a 1D grid of form:
          \forall m\in\{0,\hdots,N-1\},\:y_{m}=\mu +b\left(\frac{2m+1}{N} - 1\right)\sigma

        The density is given by:
          p_{m}= Q_{m}+S_{m}
        with S_{m} = \frac{h}{2\pi}\sum_{k=-N}^{N} \delta(kh)\times E_{m}(k)
        Here :
          E_{m}(k)=e^{-i kh (\mu + b (\frac{2m+1}{N}-1)\sigma)}
        Using FFT,
        S_{m}=\frac{h}{2\pi} \Sigma_{m}^{+} + \Sigma_{m}^{-}
        with
        \Sigma_{m}^{+}=\sum_{k=0}^{N-1}\delta((k+1)h) E_{m}(k+1)
        \Sigma_{m}^{-}=\sum_{k=0}^{N-1} \delta(-(k+1)h)E_{m}(-(k+1))

        Parameters
        ----------
        b : positive float
            The number of marginal standard deviations beyond which the density is evaluated

        N : positive integer, preference of form N = 2**k
            The number of points used for meshing the interval [mean - b * sigma, mean + b * sigma]

        Returns
        -------
        grid_values : array
                      1D Grid on which the probability density function has been evaluated

        pdf_values : array
                    The probability density function values on the grid

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> b = 4 # we are interested in the pdf on mean +/- b * sigma
        >>> N = 256 # 256 points for the grid
        >>> [grid_values, pdf_values] = dist.computePDFOn1DGrid(b, N)

        """
        # Initializing some variables
        pi = np.pi
        mu = self.getMean()[0]
        sigma = self.getStandardDeviation()[0]
        b_sigma = b * sigma
        tau = mu / b_sigma
        h = pi / b_sigma
        # Vectorizing some functions
        normal_pdf = np.vectorize(self.equivalentNormal_.computePDF)
        normal_cf = np.vectorize(self.equivalentNormal_.computeCharacteristicFunction)

        # compute the gaussian pdf on y_m + 2(k+1)b \sigma, k in 0,..,N-1, m in 0,..,N-1
        # ym_grid is also usefull for visualization
        ym_grid = mu + ( (2.0 * np.arange(N) + 1.0) / N - 1.0) * b_sigma
        # to check here
        ot.Log.Info("Precomputing gaussian pdf")
        pdf = normal_pdf(ym_grid)
        ot.Log.Info("End of gaussian approximation")
        # interest is to build 2 * (k+1)*b *sigma and compute the gaussian pdf on the grid of form
        # grid of k, k =1,...,N
        k_grid = 2.0 * np.arange(1, N + 1) * b_sigma
        # TODO check the interest here or optimize
        # This part is too much CPU consuming
        # y_m + 2k*b*sigma
        #tmp = np.array([el + k_grid for el in ym_grid])
        #pdf += np.sum(normal_pdf(tmp), axis=1)
        # y_m - 2k*b*sigma
        #tmp = np.array([el - k_grid for el in ym_grid])
        #pdf += np.sum(normal_pdf(tmp), axis=1)

        # Precompute the grid of delta functions
        # the concerning grid is of form h,2h,...,Nh
        ot.Log.Info("Precomputing delta grid")
        delta_grid = np.arange(1, N + 1) * h
        dcf = np.array([self.computeCharacteristicFunction([k]) for k in delta_grid]) - normal_cf(delta_grid)
        ot.Log.Info("End of precomputing delta grid")

        # compute \Sigma_+
        yk = dcf * np.exp( -2 * pi* 1j * (tau + 1.0 /N - 1.0) * np.arange(1, N+1))
        yk_hat = np.fft.fft(yk)
        sigma_plus = yk_hat * np.exp(2 * pi* 1j *np.arange(N) / N)

        # compute the \Sigma_-
        zk = np.conjugate(dcf[N - np.arange(N) - 1]) * np.exp(pi* 1j * (tau + 1.0 /N - 1.0) * (N - np.arange(1,N+1)))
        zk_hat = np.fft.fft(zk)
        sigma_minus = zk_hat * np.exp(2 * pi* 1j * np.arange(N))

        # final computation
        s_m = h / (2.0 * pi) * (sigma_plus + sigma_minus)
        total_pdf = pdf + s_m.real
        total_pdf *= (total_pdf > 0)
        return [ym_grid, total_pdf]

    def getAlpha(self):
        """
        Returns the alpha parameter used for the evaluation of the range.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> alpha = dist.getAlpha()

        """
        return self.alpha_

    def getBeta(self):
        """
        Returns the beta parameter used for the evaluation of the range.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> beta = dist.getBeta()

        """
        return self.beta_

    def getBlockMax(self):
        """
        Returns the blockMax parameter used for the evaluation
        of the pdf using the gaussian approximation.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> blockMax = dist.getBlockMax()

        """
        return self.blockMax_

    def getBlockMin(self):
        """
        Returns the blockMin parameter used for the evaluation
        of the pdf using the gaussian approximation.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> blockMin = dist.getBlockMin()

        """
        return self.blockMin_

    def getConstant(self):
        """
        Returns the constant vector. This vector is also denoted by y_0
        in the reference guide documentation.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> constant = dist.getConstant() # ==> the output should be the previous vector constant

        """
        return self.constant_

    def getCorrelation(self):
        """
        Returns the correlation matrix of the mixture.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> corr = dist.getCorrelation()

        """
        return self.corr_

    def getCovariance(self):
        """
        Returns the covariance matrix of the mixture.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> cov = dist.getCovariance()

        """
        return self.cov_

    def getDistributionCollection(self):
        """
        Returns the distribution collection.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> distCollection = dist.getDistributionCollection()

        """
        return ot.DistributionCollection(self.collection_)

    def getLastPDFError(self):
        """
        Returns the last error obtained during computation of the probability density function.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> pdf = dist.computePDF([0.0, 1.0])
        >>> last_error = dist.getLastPDFError()

        """
        return self.pdfError_

    def getMatrix(self):
        """
        Returns the matrix of the affine transform.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> mat = dist.getMatrix()

        """
        return self.matrix_

    def getMean(self):
        """
        Returns the mean vector of the mixture.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> mean = dist.getMean()

        """
        return self.mean_

    def getPDFPrecision(self):
        """
        Returns the pdf precision used during computation of the probability density function.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> pdf_precision = dist.getPDFPrecision()

        """
        return self.pdfPrecision_

    def getRange(self):
        """
        Returns the range of the distribution.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> interval = dist.getRange()

        """
        return self.interval_

    def getRealization(self):
        """
        Get a realization of the distribution.

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

    def getReferenceBandwidth(self):
        """
        Returns the bandwidth parameter used for the evaluation
        of the pdf using the gaussian approximation.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> referenceBandwidth = dist.getReferenceBandwidth()

        """
        return self.referenceBandwidth_

    def getSample(self, n):
        """
        Get a sample of size n of the distribution.

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
        sample = ot.ComposedDistribution(ot.DistributionCollection(self.collection_)).getSample(n)
        # product matrix * realization
        # using np for scalability (matrix * sample not available)
        sample = np.array(sample) * np.matrix(self.matrixT_)
        # np.matrix could not be casted into ot.NumericalSample
        sample = np.array(sample)
        sample = ot.NumericalSample(sample)
        # Do not forget the constant term
        # optimization : constant size is usually negligible compared to the sample size
        if self.constant_.norm() != 0:
            sample.translate(self.constant_)
        return sample

    def getStandardDeviation(self):
        """
        Returns the standard deviation.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> sigma = dist.getStandardDeviation()

        """
        return self.sigma_

    def setAlpha(self, alpha):
        """
        Set the alpha parameter used for the evaluation of the range.
        The parameter corresponds to the number of standard deviations
        covered by the marginal distributions.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> dist.setAlpha(5.0)

        """
        self.alpha_ = float(alpha)

    def setBeta(self, beta):
        """
        Set the beta parameter used for the evaluation of the range.
        The parameter corresponds to the number of marginal deviations
        beyond which the density is negligible

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> dist.setBeta(6.0)

        """
        self.beta_ = float(beta)

    def setBlockMax(self, blockMax):
        """
        Set the blockMax parameter used for the evaluation
        of the pdf using the gaussian approximation.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> dist.setBlockMax(8)

        """
        self.blockMax_ = int(blockMax)

    def setBlockMin(self, blockMin):
        """
        Set the blockMin parameter used for the evaluation
        of the pdf using the gaussian approximation

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> dist.setBlockMin(2)

        """
        self.blockMin_ = int(blockMin)

    def setGridMesher(self, gridMesher):
        """
        Modify the function used when iterating over grid.  This method is called by __init__,
        but it can be called to specify an alternate function.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> import MaxNormMeshGrid
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> dist.setGridMesher(MaxNormMeshGrid.Cube2D(dist.getReferenceBandwidth(), True))

        """
        self.meshGrid_ = gridMesher
        two_pi_on_h = [2.0 * cmath.pi / element for element in self.referenceBandwidth_]
        self.meshAltGrid_ = self.meshGrid_.clone(two_pi_on_h)

    def setPDFPrecision(self, pdfPrecision):
        """
        Sets the pdf precision used during computation of the probability density function.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> pdfPrecision = 1e-6
        >>> dist.setPDFPrecision(pdfPrecision)

        """
        assert float(pdfPrecision) > 0
        self.pdfPrecision_ = float(pdfPrecision)

    def setReferenceBandwidth(self, bandwidth):
        """
        Set the blockMin parameter used for the evaluation
        of the pdf using the gaussian approximation.

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> constant = [5, 6]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> dist.setReferenceBandwidth([0.1, 0.1])

        """
        if len(bandwidth) != self.dimension_:
            raise ValueError("The given bandwidth's size differ with the dimension of distribution")
        self.referenceBandwidth_ = [float(element) for element in bandwidth]


class MultivariateRandomMixture(ot.Distribution):
    """
    MultivariateRandomMixture allows to build an OpenTURNS distribution
    which aims to model a multivariate random vector with the following structure:
    Y = y_0 + M X
    where: Y of size d, d\in {1,2,3}
           X is a n-random vector with independent components, i.e. a collection of univariate distributions,
           M is a (d x n) deterministic matrix, i.e. the linear operator of the affine transformation,
           y_0 a constant and deterministic vector,  i.e the constant part of the affine transformation.
    The distribution is a generalization of the unidimensional RandomMixture distribution.

    """
    def __new__(self, collection, matrix, constant):
        instance = PythonMultivariateRandomMixture(collection, matrix, constant)
        return ot.Distribution(instance)
