//                                               -*- C++ -*-
/**
 *  @file  Process.cxx
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */

#include "PersistentObjectFactory.hxx"
#include "Process.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Process);

/* Default constructor */
Process::Process()
  : TypedInterfaceObject<ProcessImplementation>(new ProcessImplementation())
{
  // Nothing to do
}

/* Parameters constructor */
Process::Process(const ProcessImplementation & implementation,
                 const String & name)
  : TypedInterfaceObject<ProcessImplementation>(implementation.clone())
{
  getImplementation()->setName(name);
}

/* Constructor from implementation */
Process::Process(const Implementation & p_implementation,
                 const String & name)
  : TypedInterfaceObject<ProcessImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* Constructor from implementation pointer */
Process::Process(ProcessImplementation * p_implementation,
                 const String & name)
  : TypedInterfaceObject<ProcessImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* String converter */
String Process::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}

/* String converter */
String Process::__str__(const String & offset) const
{
  return getImplementation()->__str__(offset);
}

/* Description Accessor */
void Process::setDescription(const Description & description)
{
  copyOnWrite();
  getImplementation()->setDescription(description);
}


/* Description Accessor */
Description Process::getDescription() const
{
  return getImplementation()->getDescription();
}

/* Is the underlying a gaussian process ? */
Bool Process::isNormal() const
{
  return getImplementation()->isNormal();
}

/* Is the underlying a stationary process ? */
Bool Process::isStationary() const
{
  return getImplementation()->isStationary();
}

/* Is the underlying a composite process ? */
Bool Process::isComposite() const
{
  return getImplementation()->isComposite();
}

/* Here is the interface that all derived class may implement */

/* Dimension accessor */
UnsignedLong Process::getDimension() const
{
  return getImplementation()->getDimension();
}

/* Realization accessor */
TimeSeries Process::getRealization() const
{
  return getImplementation()->getRealization();
}

/* TimeGrid accessor - get */
RegularGrid Process::getTimeGrid() const
{
  return getImplementation()->getTimeGrid();
}

/* TimeGrid accessor - set */
void Process::setTimeGrid(const RegularGrid & timeGrid)
{
  copyOnWrite();
  return getImplementation()->setTimeGrid(timeGrid);
}

ProcessSample Process::getSample(const UnsignedLong size) const
{
  return getImplementation()->getSample(size);
}

TimeSeries Process::getFuture(const UnsignedLong stepNumber) const
{
  return getImplementation()->getFuture(stepNumber);
}

ProcessSample Process::getFuture(const UnsignedLong stepNumber,
                                 const UnsignedLong size) const
{
  return getImplementation()->getFuture(stepNumber, size);
}

/* Get the process corresponding to the i-th marginal component */
Process Process::getMarginalProcess(const UnsignedLong i) const
{
  return *(getImplementation()->getMarginalProcess(i));
}

/* Get the marginal random vector corresponding to indices components */
Process Process::getMarginalProcess(const Indices & indices) const
{
  return *(getImplementation()->getMarginalProcess(indices));
}
END_NAMESPACE_OPENTURNS
