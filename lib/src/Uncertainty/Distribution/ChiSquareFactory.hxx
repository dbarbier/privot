//                                               -*- C++ -*-
/**
 *  @file  ChiSquareFactory.hxx
 *  @brief Factory for ChiSquare distribution
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
 *  Id      ChiSquareFactory.hxx 434 2007-05-10 14:43:31Z dutka
 */
#ifndef OPENTURNS_CHISQUAREFACTORY_HXX
#define OPENTURNS_CHISQUAREFACTORY_HXX

#include "DistributionImplementationFactory.hxx"
#include "ChiSquare.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ChiSquareFactory
 */
class ChiSquareFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  ChiSquareFactory();

  /** Virtual constructor */
  virtual ChiSquareFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;

  Implementation build(const NumericalSample & sample) const;
  Implementation build(const NumericalPointCollection & parameters) const;
  Implementation build() const;
  ChiSquare buildAsChiSquare(const NumericalSample & sample) const;
  ChiSquare buildAsChiSquare(const NumericalPointCollection & parameters) const;
  ChiSquare buildAsChiSquare(const NumericalPointWithDescriptionCollection & parameters) const;
  ChiSquare buildAsChiSquare() const;

}; /* class ChiSquareFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_CHISQUAREFACTORY_HXX */
