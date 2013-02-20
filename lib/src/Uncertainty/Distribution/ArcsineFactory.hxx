//                                               -*- C++ -*-
/**
 *  @file  ArcsineFactory.hxx
 *  @brief Factory for DerivativeArcsine distribution
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
 *  @date   2009-09-30 17:51:57 +0200 (mer. 30 sept. 2009)
 *  Id      ArcsineFactory.hxx 1357 2009-09-30 15:51:57Z schueller
 */
#ifndef OPENTURNS_ARCSINEFACTORY_HXX
#define OPENTURNS_ARCSINEFACTORY_HXX

#include "OTprivate.hxx"
#include "DistributionImplementationFactory.hxx"
#include "Arcsine.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ArcsineFactory
 */
class ArcsineFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  ArcsineFactory();

  /** Virtual constructor */
  ArcsineFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;

  Implementation build(const NumericalSample & sample) const;
  Implementation build(const NumericalPointCollection & parameters) const;
  Implementation build() const;
  Arcsine buildAsArcsine(const NumericalSample & sample) const;
  Arcsine buildAsArcsine(const NumericalPointCollection & parameters) const;
  Arcsine buildAsArcsine(const NumericalPointWithDescriptionCollection & parameters) const;
  Arcsine buildAsArcsine() const;

}; /* class ArcsineFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ARCSINEFACTORY_HXX */
