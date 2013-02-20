//                                               -*- C++ -*-
/**
 *  @file  LogisticFactory.hxx
 *  @brief Factory for Logistic distribution
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
 *  Id      LogisticFactory.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_LOGISTICFACTORY_HXX
#define OPENTURNS_LOGISTICFACTORY_HXX

#include "OTprivate.hxx"
#include "DistributionImplementationFactory.hxx"
#include "Logistic.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class LogisticFactory
 */
class LogisticFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  LogisticFactory();

  /** Virtual constructor */
  virtual LogisticFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;

  Implementation build(const NumericalSample & sample) const;
  Implementation build(const NumericalPointCollection & parameters) const;
  Implementation build() const;
  Logistic buildAsLogistic(const NumericalSample & sample) const;
  Logistic buildAsLogistic(const NumericalPointCollection & parameters) const;
  Logistic buildAsLogistic(const NumericalPointWithDescriptionCollection & parameters) const;
  Logistic buildAsLogistic() const;

}; /* class LogisticFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LOGISTICFACTORY_HXX */
