//                                               -*- C++ -*-
/**
 * @file  DistributionFactory.hxx
 * @brief Top-level class for all distribution factories
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
 * @author schueller
 * @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */

#ifndef OPENTURNS_DISTRIBUTIONFACTORY_HXX
#define OPENTURNS_DISTRIBUTIONFACTORY_HXX

#include "DistributionImplementationFactory.hxx"
#include "Distribution.hxx"
#include "TypedInterfaceObject.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class DistributionFactory
 */
class DistributionFactory
  : public TypedInterfaceObject<DistributionImplementationFactory>
{
  CLASSNAME;
public:

  typedef Collection<NumericalPoint> NumericalPointCollection;
  typedef Collection<NumericalPointWithDescription> NumericalPointWithDescriptionCollection;

  /** Default constructor */
  DistributionFactory(const UnsignedLong bootstrapSize = ResourceMap::GetAsUnsignedLong("DistributionImplementationFactory-DefaultBootstrapSize"),
                      const String & name = DefaultName);

  /** Parameter constructor */
  DistributionFactory(const DistributionImplementationFactory & factory);

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

  /* Here is the interface that all derived class must implement */

  virtual Distribution build(const NumericalSample & sample) const;
  virtual Distribution build(const NumericalPointCollection & parameters) const;
  virtual Distribution build(const NumericalPointWithDescriptionCollection & parameters) const;
  virtual Distribution build() const;

private:

}; /* class DistributionFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DISTRIBUTIONFACTORY_HXX */
