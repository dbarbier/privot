//                                               -*- C++ -*-
/**
 *  @file  BernoulliFactory.hxx
 *  @brief Factory for Bernoulli distribution
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
 *  @author dutka
 *  @date   2010-02-04 16:44:49 +0100 (jeu. 04 févr. 2010)
 *  Id      BernoulliFactory.hxx 1473 2010-02-04 15:44:49Z dutka
 */
#ifndef OPENTURNS_BERNOULLIFACTORY_HXX
#define OPENTURNS_BERNOULLIFACTORY_HXX

#include "OTprivate.hxx"
#include "DistributionImplementationFactory.hxx"
#include "Bernoulli.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class BernoulliFactory
 */
class BernoulliFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  BernoulliFactory();

  /** Virtual constructor */
  virtual BernoulliFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;
  Implementation build(const NumericalSample & sample) const;
  Implementation build(const NumericalPointCollection & parameters) const;
  Implementation build() const;
  Bernoulli buildAsBernoulli(const NumericalSample & sample) const;
  Bernoulli buildAsBernoulli(const NumericalPointCollection & parameters) const;
  Bernoulli buildAsBernoulli(const NumericalPointWithDescriptionCollection & parameters) const;
  Bernoulli buildAsBernoulli() const;

}; /* class BernoulliFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BERNOULLIFACTORY_HXX */
