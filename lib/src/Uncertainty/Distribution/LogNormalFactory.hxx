//                                               -*- C++ -*-
/**
 *  @file  LogNormalFactory.hxx
 *  @brief Factory for LogNormal distribution
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
 *  Id      LogNormalFactory.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_LOGNORMALFACTORY_HXX
#define OPENTURNS_LOGNORMALFACTORY_HXX

#include "OTprivate.hxx"
#include "DistributionImplementationFactory.hxx"
#include "LogNormal.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class LogNormalFactory
 */
class LogNormalFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  LogNormalFactory();

  /** Virtual constructor */
  virtual LogNormalFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;

  Implementation build(const NumericalSample & sample) const
  {
    return buildAsLogNormal(sample).clone();
  }

  LogNormal buildAsLogNormal(const NumericalSample & sample) const
  {
    return buildAsLogNormal(sample, ResourceMap::GetAsUnsignedLong("LogNormalFactory-EstimationMethod"));
  };
  Implementation build(const NumericalSample & sample,
                       const UnsignedLong method) const;
  Implementation build(const NumericalPointCollection & parameters) const;
  Implementation build() const;
  LogNormal buildAsLogNormal(const NumericalSample & sample,
                             const UnsignedLong method) const;
  LogNormal buildAsLogNormal(const NumericalPointCollection & parameters) const;
  LogNormal buildAsLogNormal(const NumericalPointWithDescriptionCollection & parameters) const;
  LogNormal buildAsLogNormal() const;

  /* Algorithm associated with the method of moments */
  LogNormal buildMethodOfMoments(const NumericalSample & sample) const;

  /* Algoritm associated with the method of local likelihood maximization */
  LogNormal buildMethodOfLocalLikelihoodMaximization(const NumericalSample & sample) const;

  /* Algorithm associated with the method of modified moments */
  LogNormal buildMethodOfModifiedMoments(const NumericalSample & sample) const;

private:

}; /* class LogNormalFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LOGNORMALFACTORY_HXX */
