//                                               -*- C++ -*-
/**
 *  @file  Logistic.hxx
 *  @brief The Logistic distribution
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
 *  Id      Logistic.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_LOGISTIC_HXX
#define OPENTURNS_LOGISTIC_HXX

#include "OTprivate.hxx"
#include "NonEllipticalDistribution.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Logistic
 *
 * The Logistic distribution.
 */
class Logistic
  : public NonEllipticalDistribution
{
  CLASSNAME;
public:

  typedef Pointer<DistributionImplementation> Implementation;

  /** Default constructor */
  Logistic();

  /** Parameters constructor */
  Logistic(const NumericalScalar alpha,
           const NumericalScalar beta);


  /** Comparison operator */
  Bool operator ==(const Logistic & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;



  /* Interface inherited from Distribution */

  /** Virtual constructor */
  Logistic * clone() const;

  /** Get one realization of the Logistic distribution */
  NumericalPoint getRealization() const;

  /** Get the DDF of the Logistic distribution */
  using NonEllipticalDistribution::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the Logistic distribution */
  using NonEllipticalDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;
  using NonEllipticalDistribution::computeLogPDF;
  NumericalScalar computeLogPDF(const NumericalPoint & point) const;

  /** Get the CDF of the Logistic distribution */
  using NonEllipticalDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;
  using NonEllipticalDistribution::computeComplementaryCDF;
  NumericalScalar computeComplementaryCDF(const NumericalPoint & point) const;

  /** Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x) const;
  NumericalComplex computeLogCharacteristicFunction(const NumericalScalar x) const;

  /** Get the PDFGradient of the Logistic distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the Logistic distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the roughness, i.e. the L2-norm of the PDF */
  NumericalScalar getRoughness() const;

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

  /* Interface specific to Logistic */

  /** Alpha accessor */
  void setAlpha(const NumericalScalar alpha);
  NumericalScalar getAlpha() const;

  /** Beta accessor */
  void setBeta(const NumericalScalar beta);
  NumericalScalar getBeta() const;

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

  /** Get the quantile of the Logistic distribution */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** The main parameter set of the distribution */
  NumericalScalar alpha_;
  NumericalScalar beta_;


}; /* class Logistic */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LOGISTIC_HXX */
