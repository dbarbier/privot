//                                               -*- C++ -*-
/**
 *  @file  Poisson.hxx
 *  @brief The Poisson distribution
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
 *  Id      Poisson.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_POISSON_HXX
#define OPENTURNS_POISSON_HXX

#include "OTprivate.hxx"
#include "DiscreteDistribution.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Poisson
 *
 * The Poisson distribution.
 */
class Poisson
  : public DiscreteDistribution
{
  CLASSNAME;
public:

  /** Default constructor */
  Poisson();

  /** Parameters constructor */
  Poisson(const NumericalScalar lambda);


  /** Comparison operator */
  Bool operator ==(const Poisson & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;



  /* Interface inherited from Distribution */

  /** Virtual constructor */
  virtual Poisson * clone() const;

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get the PDF of the distribution */
  using DiscreteDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution */
  using DiscreteDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;
  using DiscreteDistribution::computeComplementaryCDF;
  NumericalScalar computeComplementaryCDF(const NumericalPoint & point) const;

  /** Get the PDFGradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Compute the characteristic function, i.e. phi(u) = E(exp(I*u*X)) */
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x) const;
  NumericalComplex computeLogCharacteristicFunction(const NumericalScalar x) const;

  /** Compute the generating function, i.e. psi(z) = E(z^X) */
  NumericalComplex computeGeneratingFunction(const NumericalComplex & z) const;
  NumericalComplex computeLogGeneratingFunction(const NumericalComplex & z) const;

  /** Get the support of a discrete distribution that intersect a given interval */
  using DistributionImplementation::getSupport;
  NumericalSample getSupport(const Interval & interval) const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using DiscreteDistribution::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /* Interface specific to Poisson */

  /** Lambda accessor */
  void setLambda(const NumericalScalar lambda);
  NumericalScalar getLambda() const;

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

  /** Get the quantile of the distribution */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** The Lambda of the Poisson distribution */
  NumericalScalar lambda_;

}; /* class Poisson */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_POISSON_HXX */
