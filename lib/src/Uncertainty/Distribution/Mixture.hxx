//                                               -*- C++ -*-
/**
 *  @file  Mixture.hxx
 *  @brief The class that implements mixtures
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
 *  Id      Mixture.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_MIXTURE_HXX
#define OPENTURNS_MIXTURE_HXX

#include "Distribution.hxx"
#include "DistributionImplementation.hxx"
#include "Collection.hxx"
#include "UserDefined.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Mixture
 *
 * The class describes the probabilistic concept of Mixture.
 */
class Mixture
  : public DistributionImplementation
{
  CLASSNAME;
public:

  /** A type for distribution collection */
  typedef Collection<Distribution>           DistributionCollection;
  typedef PersistentCollection<Distribution> DistributionPersistentCollection;

  /** Default constructor */
  explicit Mixture();

  /** Parameters constructor */
  explicit Mixture(const DistributionCollection & coll);
  explicit Mixture(const DistributionCollection & coll,
                   const NumericalPoint & weights);

  /** Comparison operator */
  Bool operator ==(const Mixture & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;


  /** Distribution collection accessor */
  void setDistributionCollection(const DistributionCollection & coll);
  const DistributionCollection & getDistributionCollection() const;

  /** get/set weights accessor */
  NumericalPoint getWeights() const;
  void setWeights(const NumericalPoint & weights);

  /* Here is the interface that all derived class must implement */

  /** Virtual constructor */
  virtual Mixture * clone() const;

  /** Get one realization of the Mixture */
  NumericalPoint getRealization() const;

  /** Get the DDF of the Mixture */
  using DistributionImplementation::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the Mixture */
  using DistributionImplementation::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the Mixture */
  using DistributionImplementation::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x) const;

  /** Get the PDF gradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDF gradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the i-th marginal distribution */
  Implementation getMarginal(const UnsignedLong i) const;

  /** Get the distribution of the marginal distribution corresponding to indices dimensions */
  Implementation getMarginal(const Indices & indices) const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;

  /** Check if the distribution is elliptical */
  Bool isElliptical() const;

  /** Check if the distribution is continuous */
  Bool isContinuous() const;

  /** Check if the distribution is integral */
  Bool isIntegral() const;

  /** Tell if the distribution has elliptical copula */
  Bool hasEllipticalCopula() const;

  /** Tell if the distribution has independent copula */
  Bool hasIndependentCopula() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:


private:
  /** Set distribution collection with weights given in a vector */
  void setDistributionCollectionWithWeights(const DistributionCollection & coll,
                                            const NumericalPoint & weights);

  /** Compute the mean of a mixture */
  void computeMean() const;

  /** Compute the covariance of a mixture */
  void computeCovariance() const;

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** Weights distribution accessor */
  void setWeightsDistribution(const UserDefined & weighstDistribution);
  UserDefined getWeightsDistribution() const;

  /** The collection of distribution of the mixture */
  DistributionPersistentCollection distributionCollection_;

  /** The discrete distribution of the weights */
  UserDefined weightsDistribution_;

}; /* class Mixture */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_MIXTURE_HXX */
