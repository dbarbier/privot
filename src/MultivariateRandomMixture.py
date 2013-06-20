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
import sys

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
        self.meshGrid_ = None
        self.computeReferenceBandwidth()
        # set equivalent Normal distribution, i.e a normal distribution with mean = self.mean_
        # and covariance = self.cov_
        self.computeEquivalentNormal()
        # self.temp_mesh_grid_ is a grid which is used by computePDF and its FFT variants.
        # It must not be cached.  It is initialized by self.referenceBandwidth_, but only its
        # dimension is used, to check that dimension is right when cloning this grid.
        if len(self.referenceBandwidth_) == 1:
            self.temp_mesh_grid_ = MaxNormMeshGrid.Cube1D(self.referenceBandwidth_, symmetric=True)
        elif len(self.referenceBandwidth_) == 2:
            self.temp_mesh_grid_ = MaxNormMeshGrid.SkinCube2D(self.referenceBandwidth_, symmetric=True)
        elif len(self.referenceBandwidth_) == 3:
            self.temp_mesh_grid_ = MaxNormMeshGrid.SkinCube3D(self.referenceBandwidth_, symmetric=True)
        cacheSize = ot.ResourceMap.GetAsUnsignedLong("MultivariateRandomMixture-DefaultCacheSize")
        if cacheSize > 0:
            self.setGridMesher(MaxNormMeshGrid.CachedMeshGrid(self.temp_mesh_grid_, size=cacheSize))
        else:
            self.setGridMesher(self.temp_mesh_grid_.clone(self.referenceBandwidth_))

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

    def computeEquivalentNormalPDFSum(self, y, mesh_grid, imax=sys.maxint):
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
        delta = max(1.0, gaussian_pdf)
        isGridSymmetric = mesh_grid.isSymmetric()
        eps = self.pdfEpsilon_
        while delta > gaussian_pdf * eps and i < imax:
            i = i + 1
            delta = 0.0
            iterator = mesh_grid.get_skin_iterator(i)
            for _ in xrange(mesh_grid.get_size_of_level(i)):
                point = iterator.next()
                x = [y[k] + point[k] for k in xrange(self.dimension_)]
                delta += self.equivalentNormal_.computePDF(x)
                if isGridSymmetric:
                    x = [y[k] - point[k] for k in xrange(self.dimension_)]
                    delta += self.equivalentNormal_.computePDF(x)
            gaussian_pdf += delta
        return gaussian_pdf

    def getEquivalentNormalPDFSumLevelMax(self, mu, mesh_grid):
        """
        Compute the range of the equivalent normal distribution.
        This method is private, it is used to speed up computeEquivalentNormalPDFSum
        when computing PDF on a regular grid.

        """
        gaussian_pdf = self.equivalentNormal_.computePDF(mu)
        i = 0
        delta = max(1.0, gaussian_pdf)
        isGridSymmetric = mesh_grid.isSymmetric()
        eps = self.pdfEpsilon_
        while delta > gaussian_pdf * eps:
            i = i + 1
            delta = 0.0
            iterator = mesh_grid.get_skin_iterator(i)
            for _ in xrange(mesh_grid.get_size_of_level(i)):
                point = iterator.next()
                x = [mu[k] + point[k] for k in xrange(self.dimension_)]
                delta += self.equivalentNormal_.computePDF(x)
                if isGridSymmetric:
                    x = [mu[k] - point[k] for k in xrange(self.dimension_)]
                    delta += self.equivalentNormal_.computePDF(x)
            gaussian_pdf += delta
        return i-1

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
            referenceBandwidth = [2.0 * cmath.pi / (self.getRange().getUpperBound()[k] - self.getRange().getLowerBound()[k]) for k in xrange(self.dimension_)]
            # Shrink a little bit the bandwidth if the range is finite
            isFinite = [self.getRange().getFiniteLowerBound()[k] and self.getRange().getFiniteUpperBound()[k] for k in xrange(self.dimension_)]
            if (all(isFinite)):
                referenceBandwidth = [referenceBandwidth[k] * 0.5 for k in xrange(self.dimension_)]
        # Else use a kind of Normal approximation
        else:
            referenceBandwidth = [2.0 * cmath.pi / ((self.beta_ + 4.0 * self.alpha_) * self.sigma_[l]) for l in xrange(self.dimension_)]
        self.setReferenceBandwidth(referenceBandwidth)

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
        for dist in collection:
            # check if distribution is univariate
            if (dist.getDimension() != 1):
                raise ValueError("Expected a collection of univariate distributions")
            distributions.append(dist)
        self.collection_ = distributions

    def clone(self, steps):
        """
        Clone the current distribution

        """
        return PythonMultivariateRandomMixture(self.collection_, self.matrix_, self.constant_)

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
        for dist, mty in zip(self.collection_, mt_y):
            somme += dist.computeLogCharacteristicFunction(mty)
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
        two_pi_on_h = [2.0 * cmath.pi / element for element in self.referenceBandwidth_]
        alt_grid_mesher = self.temp_mesh_grid_.clone(two_pi_on_h)
        value = self.computeEquivalentNormalPDFSum(y, alt_grid_mesher)
        # Compute the factor \prod_{k=1}^{d} h_k/'2\pi
        # h_k are supposed to be small values, we must care about
        # numerical troubles
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
                for n in xrange(self.meshGrid_.get_size_of_level(m)):
                    h, cfValue = iterator.next()
                    h_y = self.meshGrid_.dot(h, y)
                    cos_hy = cmath.cos(h_y).real
                    sin_hy = cmath.sin(h_y).real
                    error += cfValue.real * cos_hy + cfValue.imag * sin_hy
            error *= self.referenceBandwidthFactor_
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
        >>> matrix = ot.Matrix([[1,2]])
        >>> constant = [0]
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix, constant)
        >>> b = 4 # we are interested in the pdf on mean +/- b * sigma
        >>> N = 256 # 256 points for the grid
        >>> [grid_values, pdf_values] = dist.computePDFOn1DGrid(b, N)
        >>> f = open("out.csv", "w")
        >>> f.write("x;pdf\n")
        >>> for i in xrange(len(grid_values)):
        ...     f.write("{0:.16g};{1:.16g}\n".format(xgrid_values[i], pdf_values[i]))
        ... f.close()

        """
        if self.dimension_ != 1:
            raise ValueError("Method available for dimension 1 only")
        assert float(b) > 0.0
        # Initializing some variables
        pi = cmath.pi
        mu = self.getMean()[0]
        sigma = self.getStandardDeviation()[0]
        b_sigma = b * sigma
        tau = mu / b_sigma
        h = pi / b_sigma

        # compute the gaussian pdf on y_m + 2(k+1)b \sigma, k in 0,..,N-1, m in 0,..,N-1
        # ym_grid is also usefull for visualization
        ym_grid = mu + ( (2.0 * np.arange(N) + 1.0) / N - 1.0) * b_sigma
        if self.isAnalyticPDF_:
            pdf = np.array([self.computePDF([dyk]) for dyk in ym_grid])
            return [ym_grid, pdf]

        ot.Log.Info("Precomputing gaussian pdf")
        alt_grid_mesher = self.temp_mesh_grid_.clone([2.0 * b_sigma])
        imax = self.getEquivalentNormalPDFSumLevelMax([mu], alt_grid_mesher);
        pdf = [self.computeEquivalentNormalPDFSum([el], alt_grid_mesher, imax) for el in ym_grid]
        ot.Log.Info("End of gaussian approximation")

        # Precompute the grid of delta functions
        # the concerning grid is of form h,2h,...,Nh
        ot.Log.Info("Precomputing delta grid")
        delta_grid = np.arange(1, N + 1) * h
        dcf = np.array([self.computeDeltaCharacteristicFunction([k]) for k in delta_grid])
        ot.Log.Info("End of precomputing delta grid")

        # compute \Sigma_+ term
        # \Sigma_{m}^{+}=\sum_{k=0}^{N-1}\delta((k+1)h) E_{m}(k+1)
        # \Sigma_{m}^{+}_{m} = fft(yk) * zm with :
        # yk = \delta[(k+1) * h] * exp(- pi* 1j * (k+1) * (tau - 1.0 + 1.0 / N)), k=0,1,...,N-1
        # zm = exp(-2.0 * pi* 1j * m / N), m =0,1,...,N-1
        yk = dcf * np.exp(- pi* 1j * (tau - 1.0 + 1.0 / N) * np.arange(1, N+1))
        yk_hat = np.fft.fft(yk)
        z_exp = np.exp(-2.0 * pi* 1j * np.arange(N) / N)
        sigma_plus = yk_hat * z_exp

        # compute the \Sigma_- term
        # \Sigma_{m}^{-}=\sum_{k=0}^{N-1}\delta(-(k+1)h) E_{m}(-(k+1))
        # \Sigma_{m}^{-}_{m} = fft(zk)
        # zk = conj(\delta[(N-k)*h]) * exp(- pi* 1j * (k-N) * (tau - 1.0 + 1.0 / N))
        zk = np.conjugate(yk[::-1])
        sigma_minus = np.fft.fft(zk)

        # final computation
        s_m = h / (2.0 * pi) * (sigma_plus + sigma_minus)
        pdf += s_m.real
        pdf *= (pdf > 0)
        return [ym_grid, pdf]

    def computePDFOn2DGrid(self, b, N):
        """
        The interest is to compute the density function on a 2D grid of form:
          r \in {1,2},\forall m\in\{0,\hdots,N-1\},\:y_{r,m}=\mu_r+b(\frac{2m+1}{N} - 1)\sigma_r

        The density is given by:
          p_{m1,m2}= Q_{m1,m2}+S_{m1,m2}
        with S_{m1,m2} = \frac{hx hy}{4\pi^2}\sum_{k1=-N}^{N}\sum_{k2=-N}^{N}\delta(k1 h1,k2hy) E_{m1,m2}(k1,k2)
        Here :
          E_{m1,m2}(k1,k2)=e^{-i\sum_{j=1}^2 k_jh_j (\mu_j+a (\frac{2m_j+1}{M}-1)\sigma_j)}
        Using FFT,
        S_{m1,m2}=\frac{hxhy}{4\pi^2} { \Sigma_{m1,m2}^{++} + \Sigma_{m1,m2}^{--} + \Sigma_{m1,m2}^{+-} + \Sigma_{m1,m2}^{-+}+...
          ...+ \Sigma_{m1,m2}^{+0}+\Sigma_{m1,m2}^{-0}+\Sigma_{m1,m2}^{0+}+\Sigma_{m1,m2}^{0-} }
        with
        \Sigma_{m1,m2}^{++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\delta((k1+1)hx,(k2+1)hy)E_{m1,m2}(k1+1,k2+1)
        \Sigma_{m1,m2}^{--}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\delta(-(k1+1) hx,-(k2+1)hy)E_{m1,m2}(-(k1+1),-(k2+1))
        \Sigma_{m1,m2}^{+-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy)E_{m1,m2}(k1+1,-(k2+1))
        \Sigma_{m1,m2}^{-+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\delta(-(k1+1)hx,(k2+1)hy)E_{m1,m2}(-(k1+1),k2+1)
        \Sigma_{m1,m2}^{+0}=\sum_{k=0}^{N-1}\delta((k+1)hx,0)E_{m1,m2}(k+1,0)
        \Sigma_{m1,m2}^{-0}=\sum_{k=0}^{N-1}\delta(-(k+1)hx,0)E_{m1,m2}(-(k+1),0)
        \Sigma_{m1,m2}^{0+}=\sum_{k=0}^{N-1}\delta(0,(k+1)hy)E_{m1,m2}(0,k+1)
        \Sigma_{m1,m2}^{0-}=\sum_{k=0}^{N-1}\delta(0,-(k+1)hy)E_{m1,m2}(0,-(k+1))

        Parameters
        ----------
        b : positive float
            The number of marginal standard deviations beyond which the density is evaluated

        N : positive integer, preference of form N = 2^k
            The number of points used for meshing the interval [mean - b * sigma, mean + b * sigma]

        Returns
        -------
        grid_values : ndarray of shape (N, 2)
                      2D Grid on which the probability density function has been evaluated

        pdf_values : ndarray of shape (N,N)
                    The probability density function values on the grid

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2], [3,4]])
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix)
        >>> b = 4 # we are interested in the pdf on mean +/- b * sigma
        >>> N = 64 # 64x64 points for the grid
        >>> [grid_values, pdf_values] = dist.computePDFOn2DGrid(b, N)
        >>> xgrid_values, ygrid_values = tuple(grid_values)
        >>> f = open("out.csv", "w")
        >>> f.write("x;y;pdf\n")
        >>> for i in xrange(len(xgrid_values)):
        ...     for j in xrange(len(ygrid_values)):
        ...         f.write("{0:.16g};{1:.16g};{2:.16g}\n".format(xgrid_values[i], ygrid_values[j], pdf_values[i][j]))
        ... f.close()

        """

        if self.dimension_ != 2:
            raise ValueError("Method available for dimension 2 only")
        # Initializing some variables
        assert (float(b) > 0)
        pi = cmath.pi
        two_pi = 2.0 * pi
        mu_x, mu_y = tuple(self.getMean())
        sigma_x, sigma_y = tuple(self.getStandardDeviation())
        b_sigma_x, b_sigma_y = sigma_x * b, sigma_y * b
        tau_x, tau_y = mu_x / b_sigma_x, mu_y / b_sigma_y
        h_x, h_y = pi / b_sigma_x, pi / b_sigma_y
        # compute the gaussian pdf on y_m + 2(k+1)b \sigma, k in 0,..,N-1, m in 0,..,N-1
        x_grid = mu_x + ( (2.0 * np.arange(N) + 1.0) / N - 1.0) * b_sigma_x
        y_grid = mu_y + ( (2.0 * np.arange(N) + 1.0) / N - 1.0) * b_sigma_y

        if self.isAnalyticPDF_:
            pdf = np.array([[self.computePDF([x_grid[i], y_grid[j]]) for j in xrange(N)] for i in xrange(N)])
            return [[x_grid, y_grid], pdf]

        # gaussian sum pdf computation
        ot.Log.Info("Precomputing gaussian pdf")
        alt_grid_mesher = self.temp_mesh_grid_.clone([2.0 * b_sigma_x, 2.0 * b_sigma_y])
        imax = self.getEquivalentNormalPDFSumLevelMax([mu_x, mu_y], alt_grid_mesher);
        pdf = np.array([[self.computeEquivalentNormalPDFSum([xm, ym], alt_grid_mesher, imax) for ym in y_grid] for xm in x_grid])
        ot.Log.Info("End of gaussian approximation")

        # Precompute the grid of delta functions
        ot.Log.Info("Precomputing delta grid")

        #  These arrays are used below
        f1 = np.exp(- pi* 1j * (tau_x - 1.0 + 1.0 / N) * np.arange(1, N+1))
        f2 = np.exp(- pi* 1j * (tau_y - 1.0 + 1.0 / N) * np.arange(1, N+1))
        z_exp_m2 = np.exp(-two_pi* 1j * np.arange(N) / N)
        z_exp_m1 = z_exp_m2.reshape(N,1)

        # compute \Sigma_++
        # \Sigma_{m1, m2}^{++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\delta((k1+1)hx, (k2+1)hy) E_{m1,m2}(k1+1, k2+1)
        # \Sigma_{m1, m2}^{++} = fft(y_{k1, k2}) * z_{m1,m2} with :
        # y_{k1, k2} = \delta[(k1+1) * hx, (k2+1) * hy] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1,m2} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m2 / N),forall k1,k2,m1,m2=0,1,...,N-1
        dcf = np.array( [[self.computeDeltaCharacteristicFunction([(i + 1) * h_x, (j + 1) * h_y]) for j in xrange(N)] for i in xrange(N)] )
        f1f2 = f1.reshape(N,1) * f2
        yplusplus = dcf * f1f2
        yk_hat = np.fft.fft2(yplusplus)
        zm1m2 = z_exp_m1 * z_exp_m2
        sigma_plus_plus = yk_hat * zm1m2

        # compute the \Sigma_--
        # \Sigma_{m1, m2}^{--}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\delta(-(k1+1)hx, -(k2+1)hy) E_{m1,m2}(-(k1+1),-(k2+1))
        # \Sigma_{m1, m2}^{--} = fft(y_{k1, k2}) * z_{m1,m2} with :
        # y_{k1, k2} = conj(\delta[-(N-k1)*hx, -(N-k2)*hy])* exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1,m2} = exp(2.0 * pi* 1j * m1) * exp(2.0 * pi* 1j * m2),forall k1,k2,m1,m2=0,1,...,N-1
        yminusminus = np.conjugate(yplusplus[::-1,::-1])
        sigma_minus_minus = np.fft.fft2(yminusminus)

        # compute the \Sigma_+-
        # \Sigma_{m1, m2}^{+-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\delta((k1+1)hx, -(k2+1)hy) E_{m1,m2}((k1+1), -(k2+1))
        # \Sigma_{m1, m2}^{+-} = fft(y_{k1, k2}) * z_{m1,m2} with :
        # y_{k1, k2} = \delta[(k1+1)*hx, (N-k2)*hy]* exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1,m2} = exp(-2.0 * pi* 1j * m1/N) * exp(2.0 * pi* 1j * m2),forall k1,k2,m1,m2=0,1,...,N-1
        dcf = np.array( [[self.computeDeltaCharacteristicFunction([(i + 1) * h_x, (j - N) * h_y]) for j in xrange(N)] for i in xrange(N)] )
        f1f2 = f1.reshape(N,1) * np.conjugate(f2[::-1])
        yplusminus = dcf * f1f2
        yk_hat = np.fft.fft2(yplusminus)
        sigma_plus_minus = yk_hat * z_exp_m1

        # compute the \Sigma_-+
        # \Sigma_{m1, m2}^{-+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\delta(-(k1+1)hx, (k2+1)hy) E_{m1,m2}(-(k1+1), k2+1)
        # \Sigma_{m1, m2}^{-+} = fft(y_{k1, k2}) * z_{m1,m2} with :
        # y_{k1, k2} = conj(\delta[-(N-k1) * hx, -(k2+1) * hy]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1,m2} = exp(2.0 * pi* 1j * m1) * exp(-2.0 * pi* 1j * m2 / N),forall k1,k2,m1,m2=0,1,...,N-1
        yminusplus = np.conjugate(yplusminus[::-1,::-1])
        yk_hat = np.fft.fft2(yminusplus)
        sigma_minus_plus = yk_hat * z_exp_m2

        # The interest here is to compute "1D" contributions
        # "1D" because one of the components is set to 0
        # The computation is similar to \Sigma_plus or \Sigma_minus in 1D case
        # The 1D array are added line per line or column per column following
        # the case
        # For the "minus" case, results are deduced from the 1D case
        # whereas for "plus" cases, the calculations have been done manually
        z_exp_m1 = np.exp(-two_pi* 1j * np.arange(N) / N)

        # \Sigma_+_0
        # \Sigma_{m1, m2}^{+0}=\sum_{k1=0}^{N-1}\delta((k1+1)hx, 0) E_{m1,m2}(k1+1, 0)
        # \Sigma_{m1, m2}^{+0} = fft(y_{k1}) * z_{m1} with :
        # y_{k1} = \delta[(k1+1) * hx, 0] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N))
        # zm_{m1} = exp(-2.0 * pi* 1j * m1 / N), forall k1,m1,m2=0,1,...,N-1
        dcf = np.array([self.computeDeltaCharacteristicFunction([(i + 1) * h_x, 0]) for i in xrange(N)])
        yk = dcf * f1
        yk_hat = np.fft.fft(yk)
        sigma_plus_0 = yk_hat * z_exp_m1

        # \Sigma_-_0
        # \Sigma_{m1, m2}^{-0}=\sum_{k1=0}^{N-1}\delta(-(k1+1)hx, 0) E_{m1,m2}(-(k1+1), 0)
        # \Sigma_{m1, m2}^{-0} = fft(y_{k1}) * z_{m1} with :
        # y_{k1} = conj(\delta[(N-k1) * hx, 0]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N))
        # zm_{m1} = exp(2.0 * pi* 1j * m1), forall k1,m1,m2=0,1,...,N-1
        yk = np.conjugate(yk[::-1])
        sigma_minus_0 = np.fft.fft(yk)

        # \Sigma_0_+
        # \Sigma_{m1, m2}^{0+}=\sum_{k2=0}^{N-1}\delta(0, (k2+1)hy) E_{m1,m2}(0, k2+1)
        # \Sigma_{m1, m2}^{0+} = fft(y_{k2}) * z_{m2} with :
        # y_{k1} = \delta[0, (k2+1) * hy] * exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1} = exp(-2.0 * pi* 1j * m2 / N), forall k2,m1,m2=0,1,...,N-1
        dcf = np.array([self.computeDeltaCharacteristicFunction([0, (i + 1) * h_y]) for i in xrange(N)])
        yk = dcf * f2
        yk_hat = np.fft.fft(yk)
        sigma_0_plus = yk_hat * z_exp_m1

        # \Sigma_0_-
        # \Sigma_{m1, m2}^{0-}=\sum_{k2=0}^{N-1}\delta(0, -(k2+1)hy) E_{m1,m2}(0, -(k2+1))
        # \Sigma_{m1, m2}^{0-} = fft(y_{k2}) * z_{m2} with :
        # y_{k1} = conj(\delta[0, (N-k2) * hy]) * exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1} = exp(2.0 * pi* 1j * m2), forall k2,m1,m2=0,1,...,N-1
        yk = np.conjugate(yk[::-1])
        sigma_0_minus = np.fft.fft(yk)

        # Summation of the contributions
        s_m = sigma_plus_plus + sigma_minus_minus + sigma_plus_minus + sigma_minus_plus + \
              sigma_plus_0.reshape(N,1) + sigma_minus_0.reshape(N,1) + sigma_0_plus + sigma_0_minus
        s_m *= (h_x * h_y) / (4.0 * pi * pi)

        # final computation
        pdf += s_m.real
        pdf *= (pdf > 0)
        ot.Log.Info("End of precomputing delta grid")
        return [[x_grid, y_grid], pdf]

    def computePDFOn3DGrid(self, b, N):

        """
        The interest is to compute the density function on a 3D grid of form:
          r \in {1,2,3},\forall m\in\{0,\hdots,N-1\},\:y_{r,m}=\mu_r+b(\frac{2m+1}{N} - 1)\sigma_r

        The density is given by:
          p_{m1,,m2, m3}= Q_{m1,m2,m3}+S_{m1,m2,m3}
        with S_{m1,m2,m3} = \frac{hx hy hz}{8\pi^3}\sum_{k1=-N}^{N}\sum_{k2=-N}^{N}\sum_{k3=-N}^{N}\delta(k1 h_x,k2 h_y k3 h_z) E_{m1,m2,m3}(k1,k2,k3)
        Here :
          E_{m1,m2,m3}(k1,k2,k3)=e^{-i\sum_{j=1}^{3} k_jh_j (\mu_j+a (\frac{2m_j+1}{M}-1)\sigma_j)}
        Using FFT,
        S_{m1,m2,m3}=\frac{hx hy hz}{4\pi^2} \sum_{s1,s2,s3 \in [0,-,+]} \Sigma_{m1,m2,m3}^{s1 s2 s3}

        with
        \Sigma_{m1,m2,m3}^{+++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(k1+1,k2+1,k3+1)
        \Sigma_{m1,m2,m3}^{---}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(-(k1+1),-(k2+1),-(k3+1))
        \Sigma_{m1,m2,m3}^{++-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,k2+1,-(k3+1))
        \Sigma_{m1,m2,m3}^{--+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,-(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(-(k1+1),-(k2+1),k3+1)
        \Sigma_{m1,m2,m3}^{+-+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),k3+1)
        \Sigma_{m1,m2,m3}^{-+-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(-(k1+1),(k2+1),-(k3+1))
        \Sigma_{m1,m2,m3}^{+--}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),-(k3+1))
        \Sigma_{m1,m2,m3}^{-++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),-(k3+1))
        \Sigma_{m1,m2,m3}^{++0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta((k1+1)hx,(k2+1)hy,0) E_{m1,m2,m3}(k1+1,k2+1,0)
        \Sigma_{m1,m2,m3}^{--0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta(-(k1+1)hx,-(k2+1)hy,0) E_{m1,m2,m3}(-(k1+1),-(k2+1),0)
        \Sigma_{m1,m2,m3}^{0++}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0,(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(0,k2+1,k3+1)
        \Sigma_{m1,m2,m3}^{0--}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0,k2+1,k3+1)
        \Sigma_{m1,m2,m3}^{+0+}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta((k1+1)hx,0,(k3+1)hz) E_{m1,m2,m3}(k1+1,0,k3+1)
        \Sigma_{m1,m2,m3}^{-0-}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(-(k1+1)hx,0,-(k3+1)hz) E_{m1,m2,m3}(k1+1,0,k3+1)
        \Sigma_{m1,m2,m3}^{+-0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta((k1+1)hx,-(k2+1)hy, 0) E_{m1,m2,m3}(k1+1,-(k2+1),0)
        \Sigma_{m1,m2,m3}^{-+0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta(-(k1+1)hx,(k2+1)hy, 0) E_{m1,m2,m3}(-(k1+1),(k2+1),0)
        \Sigma_{m1,m2,m3}^{+0-}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta((k1+1)hx,0,-(k3+1)hz) E_{m1,m2,m3}(k1+1,0,-(k3+1)
        \Sigma_{m1,m2,m3}^{-0+}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(-(k1+1)hx,0,(k3+1)hz) E_{m1,m2,m3}(-(k1+1),0,(k3+1)
        \Sigma_{m1,m2,m3}^{0+-}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0, (k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0, k2+1,-(k2+1))
        \Sigma_{m1,m2,m3}^{0-+}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0, (k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0, k2+1,-(k3+1))
        \Sigma_{m1,m2,m3}^{+00}=\sum_{k1=0}^{N-1} \delta((k1+1)hx,0,0) E_{m1,m2,m3}(k1+1,0,0)
        \Sigma_{m1,m2,m3}^{-00}=\sum_{k1=0}^{N-1} \delta(-(k1+1)hx,0,0) E_{m1,m2,m3}(-(k1+1),0,0)
        \Sigma_{m1,m2,m3}^{0+0}=\sum_{k2=0}^{N-1} \delta(0,(k2+1)hy,0) E_{m1,m2,m3}(0,k2+1,0)
        \Sigma_{m1,m2,m3}^{0-0}=\sum_{k2=0}^{N-1} \delta(0,-(k2+1)hy,0) E_{m1,m2,m3}(0,-(k2+1),0)
        \Sigma_{m1,m2,m3}^{00+}=\sum_{k3=0}^{N-1} \delta(0,0,(k3+1)hz) E_{m1,m2,m3}(0,0,k3+1)
        \Sigma_{m1,m2,m3}^{00-}=\sum_{k3=0}^{N-1} \delta(0,0,-(k3+1)hz) E_{m1,m2,m3}(0,0,-(k3+1)

        Parameters
        ----------
        b : positive float
            The number of marginal standard deviations beyond which the density is evaluated

        N : positive integer, preference of form N = 2^k
            The number of points used for meshing the interval [mean - b * sigma, mean + b * sigma]

        Returns
        -------
        grid_values : ndarray of shape (N, 3)
                      3D Grid on which the probability density function has been evaluated

        pdf_values : ndarray of shape (N,N,N)
                    The probability density function values on the grid

        Example
        -------
        >>> import openturns as ot
        >>> import MultivariateRandomMixture as MV
        >>> collection = ot.DistributionCollection([ot.Normal(0.0, 1.0), ot.Uniform(2.0, 5.0), ot.Uniform(2.0, 5.0)])
        >>> matrix = ot.Matrix([[1,2, 4], [3,4,5], [6,0,1]])
        >>> dist = MV.PythonMultivariateRandomMixture(collection, matrix)
        >>> b = 4 # we are interested in the pdf on mean +/- b * sigma
        >>> N = 128 # 128x128x128 points for the 3D grid
        >>> [grid_values, pdf_values] = dist.computePDFOn3DGrid(b, N)
        >>> xgrid_values, ygrid_values, zgrid_values = tuple(grid_values)
        >>> f = open("out.csv", "w")
        >>> f.write("x;y;z;pdf\n")
        >>> for i in xrange(len(xgrid_values)):
        ...     for j in xrange(len(ygrid_values)):
        ...         for k in xrange(len(zgrid_values)):
        ...             f.write("{0:.16g};{1:.16g};{2:.16g};{3:.16g}\n".format(xgrid_values[i], ygrid_values[j], zgrid_values[k], pdf_values[i][j][k]))
        ... f.close()

        """

        if self.dimension_ != 3:
            raise ValueError("Method available for dimension 3 only")
            # Initializing some variables
        assert (float(b) > 0)
        pi = cmath.pi
        two_pi = 2.0 * pi
        mu_x, mu_y, mu_z = tuple(self.getMean())
        sigma_x, sigma_y, sigma_z = tuple(self.getStandardDeviation())
        b_sigma_x, b_sigma_y , b_sigma_z = sigma_x * b, sigma_y * b, sigma_z * b
        tau_x, tau_y, tau_z = mu_x / b_sigma_x, mu_y / b_sigma_y, mu_z / b_sigma_z
        h_x, h_y, h_z = pi / b_sigma_x, pi / b_sigma_y, pi / b_sigma_z
        # compute the gaussian pdf on y_m + 2(k+1)b \sigma, k in 0,..,N-1, m in 0,..,N-1
        x_grid = mu_x + ( (2.0 * np.arange(N) + 1.0) / N - 1.0) * b_sigma_x
        y_grid = mu_y + ( (2.0 * np.arange(N) + 1.0) / N - 1.0) * b_sigma_y
        z_grid = mu_z + ( (2.0 * np.arange(N) + 1.0) / N - 1.0) * b_sigma_z
        if self.isAnalyticPDF_:
            pdf = np.array([[[self.computePDF([x_grid[i], y_grid[j], z_grid[k]]) for k in xrange(N)] for j in xrange(N)] for i in xrange(N)] )
            return [[x_grid, y_grid, z_grid], pdf]

        # gaussian sum pdf computation
        ot.Log.Info("Precomputing gaussian pdf")
        alt_grid_mesher = self.temp_mesh_grid_.clone([2.0 * b_sigma_x, 2.0 * b_sigma_y, 2.0 * b_sigma_z])
        imax = self.getEquivalentNormalPDFSumLevelMax([mu_x, mu_y, mu_z], alt_grid_mesher);
        pdf = np.array([[[self.computeEquivalentNormalPDFSum([xm, ym, zm], alt_grid_mesher, imax) for zm in z_grid] for ym in y_grid] for xm in x_grid])
        ot.Log.Info("End of gaussian approximation")

        #  These arrays are used below
        f1 = np.exp(- pi* 1j * (tau_x - 1.0 + 1.0 / N) * np.arange(1, N+1)).reshape(N,1,1)
        f2 = np.exp(- pi* 1j * (tau_y - 1.0 + 1.0 / N) * np.arange(1, N+1)).reshape(1,N,1)
        f3 = np.exp(- pi* 1j * (tau_z - 1.0 + 1.0 / N) * np.arange(1, N+1)).reshape(1,1,N)
        z_exp_m3 = np.exp(-two_pi* 1j * np.arange(N) / N)
        z_exp_m1 = z_exp_m3.reshape(N,1,1)
        z_exp_m2 = z_exp_m3.reshape(1,N,1)

        # 1) compute \Sigma_+++
        # \Sigma_{m1,m2,m3}^{+++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(k1+1,k2+1,k3+1)
        # \Sigma_{m1,m2,m3}^{+++} = fft(y_{k1, k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2+1) hy, (k3+1) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m2 / N) * exp(-2.0 * pi* 1j * m3 / N)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        dcf = np.array([[[self.computeDeltaCharacteristicFunction([(i+1)*h_x, (j+1)*h_y, (k+1)*h_z]) for k in xrange(N)] for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f1 * f2 * f3
        yk_hat = np.fft.fftn(yk)
        sigma_plus_plus_plus = yk_hat * z_exp_m1 * z_exp_m2 * z_exp_m3

        # 2) compute \Sigma_---
        # \Sigma_{m1,m2,m3}^{---}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(-(k1+1),-(k2+1),-(k3+1))
        # \Sigma_{m1,m2,m3}^{---} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = conj(\delta[-(k1-N) hx, -(k2-N) hy, -(k3-N) hz]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(2.0 * pi* 1j * m2) * exp(2.0 * pi* 1j * m3)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        sigma_minus_minus_minus = np.fft.fftn(np.conjugate(yk[::-1,::-1,::-1]))

        # 3) compute \Sigma_++-
        # \Sigma_{m1,m2,m3}^{++-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,k2+1,-(k3+1))
        # \Sigma_{m1,m2,m3}^{++-} = fft(y_{k1, k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2+1) hy, (k3-N) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m2 / N) * exp(2.0 * pi* 1j * m3)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        dcf = np.array([[[self.computeDeltaCharacteristicFunction([(i+1)*h_x, (j+1)*h_y, (k-N)*h_z]) for k in xrange(N)] for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f1 * f2 * np.conjugate(f3[:,:,::-1])
        yk_hat = np.fft.fftn(yk)
        sigma_plus_plus_minus = yk_hat * z_exp_m1 * z_exp_m2

        # 4) compute \Sigma_--+
        # \Sigma_{m1,m2,m3}^{--+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,-(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(-(k1+1),-(k2+1),k3+1)
        # \Sigma_{m1,m2,m3}^{--+} = fft(y_{k1, k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = conj(\delta[-(k1-N) hx, -(k2-N) hy, -(k3+1) hz]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(2.0 * pi* 1j * m2) * exp(-2.0 * pi* 1j * m3/N)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        yk_hat = np.fft.fftn(np.conjugate(yk[::-1,::-1,::-1]))
        sigma_minus_minus_plus = yk_hat * z_exp_m3

        # 5) compute \Sigma_+-+
        # \Sigma_{m1,m2,m3}^{+-+}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),k3+1)
        # \Sigma_{m1,m2,m3}^{+-+} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2-N) hy, (k3+1) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2) * exp(-2.0 * pi* 1j * m3 / N)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        dcf = np.array([[[self.computeDeltaCharacteristicFunction([(i+1)*h_x, (j-N)*h_y, (k+1)*h_z]) for k in xrange(N)] for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f1 * np.conjugate(f2[:,::-1,:]) * f3
        yk_hat = np.fft.fftn(yk)
        sigma_plus_minus_plus = yk_hat * z_exp_m1 * z_exp_m3

        # 6) compute \Sigma_-+-
        # \Sigma_{m1,m2,m3}^{-+-}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta(-(k1+1)hx,(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(-(k1+1),(k2+1),-(k3+1))
        # \Sigma_{m1,m2,m3}^{-+-} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = conj(\delta[(k1-N) hx, (k2+1) hy, (k3-N) hz]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(-2.0 * pi* 1j * m2/N) * exp(2.0 * pi* 1j * m3)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        yk_hat = np.fft.fftn(np.conjugate(yk[::-1,::-1,::-1]))
        sigma_minus_plus_minus = yk_hat * z_exp_m2

        # 7) compute \Sigma_+--
        # \Sigma_{m1,m2,m3}^{+--}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),-(k3+1))
        # \Sigma_{m1,m2,m3}^{+--} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2-N) hy, (k3-N) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(-pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2) * exp(2.0 * pi* 1j * m3)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        dcf = np.array([[[self.computeDeltaCharacteristicFunction([(i+1)*h_x, (j-N)*h_y, (k-N)*h_z]) for k in xrange(N)] for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f1 * np.conjugate(f2[:,::-1,:]) * np.conjugate(f3[:,:,::-1])
        yk_hat = np.fft.fftn(yk)
        sigma_plus_minus_minus = yk_hat * z_exp_m1

        # 8) compute \Sigma_-++
        # \Sigma_{m1,m2,m3}^{-++}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\delta((k1+1)hx,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(k1+1,-(k2+1),-(k3+1))
        # \Sigma_{m1,m2,m3}^{-++} = fft(y_{k1,k2,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,k3} = \delta[(k1+1) hx, (k2-N) hy, (k3-N) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(-pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2) * exp(2.0 * pi* 1j * m3)
        # forall k1,k2,k3,m1,m2,m3=0,1,...,N-1
        yk_hat = np.fft.fftn(np.conjugate(yk[::-1,::-1,::-1]))
        sigma_minus_plus_plus = yk_hat * z_exp_m2 * z_exp_m3

        #----------------------------------------#
        #----------------- FFT2D ----------------#
        #--- In this section, we apply 2D FFT ---#
        #--- and we propagate in 3 dimensions ---#
        #----------------------------------------#
        f1 = np.exp(- pi* 1j * (tau_x - 1.0 + 1.0 / N) * np.arange(1, N+1))
        f2 = np.exp(- pi* 1j * (tau_y - 1.0 + 1.0 / N) * np.arange(1, N+1))
        f3 = np.exp(- pi* 1j * (tau_z - 1.0 + 1.0 / N) * np.arange(1, N+1))
        z_exp_m2 = np.exp(-two_pi* 1j * np.arange(N) / N)
        z_exp_m1 = z_exp_m2.reshape(N,1)

        # 9) compute \Sigma_++0
        # \Sigma_{m1,m2,m3}^{++0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta((k1+1)hx,(k2+1)hy,0) E_{m1,m2,m3}(k1+1,k2+1,0)
        # \Sigma_{m1,m2,m3}^{++0} = fft(y_{k1, k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k1+1) hx, (k2+1) hy, 0] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m2 / N)
        # forall k1,k2,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        dcf = np.array([[self.computeDeltaCharacteristicFunction([(i+1) * h_x, (j+1) * h_y, 0]) for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f1.reshape(N,1) * f2
        yk_hat = np.fft.fft2(yk)
        sigma_plus_plus_0 = yk_hat * z_exp_m1 * z_exp_m2

        # 10) compute \Sigma_--0
        # \Sigma_{m1,m2,m3}^{--0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta(-(k1+1)hx,-(k2+1)hy,0) E_{m1,m2,m3}(-(k1+1),-(k2+1),0)
        # \Sigma_{m1,m2,m3}^{--0} = fft(y_{k1, k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = conj(\delta[-(k1-N) hx, -(k2-N) hy, 0]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(-2.0 * pi* 1j * m2)
        # forall k1,k2,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        sigma_minus_minus_0 = np.fft.fft2(np.conjugate(yk[::-1,::-1]))

        # 11) compute \Sigma_0++
        # \Sigma_{m1,m2,m3}^{0++}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0,(k2+1)hy,(k3+1)hz) E_{m1,m2,m3}(0,k2+1,k3+1)
        # \Sigma_{m1,m2,m3}^{0++} = fft(y_{0,k2, k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[0,(k2+1) hy, (k3+1) hz] * exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m2 / N) * exp(-2.0 * pi* 1j * m3 / N)
        # forall k2,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        dcf = np.array([[self.computeDeltaCharacteristicFunction([0, (i+1)*h_y, (j+1)*h_z]) for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f2.reshape(N,1) * f3
        yk_hat = np.fft.fft2(yk)
        sigma_0_plus_plus = yk_hat * z_exp_m1 * z_exp_m2

        # 12) compute \Sigma_0--
        # \Sigma_{m1,m2,m3}^{0--}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0,-(k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0,k2+1,k3+1)
        # \Sigma_{m1,m2,m3}^{0-} = fft(y_{0,k2, k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = conj(\delta[0,-(k2-N) hy, -(k3-N) hz]) * exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m2) * exp(-2.0 * pi* 1j * m3)
        # forall k2,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        sigma_0_minus_minus = np.fft.fft2(np.conjugate(yk[::-1,::-1]))

        # 13) compute \Sigma_+0+
        # \Sigma_{m1,m2,m3}^{+0+}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta((k1+1)hx,0,(k3+1)hz) E_{m1,m2,m3}(k1+1,0,k3+1)
        # \Sigma_{m1,m2,m3}^{+0+} = fft(y_{k1,0,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k1+1) hx,0,(k3+1) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m3 / N)
        # forall k1,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        dcf = np.array([[self.computeDeltaCharacteristicFunction([(i + 1) * h_x, 0, (j + 1) * h_z]) for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f1.reshape(N,1) * f3
        yk_hat = np.fft.fft2(yk)
        sigma_plus_0_plus = yk_hat * z_exp_m1 * z_exp_m2

        # 14) compute \Sigma_-0-
        # \Sigma_{m1,m2,m3}^{-0-}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(-(k1+1)hx,0,-(k3+1)hz) E_{m1,m2,m3}(k1+1,0,k3+1)
        # \Sigma_{m1,m2,m3}^{-0-} = fft(y_{k1,0,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[-(k1-N) hx,0,-(k3-N) hz] * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(-2.0 * pi* 1j * m3 / N)
        # forall k1,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        sigma_minus_0_minus = np.fft.fft2(np.conjugate(yk[::-1,::-1]))

        # 15) compute \Sigma_+-0
        # \Sigma_{m1,m2,m3}^{+-0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta((k1+1)hx,-(k2+1)hy, 0) E_{m1,m2,m3}(k1+1,-(k2+1),0)
        # \Sigma_{m1,m2,m3}^{+-0} = fft(y_{k1,k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k1+1) hx,0,(k2-N) hy,0] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2-N) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2)
        # forall k1,k2,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        dcf = np.array([[self.computeDeltaCharacteristicFunction([(i+1)*h_x, (j-N)*h_y, 0]) for j in xrange(N)] for i in xrange(N)])
        yk = dcf * f1.reshape(N,1) * np.conjugate(f2[::-1])
        yk_hat = np.fft.fft2(yk)
        sigma_plus_minus_0 = yk_hat * z_exp_m1

        # 16) compute \Sigma_-+0
        # \Sigma_{m1,m2,m3}^{-+0}=\sum_{k1=0}^{N-1}\sum_{k2=0}^{N-1}\\delta(-(k1+1)hx,(k2+1)hy, 0) E_{m1,m2,m3}(-(k1+1),(k2+1),0)
        # \Sigma_{m1,m2,m3}^{-+0} = fft(y_{k1,k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = conj(\delta[(k1-N) hx,0,(k2+1) hy,0]) * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m2)
        # forall k1,k2,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        sigma_minus_plus_0 = np.fft.fft2(np.conjugate(yk[::-1,::-1])) * z_exp_m2

        # 17) compute \Sigma_+0-
        # \Sigma_{m1,m2,m3}^{+0-}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta((k1+1)hx,0,-(k3+1)hz) E_{m1,m2,m3}(k1+1,0,-(k3+1)
        # \Sigma_{m1,m2,m3}^{+0-} = fft(y_{k1,0,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k1+1) hx,0,(k3-N) hz] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N) * exp(2.0 * pi* 1j * m3)
        # forall k1,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        dcf = np.array([[self.computeDeltaCharacteristicFunction([(i+1)*h_x, 0, (j-N)*h_z]) for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f1.reshape(N,1) * np.conjugate(f3[::-1])
        yk_hat = np.fft.fft2(yk)
        sigma_plus_0_minus = yk_hat * z_exp_m1

        # 18) compute \Sigma_-0+
        # \Sigma_{m1,m2,m3}^{-0+}=\sum_{k1=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(-(k1+1)hx,0,(k3+1)hz) E_{m1,m2,m3}(-(k1+1),0,(k3+1)
        # \Sigma_{m1,m2,m3}^{-0+} = fft(y_{k1,0,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k1+1) hx,0,(k3-N) hz] * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1) * exp(-2.0 * pi* 1j * m3/N)
        # forall k1,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        sigma_minus_0_plus = np.fft.fft2(np.conjugate(yk[::-1,::-1])) * z_exp_m2

        # 19) compute \Sigma_0+-
        # \Sigma_{m1,m2,m3}^{0+-}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0, (k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0, k2+1,-(k2+1))
        # \Sigma_{m1,m2,m3}^{0+-} = fft(y_{k1,k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k2+1) hy,0,(k3-N) hz] * exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m2 / N) * exp(2.0 * pi* 1j * m3)
        # forall k2,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        dcf = np.array([[self.computeDeltaCharacteristicFunction([0, (i+1)*h_y, (j-N)*h_z]) for j in xrange(N)] for i in xrange(N)] )
        yk = dcf * f2.reshape(N,1) * np.conjugate(f3[::-1])
        yk_hat = np.fft.fft2(yk)
        sigma_0_plus_minus = yk_hat * z_exp_m1

        # 20) compute \Sigma_0-+
        # \Sigma_{m1,m2,m3}^{0-+}=\sum_{k2=0}^{N-1}\sum_{k3=0}^{N-1}\\delta(0, (k2+1)hy,-(k3+1)hz) E_{m1,m2,m3}(0, k2+1,-(k3+1))
        # \Sigma_{m1,m2,m3}^{0-+} = fft(y_{k1,k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k2+1) hy,0,(k3-N) hz] * exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N)) *
        # exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m2 / N) * exp(2.0 * pi* 1j * m3)
        # forall k2,k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 2, FFT should be of dimension 2
        sigma_0_minus_plus = np.fft.fft2(np.conjugate(yk[::-1,::-1])) * z_exp_m2

        #----------------------------------------#
        #----------------- FFT1D ----------------#
        #--- In this section, we apply 1D FFT ---#
        #--- and we propagate in 3 dimensions ---#
        #----------------------------------------#
        z_exp_m1 = np.exp(-two_pi* 1j * np.arange(N) / N)

        # 21) compute \Sigma_+00
        # \Sigma_{m1,m2,m3}^{+00}=\sum_{k1=0}^{N-1} \delta((k1+1)hx,0,0) E_{m1,m2,m3}(k1+1,0,0)
        # \Sigma_{m1,m2,m3}^{+00} = fft(y_{k1,0,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k1+1)hx,0,0] * exp(- pi* 1j * (k1+1) * (tau_x - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m1 / N)
        # forall k1,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 1, FFT should be of dimension 1
        dcf = np.array([self.computeDeltaCharacteristicFunction([(i+1)*h_x,0,0]) for i in xrange(N)])
        yk = dcf * f1
        yk_hat = np.fft.fft(yk)
        sigma_plus_0_0 = yk_hat * z_exp_m1

        # 22) compute \Sigma_-00
        # \Sigma_{m1,m2,m3}^{-00}=\sum_{k1=0}^{N-1} \delta(-(k1+1)hx,0,0) E_{m1,m2,m3}(-(k1+1),0,0)
        # \Sigma_{m1,m2,m3}^{-00} = fft(y_{k1,0,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[(k1+1)hx,0,0] * exp(- pi* 1j * (k1-N) * (tau_x - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m1)
        # forall k1,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 1, FFT should be of dimension 1
        sigma_minus_0_0 = np.fft.fft(np.conjugate(yk[::-1]))

        # 23) compute \Sigma_0+0
        # \Sigma_{m1,m2,m3}^{0+0}=\sum_{k2=0}^{N-1} \delta(0, (k2+1)hy,0) E_{m1,m2,m3}(0,k2+1,0)
        # \Sigma_{m1,m2,m3}^{0+0} = fft(y_{0,k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[0,(k2+1)hy,0] * exp(- pi* 1j * (k2+1) * (tau_y - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m2 / N)
        # forall k2,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 1, FFT should be of dimension 1
        dcf = np.array([self.computeDeltaCharacteristicFunction([0,(i+1)*h_y,0]) for i in xrange(N)])
        yk = dcf * f2
        yk_hat = np.fft.fft(yk)
        sigma_0_plus_0 = yk_hat * z_exp_m1

        # 24) compute \Sigma_0-0
        # \Sigma_{m1,m2,m3}^{0-0}=\sum_{k2=0}^{N-1} \delta(0, (k2+1)hy,0) E_{m1,m2,m3}(0,k2+1,0)
        # \Sigma_{m1,m2,m3}^{0-0} = fft(y_{0,k2,0}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = conj(\delta[0,-(k2-N)hy,0]) * exp(- pi* 1j * (k2+1) * (tau_x - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m2)
        # forall k2,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 1, FFT should be of dimension 1
        sigma_0_minus_0 = np.fft.fft(np.conjugate(yk[::-1]))

        # 25) compute \Sigma_00+
        # \Sigma_{m1,m2,m3}^{00+}=\sum_{k3=0}^{N-1} \delta(0, 0,(k3+1)hz) E_{m1,m2,m3}(0,0,k3+1)
        # \Sigma_{m1,m2,m3}^{00+} = fft(y_{0,0,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = \delta[0,0,(k3+1)hz] * exp(- pi* 1j * (k3+1) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(-2.0 * pi* 1j * m3 / N)
        # forall k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 1, FFT should be of dimension 1
        dcf = np.array([self.computeDeltaCharacteristicFunction([0,0,(i+1)*h_z]) for i in xrange(N)])
        yk = dcf * f3
        yk_hat = np.fft.fft(yk)
        sigma_0_0_plus = yk_hat * z_exp_m1

        # 26) compute \Sigma_00-
        # \Sigma_{m1,m2,m3}^{00-}=\sum_{k3=0}^{N-1} \delta(0, 0,(k3+1)hz) E_{m1,m2,m3}(0,0,k3+1)
        # \Sigma_{m1,m2,m3}^{00-} = fft(y_{0,0,k3}) * z_{m1,m2,m3} with :
        # y_{k1,k2,0} = conj(\delta[0,0,-(k3-N)hz]) * exp(- pi* 1j * (k3-N) * (tau_z - 1.0 + 1.0 / N))
        # zm_{m1,m2,m3} = exp(2.0 * pi* 1j * m3)
        # forall k3,m1,m2,m3=0,1,...,N-1
        # Care components here are of dimension 1, FFT should be of dimension 1
        sigma_0_0_minus = np.fft.fft(np.conjugate(yk[::-1]))

        s_m = sigma_plus_plus_plus + sigma_minus_minus_minus + sigma_plus_plus_minus + sigma_minus_minus_plus \
            + sigma_plus_minus_plus + sigma_minus_plus_minus + sigma_plus_minus_minus + sigma_minus_plus_plus \
            + sigma_plus_plus_0.reshape(N,N,1)  + sigma_minus_minus_0.reshape(N,N,1) \
            + sigma_plus_minus_0.reshape(N,N,1) + sigma_minus_plus_0.reshape(N,N,1) \
            + sigma_plus_0_plus.reshape(N,1,N)  + sigma_minus_0_minus.reshape(N,1,N) \
            + sigma_plus_0_minus.reshape(N,1,N) + sigma_minus_0_plus.reshape(N,1,N) \
            + sigma_0_plus_plus.reshape(1,N,N)  + sigma_0_minus_minus.reshape(1,N,N) \
            + sigma_0_plus_minus.reshape(1,N,N) + sigma_0_minus_plus.reshape(1,N,N) \
            + sigma_0_0_plus.reshape(1,1,N)     + sigma_0_0_minus.reshape(1,1,N) \
            + sigma_0_plus_0.reshape(1,N,1)     + sigma_0_minus_0.reshape(1,N,1) \
            + sigma_plus_0_0.reshape(N,1,1)     + sigma_minus_0_0.reshape(N,1,1)

        s_m *= (h_x * h_y * h_z) /(8*pi*pi*pi)
        ot.Log.Info("End of precomputing delta grid")

        # final computation
        # We add the real part of the FFT to the gaussian
        # pdf contributions
        pdf += s_m.real
        pdf *= (pdf > 0)

        return [[x_grid, y_grid, z_grid], pdf]

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
        self.temp_mesh_grid_.setSymmetric(self.meshGrid_.isSymmetric())

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
        # Update the grid mesher
        if self.meshGrid_:
            self.setGridMesher(self.meshGrid_.clone(self.referenceBandwidth_))
        # Update other members
        self.referenceBandwidthFactor_ = 1.0
        for component in self.referenceBandwidth_:
            self.referenceBandwidthFactor_ *= component / (2.0 * cmath.pi)

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
