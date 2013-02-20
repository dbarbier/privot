//                                               -*- C++ -*-
/**
 *  @file  ARMAFactory.cxx
 *  @brief The class that implements all random vectors
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */

#include "PersistentObjectFactory.hxx"
#include "ARMAFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(ARMAFactory);

/* Default constructor */
ARMAFactory::ARMAFactory()
  : TypedInterfaceObject<ARMAFactoryImplementation>(new ARMAFactoryImplementation())
{
  // Nothing to do
}

/* Parameters constructor */
ARMAFactory::ARMAFactory(const ARMAFactoryImplementation & implementation,
                         const String & name)
  : TypedInterfaceObject<ARMAFactoryImplementation>(implementation.clone())
{
  getImplementation()->setName(name);
}

/* Constructor from implementation */
ARMAFactory::ARMAFactory(const Implementation & p_implementation,
                         const String & name)
  : TypedInterfaceObject<ARMAFactoryImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* Constructor from implementation pointer */
ARMAFactory::ARMAFactory(ARMAFactoryImplementation * p_implementation,
                         const String & name)
  : TypedInterfaceObject<ARMAFactoryImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* Virtual constructor */
ARMAFactory * ARMAFactory::clone() const
{
  return new ARMAFactory(*this);
}

/* String converter */
String ARMAFactory::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}

/* String converter */
String ARMAFactory::__str__(const String & offset) const
{
  return __repr__();
}

/* Build method */
ARMA ARMAFactory::build(const TimeSeries & timeSeries) const
{
  return *(getImplementation()->build(timeSeries));
}

/* Build method */
ARMA ARMAFactory::build(const ProcessSample & sample) const
{
  return *(getImplementation()->build(sample));
}



END_NAMESPACE_OPENTURNS
