//                                               -*- C++ -*-
/**
 *  @file  DiracFactory.hxx
 *  @brief Factory for Dirac distribution
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
 *  Id      DiracFactory.hxx 1473 2010-02-04 15:44:49Z dutka
 */
#ifndef OPENTURNS_DIRACFACTORY_HXX
#define OPENTURNS_DIRACFACTORY_HXX

#include "OTprivate.hxx"
#include "DistributionImplementationFactory.hxx"
#include "Dirac.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class DiracFactory
 */
class DiracFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  DiracFactory();

  /** Virtual constructor */
  virtual DiracFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;
  Implementation build(const NumericalSample & sample) const;
  Implementation build(const NumericalPointCollection & parameters) const;
  Implementation build() const;
  Dirac buildAsDirac(const NumericalSample & sample) const;
  Dirac buildAsDirac(const NumericalPointCollection & parameters) const;
  Dirac buildAsDirac(const NumericalPointWithDescriptionCollection & parameters) const;
  Dirac buildAsDirac() const;

}; /* class DiracFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DIRACFACTORY_HXX */
