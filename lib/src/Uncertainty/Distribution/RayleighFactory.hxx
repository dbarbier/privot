//                                               -*- C++ -*-
/**
 *  @file  RayleighFactory.hxx
 *  @brief Factory for Rayleigh distribution
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
 *  @date   2007-05-10 16:43:31 +0200 (jeu. 10 mai 2007)
 *  Id      RayleighFactory.hxx 434 2007-05-10 14:43:31Z dutka
 */
#ifndef OPENTURNS_RAYLEIGHFACTORY_HXX
#define OPENTURNS_RAYLEIGHFACTORY_HXX

#include "DistributionImplementationFactory.hxx"
#include "Rayleigh.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class RayleighFactory
 */
class RayleighFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  RayleighFactory();

  /** Virtual constructor */
  virtual RayleighFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;

  Implementation build(const NumericalSample & sample) const;
  Implementation build(const NumericalPointCollection & parameters) const;
  Implementation build() const;
  Rayleigh buildAsRayleigh(const NumericalSample & sample) const;
  Rayleigh buildAsRayleigh(const NumericalPointCollection & parameters) const;
  Rayleigh buildAsRayleigh(const NumericalPointWithDescriptionCollection & parameters) const;
  Rayleigh buildAsRayleigh() const;

}; /* class RayleighFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_RAYLEIGHFACTORY_HXX */
