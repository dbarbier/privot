//                                               -*- C++ -*-
/**
 *  @file  DistributionImplementationFactory.hxx
 *  @brief Top-level class for all distribution factories
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
 *  Id      DistributionImplementationFactory.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_DISTRIBUTIONIMPLEMENTATIONFACTORY_HXX
#define OPENTURNS_DISTRIBUTIONIMPLEMENTATIONFACTORY_HXX

#include "DistributionImplementation.hxx"
#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"
#include "CovarianceMatrix.hxx"
#include "PersistentObject.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class DistributionImplementationFactory
 */
class DistributionImplementationFactory
  : public PersistentObject
{
  CLASSNAME;
public:

  typedef Collection<NumericalPoint>                 NumericalPointCollection;
  typedef Collection<NumericalPointWithDescription>  NumericalPointWithDescriptionCollection;
  typedef DistributionImplementation::Implementation Implementation;

  /** Default constructor */
  DistributionImplementationFactory(const UnsignedLong bootstrapSize = ResourceMap::GetAsUnsignedLong("DistributionImplementationFactory-DefaultBootstrapSize"),
                                    const String & name = DefaultName);

  /** Virtual constructor */
  virtual DistributionImplementationFactory * clone() const;

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

  /* Here is the interface that all derived class must implement */
  /** Build a distribution based on a sample */
  virtual Implementation build(const NumericalSample & sample) const;

  /** Build a distribution based on a sample and gives the covariance matrix of the estimate */
  virtual Implementation build(const NumericalSample & sample,
                               CovarianceMatrix & covariance) const;

  /** Build a distribution based on a set of parameters */
  virtual Implementation build(const NumericalPointCollection & parameters) const;
  /** Build a distribution based on a set of parameters */
  virtual Implementation build(const NumericalPointWithDescriptionCollection & parameters) const;

  /** Build a distribution using its default constructor */
  virtual Implementation build() const;

  /** Bootstrap size accessor */
  UnsignedLong getBootstrapSize() const;
  void setBootstrapSize(const UnsignedLong bootstrapSize);


protected:
  /* Execute a R script */
  virtual NumericalPoint runRFactory(const NumericalSample & sample,
                                     const DistributionImplementation & distribution) const;

  /* Number of bootstrap resampling for ovariance estimation */
  UnsignedLong bootstrapSize_;

  /* Convert a NumericalPointWithDescriptionCollection into a NumericalPointCollection */
  static NumericalPointCollection RemoveDescriptionFromCollection(const NumericalPointWithDescriptionCollection & coll);

  /* Convert a NumericalPointCollection into a NumericalPointWithDescriptionCollection */
  static NumericalPointWithDescriptionCollection AddDescriptionToCollection(const NumericalPointCollection & coll);

  /* Convert a parameters collection into a NumericalPoint */
  static NumericalPoint ParametersAsNumericalPoint(const NumericalPointWithDescriptionCollection & parametersCollection);

}; /* class DistributionImplementationFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DISTRIBUTIONIMPLEMENTATIONFACTORY_HXX */
