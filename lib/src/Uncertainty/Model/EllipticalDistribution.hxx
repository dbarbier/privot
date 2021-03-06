//                                               -*- C++ -*-
/**
 *  @file  EllipticalDistribution.hxx
 *  @brief Abstract top-level class for elliptical distributions
 *
 *  Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author schueller
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 *  Id      EllipticalDistribution.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_ELLIPTICALDISTRIBUTIONIMPLEMENTATION_HXX
#define OPENTURNS_ELLIPTICALDISTRIBUTIONIMPLEMENTATION_HXX

#include "ContinuousDistribution.hxx"
#include "CorrelationMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class EllipticalDistribution
 *
 * A subclass for elliptical usual distributions.
 */
class EllipticalDistribution
  : public ContinuousDistribution
{
  CLASSNAME;

public:

  // Numerical precision for computing the quantile

  /** Default constructor */
  explicit EllipticalDistribution(const NumericalPoint & mean,
                                  const NumericalPoint & sigma,
                                  const CorrelationMatrix & R,
                                  const NumericalScalar covarianceNormalizationFactor,
                                  const String & name = DefaultName);

  /** Parameter constructor */
  explicit EllipticalDistribution(const String & name = DefaultName);


  /** Virtual copy constructor */
  virtual EllipticalDistribution * clone() const;

  /** Comparison operator */
  Bool operator ==(const EllipticalDistribution & other) const;

  /** String converter */
  String __repr__() const;

  /** Tell if the distribution is elliptical */
  Bool isElliptical() const;

  /** Tell if the distribution has elliptical copula */
  Bool hasEllipticalCopula() const;

  /** Get the DDF of the distribution */
  using ContinuousDistribution::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the distribution */
  using ContinuousDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the PDF gradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Compute the density generator of the elliptical distribution, i.e.
   *  the function phi such that the density of the distribution can
   *  be written as p(x) = phi(t(x-mu)R^{-1}(x-mu))                      */
  virtual NumericalScalar computeDensityGenerator(const NumericalScalar betaSquare) const;

  /** Compute the derivative of the density generator */
  virtual NumericalScalar computeDensityGeneratorDerivative(const NumericalScalar betaSquare) const;

  /** Compute the second derivative of the density generator */
  virtual NumericalScalar computeDensityGeneratorSecondDerivative(const NumericalScalar betaSquare) const;

  /** Mean point accessor */
  void setMean(const NumericalPoint & mean);

  /** Sigma vector accessor */
  void setSigma(const NumericalPoint & sigma);

  /** Sigma vector accessor */
  NumericalPoint getSigma() const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Correlation matrix accessor */
  void setCorrelation(const CorrelationMatrix & R);

  /** Correlation matrix accessor */
  CorrelationMatrix getCorrelation() const;

protected:
  /** Compute the mean of the distribution */
  void computeMean() const;

  /** Compute the covariance of the distribution */
  void computeCovariance() const;

public:
  /** Normalize the given point u_i = (x_i - mu_i) / sigma_i */
  NumericalPoint normalize(const NumericalPoint & x) const;

  /** Denormalize the given point x_i = mu_i + sigma_i * x_i */
  NumericalPoint denormalize(const NumericalPoint & u) const;

  /** Inverse correlation matrix accessor */
  SquareMatrix getInverseCorrelation() const;

  /** Cholesky factor of the correlation matrix accessor */
  SquareMatrix getCholesky() const;

  /** Inverse of the Cholesky factor of the correlation matrix accessor */
  SquareMatrix getInverseCholesky() const;

  /** Get the isoprobabilist transformation */
  IsoProbabilisticTransformation getIsoProbabilisticTransformation() const;

  /** Get the inverse isoprobabilist transformation */
  InverseIsoProbabilisticTransformation getInverseIsoProbabilisticTransformation() const;

  /** Get the standard distribution, i.e. a distribution of the same kind but with zero mean,
   * unit marginal standard distribution and identity correlation */
  Implementation getStandardDistribution() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using ContinuousDistribution::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  /** The sigma vector of the distribution */
  mutable NumericalPoint sigma_;

  /** The correlation matrix (Rij) of the distribution */
  mutable CorrelationMatrix R_;

  /** The shape matrix of the distribution = Diag(sigma_) * R_ * Diag(sigma_) */
  mutable CovarianceMatrix shape_;

  /** The inverse of the correlation matrix of the distribution */
  SymmetricMatrix inverseR_;

  /** The Cholesky factor of the shape matrix shape_ = cholesky_ * cholesky_.transpose() */
  SquareMatrix cholesky_;

  /** The inverse Cholesky factor of the covariance matrix */
  SquareMatrix inverseCholesky_;

  /** The normalization factor of the distribution */
  NumericalScalar normalizationFactor_;

  /** The scaling factor of the covariance matrix covariance = covarianceScalingFactor_ * shape_*/
  NumericalScalar covarianceScalingFactor_;

private:
  /** Compute the value of the auxiliary attributes */
  void update();

}; /* class EllipticalDistribution */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ELLIPTICALDISTRIBUTIONIMPLEMENTATION_HXX */
