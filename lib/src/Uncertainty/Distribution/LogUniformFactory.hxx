//                                               -*- C++ -*-
/**
 *  @file  LogUniformFactory.hxx
 *  @brief Factory for LogUniform distribution
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
 *  @date   2011-05-24 19:30:41 +0200 (mar. 24 mai 2011)
 *  Id      LogUniformFactory.hxx 1910 2011-05-24 17:30:41Z schueller
 */
#ifndef OPENTURNS_LOGUNIFORMFACTORY_HXX
#define OPENTURNS_LOGUNIFORMFACTORY_HXX

#include "OTprivate.hxx"
#include "DistributionImplementationFactory.hxx"
#include "LogUniform.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class LogUniformFactory
 */
class LogUniformFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  LogUniformFactory();

  /** Virtual constructor */
  LogUniformFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;

  Implementation build(const NumericalSample & sample) const;
  Implementation build(const NumericalPointCollection & parameters) const;
  Implementation build() const;
  LogUniform buildAsLogUniform(const NumericalSample & sample) const;
  LogUniform buildAsLogUniform(const NumericalPointCollection & parameters) const;
  LogUniform buildAsLogUniform(const NumericalPointWithDescriptionCollection & parameters) const;
  LogUniform buildAsLogUniform() const;

}; /* class LogUniformFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LOGUNIFORMFACTORY_HXX */
