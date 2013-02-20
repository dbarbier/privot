//                                               -*- C++ -*-
/**
 * @file  DistributionFactory.cxx
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
 * @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */

#include "DistributionFactory.hxx"
#include "Description.hxx"
#include "Path.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(DistributionFactory);

/* Default constructor */
DistributionFactory::DistributionFactory(const UnsignedLong bootstrapSize,
                                         const String & name)
  : TypedInterfaceObject<DistributionImplementationFactory>(new DistributionImplementationFactory(bootstrapSize, name))
{
  // Nothing to do
}

/* Parameter constructor */
DistributionFactory::DistributionFactory(const DistributionImplementationFactory & factory)
  : TypedInterfaceObject<DistributionImplementationFactory>(factory.clone())
{
  // Nothing to do
}

/* String converter */
String DistributionFactory::__repr__() const
{
  OSS oss;
  oss << "class=" << DistributionFactory::GetClassName()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}

/* String converter */
String DistributionFactory::__str__(const String & offset) const
{
  return getImplementation()->__str__();
}

/* Here is the interface that all derived class must implement */
Distribution DistributionFactory::build(const NumericalSample & sample) const
{
  return getImplementation()->build(sample);
}

Distribution DistributionFactory::build(const NumericalPointCollection & parameters) const
{
  return getImplementation()->build(parameters);
}

Distribution DistributionFactory::build(const NumericalPointWithDescriptionCollection & parameters) const
{
  return getImplementation()->build(parameters);
}

Distribution DistributionFactory::build() const
{
  return getImplementation()->build();
}

END_NAMESPACE_OPENTURNS
