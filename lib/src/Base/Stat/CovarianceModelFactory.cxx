//                                               -*- C++ -*-
/**
 *  @file  CovarianceModelFactory.cxx
 *  @brief Top-level class for all spectral model factories
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
 *  @author: $LastChangedBy$
 *  @author: $LastChangedBy$
 *  @date:   $LastChangedDate$
 *  Id:      $Id$
 */
#include "CovarianceModelFactory.hxx"


BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(CovarianceModelFactory);

/* Default constructor */
CovarianceModelFactory::CovarianceModelFactory(const String & name)
  : TypedInterfaceObject<CovarianceModelFactoryImplementation>(new CovarianceModelFactoryImplementation(name))
{
  // Nothing to do
}

/* Parameters constructor */
CovarianceModelFactory::CovarianceModelFactory(const CovarianceModelFactoryImplementation & implementation,
                                               const String & name)
  : TypedInterfaceObject<CovarianceModelFactoryImplementation>(implementation.clone())
{
  getImplementation()->setName(name);
}

/* Constructor from implementation */
CovarianceModelFactory::CovarianceModelFactory(const Implementation & p_implementation,
                                               const String & name)
  : TypedInterfaceObject<CovarianceModelFactoryImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* Constructor from implementation pointer */
CovarianceModelFactory::CovarianceModelFactory(CovarianceModelFactoryImplementation * p_implementation,
                                               const String & name)
  : TypedInterfaceObject<CovarianceModelFactoryImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}


/* Virtual constructor */
CovarianceModelFactory * CovarianceModelFactory::clone() const
{
  return new CovarianceModelFactory(*this);
}

/* String converter */
String CovarianceModelFactory::__repr__() const
{
  OSS oss;
  oss << "class=" << CovarianceModelFactory::GetClassName()
      << " implementation = " << getImplementation()->__repr__();
  return oss;
}

/* String converter */
String CovarianceModelFactory::__str__(const String & offset) const
{
  return getImplementation()->__str__();
}

CovarianceModel CovarianceModelFactory::build(const ProcessSample & sample) const
{
  return getImplementation()->build(sample);
}

CovarianceModel CovarianceModelFactory::build(const TimeSeries & timeSeries) const
{
  return getImplementation()->build(timeSeries);
}

END_NAMESPACE_OPENTURNS
