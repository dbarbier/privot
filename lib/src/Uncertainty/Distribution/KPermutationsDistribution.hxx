//                                               -*- C++ -*-
/**
 *  @file  KPermutationsDistribution.hxx
 *  @brief The KPermutationsDistribution distribution
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
 *  Id      KPermutationsDistribution.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_KPERMUTATIONSDISTRIBUTION_HXX
#define OPENTURNS_KPERMUTATIONSDISTRIBUTION_HXX

#include "OTprivate.hxx"
#include "DiscreteDistribution.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class KPermutationsDistribution
 *
 * The KPermutationsDistribution distribution.
 */
class KPermutationsDistribution
  : public DiscreteDistribution
{
  CLASSNAME;
public:

  /** Default constructor */
  KPermutationsDistribution();


  /** Parameters constructor */
  KPermutationsDistribution(const UnsignedLong k,
                            const UnsignedLong n);


  /** Comparison operator */
  Bool operator ==(const KPermutationsDistribution & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;



  /* Interface inherited from Distribution */

  /** Virtual constructor */
  virtual KPermutationsDistribution * clone() const;

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get the PDF of the distribution */
  using DiscreteDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;
  using DiscreteDistribution::computeLogPDF;
  NumericalScalar computeLogPDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution */
  using DiscreteDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the i-th marginal distribution */
  using DiscreteDistribution::getMarginal;
  Implementation getMarginal(const UnsignedLong i) const;

  /** Get the distribution of the marginal distribution corresponding to indices dimensions */
  Implementation getMarginal(const Indices & indices) const;

  /** Get the support of a discrete distribution that intersect a given interval */
  using DistributionImplementation::getSupport;
  NumericalSample getSupport(const Interval & interval) const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;

  /* Interface specific to KPermutationsDistribution */

  /** K accessor */
  void setK(const UnsignedLong k);
  UnsignedLong getK() const;

  /** N accessor */
  void setN(const UnsignedLong n);
  UnsignedLong getN() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:


private:

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** Quantile computation for dimension=1 */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** Compute the mean of the distribution */
  void computeMean() const;

  /** Compute the covariance of the distribution */
  void computeCovariance() const;

  /** Size of the permutations */
  UnsignedLong k_;

  /** Size of the base set */
  UnsignedLong n_;

  /** Log PDF value */
  NumericalScalar logPDFValue_;

}; /* class KPermutationsDistribution */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_KPERMUTATIONSDISTRIBUTION_HXX */
