//                                               -*- C++ -*-
/**
 *  @file  LogNormal.hxx
 *  @brief The LogNormal distribution
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 *  Id      LogNormal.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_LOGNORMAL_HXX
#define OPENTURNS_LOGNORMAL_HXX

#include "OTprivate.hxx"
#include "NonEllipticalDistribution.hxx"
#include "ResourceMap.hxx"
#include "PersistentCollection.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class LogNormal
 *
 * The LogNormal distribution.
 */
class LogNormal
  : public NonEllipticalDistribution
{
  CLASSNAME;
public:

  enum ParameterSet { MUSIGMA_LOG, MUSIGMA, MU_SIGMAOVERMU };

  typedef Pointer<DistributionImplementation> Implementation;

  /** Default constructor */
  LogNormal();

  /** Parameters constructor */
  LogNormal(const NumericalScalar arg1,
            const NumericalScalar arg2,
            const NumericalScalar gamma = 0.0,
            const ParameterSet set = MUSIGMA_LOG);


  /** Comparison operator */
  Bool operator ==(const LogNormal & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;



  /* Interface inherited from Distribution */

  /** Virtual constructor */
  virtual LogNormal * clone() const;

  /** Get one realization of the LogNormal distribution */
  NumericalPoint getRealization() const;

  /** Get the DDF of the LogNormal distribution */
  using NonEllipticalDistribution::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the LogNormal distribution */
  using NonEllipticalDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;
  using NonEllipticalDistribution::computeLogPDF;
  NumericalScalar computeLogPDF(const NumericalPoint & point) const;

  /** Get the CDF of the LogNormal distribution */
  using NonEllipticalDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;
  using NonEllipticalDistribution::computeComplementaryCDF;
  NumericalScalar computeComplementaryCDF(const NumericalPoint & point) const;

  /** Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x) const;
  NumericalComplex computeLogCharacteristicFunction(const NumericalScalar x) const;

  /** Get the PDFGradient of the LogNormal distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the LogNormal distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Get the raw moments of the standardized distribution */
  NumericalPoint getStandardMoment(const UnsignedLong n) const;

  /** Get the standard representative in the parametric family, associated with the standard moments */
  Implementation getStandardRepresentative() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using NonEllipticalDistribution::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /* Interface specific to LogNormal */

  /** MuLog accessor */
  void setMuLog(const NumericalScalar muLog);
  NumericalScalar getMuLog() const;

  /** SigmaLog accessor */
  void setSigmaLog(const NumericalScalar sigmaLog);
  NumericalScalar getSigmaLog() const;


  /** Mu accessor */
  void setMuSigma(const NumericalScalar mu,
                  const NumericalScalar sigma);
  NumericalScalar getMu() const;

  /** Sigma accessor */
  NumericalScalar getSigma() const;

  /** Gamma accessor */
  void setGamma(const NumericalScalar gamma);
  NumericalScalar getGamma() const;

  /** SigmaOverMu accessor */
  NumericalScalar getSigmaOverMu() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  /** MuLogSigmaLog accessor */
  void setMuLogSigmaLog(const NumericalScalar muLog,
                        const NumericalScalar sigmaLog);

private:

  /** Compute the mean of the distribution */
  void computeMean() const;

  /** Compute the covariance of the distribution */
  void computeCovariance() const;

  /** Get the quantile of the LogNormal distribution */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** Compute the integrand that is involved in the computation of the characteristic function */
  NumericalComplex characteristicIntegrand(const NumericalScalar eta,
                                           const NumericalScalar sStar) const;

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** The main parameter set of the distribution */
  NumericalScalar muLog_;
  NumericalScalar sigmaLog_;
  NumericalScalar gamma_;
  NumericalScalar normalizationFactor_;
  /** Normalization factor for the characteristic function */
  NumericalScalar H_;
  /** Hermite integration */
  mutable NumericalPoint hermiteNodes_;
  mutable NumericalPoint hermiteWeights_;

}; /* class LogNormal */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LOGNORMAL_HXX */
