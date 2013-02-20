//                                               -*- C++ -*-
/**
 *  @file  LaplaceFactory.hxx
 *  @brief Factory for Laplace distribution
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
 *  Id      LaplaceFactory.hxx 434 2007-05-10 14:43:31Z dutka
 */
#ifndef OPENTURNS_LAPLACEFACTORY_HXX
#define OPENTURNS_LAPLACEFACTORY_HXX

#include "OTprivate.hxx"
#include "DistributionImplementationFactory.hxx"
#include "Laplace.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class LaplaceFactory
 */
class LaplaceFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  LaplaceFactory();

  /** Virtual constructor */
  virtual LaplaceFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;

  Implementation build(const NumericalSample & sample) const;
  Implementation build(const NumericalPointCollection & parameters) const;
  Implementation build() const;
  Laplace buildAsLaplace(const NumericalSample & sample) const;
  Laplace buildAsLaplace(const NumericalPointCollection & parameters) const;
  Laplace buildAsLaplace(const NumericalPointWithDescriptionCollection & parameters) const;
  Laplace buildAsLaplace() const;

}; /* class LaplaceFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LAPLACEFACTORY_HXX */
