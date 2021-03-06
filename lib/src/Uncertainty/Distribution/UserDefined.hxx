//                                               -*- C++ -*-
/**
 *  @file  UserDefined.hxx
 *  @brief The UserDefined distribution
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
 *  Id      UserDefined.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_USERDEFINED_HXX
#define OPENTURNS_USERDEFINED_HXX

#include "OTprivate.hxx"
#include "DiscreteDistribution.hxx"
#include "PersistentCollection.hxx"
#include "PersistentObject.hxx"
#include "UserDefinedPair.hxx"
#include "ResourceMap.hxx"
#include "Indices.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class UserDefined
 *
 * The UserDefined distribution.
 */
class UserDefined
  : public DiscreteDistribution
{
  CLASSNAME;
public:

  typedef Collection<UserDefinedPair>           UserDefinedPairCollection;
  typedef PersistentCollection<UserDefinedPair> UserDefinedPairPersistentCollection;
  typedef Collection<NumericalScalar>           NumericalScalarCollection;
  typedef PersistentCollection<NumericalScalar> NumericalScalarPersistentCollection;

  /** Default constructor */
  UserDefined();

  /** Constructor from PairCollection */
  explicit UserDefined(const UserDefinedPairCollection & collection);

  /** Constructor from a sample */
  explicit UserDefined(const NumericalSample & sample);

  /** Constructor from a sample and associated weights */
  explicit UserDefined(const NumericalSample & sample,
                       const NumericalPoint & weights);


  /** Comparison operator */
  Bool operator ==(const UserDefined & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;



  /* Interface inherited from Distribution */

  /** Virtual constructor */
  virtual UserDefined * clone() const;

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get the PDF of the distribution */
  using DiscreteDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution */
  using DiscreteDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the PDFGradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** Get the support of a discrete distribution that intersect a given interval */
  using DistributionImplementation::getSupport;
  NumericalSample getSupport(const Interval & interval) const;

  /** Tell if the distribution is integer valued */
  Bool isIntegral() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;

  /* Interface specific to UserDefined */

  /** Pair collection accessor */
  void setPairCollection(const UserDefinedPairCollection & collection);
  UserDefinedPairCollection getPairCollection() const;

  /** Get the i-th marginal distribution */
  Implementation getMarginal(const UnsignedLong i) const;

  /** Get the distribution of the marginal distribution corresponding to indices dimensions */
  Implementation getMarginal(const Indices & indices) const;

  /** Merge the identical points of the support */
  void compactSupport(const NumericalScalar epsilon = ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"));

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

  /** Quantile computation for dimension=1 */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** The collection of couple (xi,pi) of the UserDefined distribution */
  UserDefinedPairPersistentCollection collection_;

  /** The cumulative probabilities si = sum(pk, k=0..i) */
  NumericalScalarPersistentCollection cumulativeProbabilities_;

  /** Flag to accelerate computations in case of uniform weights */
  mutable Bool hasUniformWeights_;

}; /* class UserDefined */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_USERDEFINED_HXX */
