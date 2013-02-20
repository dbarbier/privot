//                                               -*- C++ -*-
/**
 *  @file  TruncatedNormal.hxx
 *  @brief The TruncatedNormal distribution
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 *  Id      TruncatedNormal.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_TRUNCATEDNORMAL_HXX
#define OPENTURNS_TRUNCATEDNORMAL_HXX

#include "OTprivate.hxx"
#include "NonEllipticalDistribution.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class TruncatedNormal
 *
 * The TruncatedNormal distribution.
 */
class TruncatedNormal
  : public NonEllipticalDistribution
{
  CLASSNAME;
public:

  typedef Pointer<DistributionImplementation> Implementation;

  /** Default constructor */
  TruncatedNormal();

  /** Parameters constructor */
  TruncatedNormal(const NumericalScalar mu,
                  const NumericalScalar sigma,
                  const NumericalScalar a,
                  const NumericalScalar b);



  /** Comparison operator */
  Bool operator ==(const TruncatedNormal & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;



  /* Interface inherited from Distribution */

  /** Virtual constructor */
  virtual TruncatedNormal * clone() const;

  /** Get one realization of the TruncatedNormal distribution */
  NumericalPoint getRealization() const;

  /** Get the DDF of the TruncatedNormal distribution */
  using NonEllipticalDistribution::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the TruncatedNormal distribution */
  using NonEllipticalDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the TruncatedNormal distribution */
  using NonEllipticalDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;
  using NonEllipticalDistribution::computeComplementaryCDF;
  NumericalScalar computeComplementaryCDF(const NumericalPoint & point) const;

  /** Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x) const;
  NumericalComplex computeLogCharacteristicFunction(const NumericalScalar x) const;

  /** Get the PDFGradient of the TruncatedNormal distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the TruncatedNormal distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Get the standard representative in the parametric family, associated with the standard moments */
  Implementation getStandardRepresentative() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using NonEllipticalDistribution::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /* Interface specific to TruncatedNormal */

  /** Mu accessor */
  void setMu(const NumericalScalar mu);
  NumericalScalar getMu() const;

  /** Sigma accessor */
  void setSigma(const NumericalScalar sigma);
  NumericalScalar getSigma() const;

  /** A accessor */
  void setA(const NumericalScalar a);
  NumericalScalar getA() const;

  /** B accessor */
  void setB(const NumericalScalar b);
  NumericalScalar getB() const;

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

  /** Get the quantile of the TruncatedNormal distribution */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** The main parameter set of the distribution */
  NumericalScalar mu_;
  NumericalScalar sigma_;
  NumericalScalar a_;
  NumericalScalar b_;
  /** Usefull quantities */
  NumericalScalar aNorm_;
  NumericalScalar bNorm_;
  NumericalScalar phiANorm_;
  NumericalScalar phiBNorm_;
  NumericalScalar PhiANorm_;
  NumericalScalar PhiBNorm_;
  NumericalScalar normalizationFactor_;

}; /* class TruncatedNormal */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_TRUNCATEDNORMAL_HXX */
