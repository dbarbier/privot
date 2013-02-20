//                                               -*- C++ -*-
/**
 *  @file  TriangularFactory.hxx
 *  @brief Factory for Triangular distribution
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
 *  Id      TriangularFactory.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_TRIANGULARFACTORY_HXX
#define OPENTURNS_TRIANGULARFACTORY_HXX

#include "OTprivate.hxx"
#include "DistributionImplementationFactory.hxx"
#include "Triangular.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class TriangularFactory
 */
class TriangularFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  TriangularFactory();

  /** Virtual constructor */
  virtual TriangularFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;

  Implementation build(const NumericalSample & sample) const;
  Implementation build(const NumericalPointCollection & parameters) const;
  Implementation build() const;
  Triangular buildAsTriangular(const NumericalSample & sample) const;
  Triangular buildAsTriangular(const NumericalPointCollection & parameters) const;
  Triangular buildAsTriangular(const NumericalPointWithDescriptionCollection & parameters) const;
  Triangular buildAsTriangular() const;

}; /* class TriangularFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_TRIANGULARFACTORY_HXX */
