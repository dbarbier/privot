//                                               -*- C++ -*-
/**
 *  @file  Experiment.cxx
 *  @brief Abstract top-level view of an experiment plane
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include "Experiment.hxx"
#include "Axial.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Experiment);

/* Default constructor */
Experiment::Experiment(const String & name)
  : TypedInterfaceObject<ExperimentImplementation>(new Axial(name))
{
  // Nothing to do
}

/* Constructor from implementation pointer */
Experiment::Experiment(const Implementation & p_implementation,
                       const String & name)
  : TypedInterfaceObject<ExperimentImplementation>(p_implementation)
{
  // Nothing to do
}

/* Constructor from implementation */
Experiment::Experiment(const ExperimentImplementation & implementation,
                       const String & name)
  : TypedInterfaceObject<ExperimentImplementation>(implementation.clone())
{
  // Nothing to do
}

/* String converter */
String Experiment::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName ()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}

void Experiment::setImplementation(const Implementation & p_implementation)
{
  copyOnWrite();
  getImplementation() = p_implementation;
}

/* Sample generation */
NumericalSample Experiment::generate()
{
  return getImplementation()->generate();
}

END_NAMESPACE_OPENTURNS
