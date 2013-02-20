//                                               -*- C++ -*-
/**
 *  @file  Rayleigh.hxx
 *  @brief The Rayleigh distribution
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
 *  @author lebrun
 *  @date   2008-09-09 23:26:29 +0200 (mar, 09 sep 2008)
 *  Id      Rayleigh.hxx 924 2008-09-09 21:26:29Z lebrun
 */
#ifndef OPENTURNS_RAYLEIGH_HXX
#define OPENTURNS_RAYLEIGH_HXX

#include "NonEllipticalDistribution.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Rayleigh
 *
 * The Rayleigh distribution.
 */
class Rayleigh
  : public NonEllipticalDistribution
{
  CLASSNAME;
public:

  typedef Pointer<DistributionImplementation> Implementation;

  /** Default constructor */
  Rayleigh();

  /** Parameters constructor */
  Rayleigh(const NumericalScalar sigma,
           const NumericalScalar gamma = 0.0);


  /** Comparison operator */
  Bool operator ==(const Rayleigh & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;



  /* Interface inherited from Distribution */

  /** Virtual constructor */
  virtual Rayleigh * clone() const;

  /** Get one realization of the Rayleigh distribution */
  NumericalPoint getRealization() const;

  /** Get the DDF of the distribution, i.e. the gradient of its PDF w.r.t. point */
  using NonEllipticalDistribution::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the distribution, i.e. P(point < X < point+dx) = PDF(point)dx + o(dx) */
  using NonEllipticalDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;
  using NonEllipticalDistribution::computeLogPDF;
  NumericalScalar computeLogPDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution, i.e. P(X <= point) = CDF(point). If tail=true, compute P(X >= point) */
  using NonEllipticalDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x) const;

  /** Get the gradient of the PDF w.r.t the parameters of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the gradient of the CDF w.r.t the parameters of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Get the raw moments of the distribution */
  NumericalPoint getStandardMoment(const UnsignedLong n) const;

  /** Get the standard representative in the parametric family, associated with the standard moments */
  Implementation getStandardRepresentative() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using NonEllipticalDistribution::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /* Interface specific to Rayleigh */

  /** Sigma accessor */
  void setSigma(const NumericalScalar sigma);
  NumericalScalar getSigma() const;

  /** Gamma accessor */
  void setGamma(const NumericalScalar gamma);
  NumericalScalar getGamma() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

private:

  /** Compute the mean of the distribution */
  void computeMean() const;

  /** Compute the covariance of the distribution */
  void computeCovariance() const;

  /** Get the quantile of the distribution, i.e the value Xp such that P(X <= Xp) = prob */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** The dispersion parameter */
  NumericalScalar sigma_;

  /** The position parameter */
  NumericalScalar gamma_;


}; /* class Rayleigh */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_RAYLEIGH_HXX */
