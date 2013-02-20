//                                               -*- C++ -*-
/**
 *  @file  DirichletFactory.hxx
 *  @brief Factory for Dirichlet distribution
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
 *  @author lebrun
 *  @date   2011-01-12 22:41:37 +0100 (mer. 12 janv. 2011)
 *  Id      DirichletFactory.hxx 1692 2011-01-12 21:41:37Z lebrun
 */
#ifndef OPENTURNS_DIRICHLETFACTORY_HXX
#define OPENTURNS_DIRICHLETFACTORY_HXX

#include "OTprivate.hxx"
#include "DistributionImplementationFactory.hxx"
#include "Dirichlet.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class DirichletFactory
 */
class DirichletFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  DirichletFactory();

  /** Virtual constructor */
  virtual DirichletFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;

  Implementation build(const NumericalSample & sample) const;
  Implementation build(const NumericalPointCollection & parameters) const;
  Implementation build() const;
  Dirichlet buildAsDirichlet(const NumericalSample & sample) const;
  Dirichlet buildAsDirichlet(const NumericalPointCollection & parameters) const;
  Dirichlet buildAsDirichlet(const NumericalPointWithDescriptionCollection & parameters) const;
  Dirichlet buildAsDirichlet() const;

}; /* class DirichletFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DIRICHLETFACTORY_HXX */
