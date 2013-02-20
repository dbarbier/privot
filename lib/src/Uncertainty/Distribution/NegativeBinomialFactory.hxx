//                                               -*- C++ -*-
/**
 *  @file  NegativeBinomialFactory.hxx
 *  @brief Factory for NegativeBinomial distribution
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
 *  Id      NegativeBinomialFactory.hxx 1473 2010-02-04 15:44:49Z dutka
 */
#ifndef OPENTURNS_NEGATIVEBINOMIALFACTORY_HXX
#define OPENTURNS_NEGATIVEBINOMIALFACTORY_HXX

#include "OTprivate.hxx"
#include "DistributionImplementationFactory.hxx"
#include "NegativeBinomial.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class NegativeBinomialFactory
 */
class NegativeBinomialFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  NegativeBinomialFactory();

  /** Virtual constructor */
  virtual NegativeBinomialFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;

  Implementation build(const NumericalSample & sample) const;
  Implementation build(const NumericalPointCollection & parameters) const;
  Implementation build() const;
  NegativeBinomial buildAsNegativeBinomial(const NumericalSample & sample) const;
  NegativeBinomial buildAsNegativeBinomial(const NumericalPointCollection & parameters) const;
  NegativeBinomial buildAsNegativeBinomial(const NumericalPointWithDescriptionCollection & parameters) const;
  NegativeBinomial buildAsNegativeBinomial() const;

}; /* class NegativeBinomialFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NEGATIVEBINOMIALFACTORY_HXX */
