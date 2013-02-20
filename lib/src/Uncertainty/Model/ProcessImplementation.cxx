//                                               -*- C++ -*-
/**
 *  @file  ProcessImplementation.cxx
 *  @brief An interface for all implementation class of process
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
#include "ProcessImplementation.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(ProcessImplementation);

static Factory<ProcessImplementation> RegisteredFactory("ProcessImplementation");


/* Default constructor */
ProcessImplementation::ProcessImplementation(const String & name)
  : PersistentObject(name),
    description_(),
    dimension_(1),
    timeGrid_()
{
  // Nothing to do
}

/* Virtual constructor */
ProcessImplementation * ProcessImplementation::clone() const
{
  return new ProcessImplementation(*this);
}

/* String converter */
String ProcessImplementation::__repr__() const
{
  OSS oss;
  oss << "class= " << ProcessImplementation::GetClassName()
      << " dimension_ = " << dimension_
      << " description = " << description_
      << " timeGrid = " << timeGrid_;
  return oss;
}

/* String converter */
String ProcessImplementation::__str__(const String & offset) const
{
  OSS oss;
  oss << "class= " << ProcessImplementation::GetClassName()
      << " dimension = " << dimension_
      << " description = " << description_.__str__(offset)
      << " timeGrid = " << timeGrid_.__str__(offset);
  return oss;
}

/* Dimension accessor */
UnsignedLong ProcessImplementation::getDimension() const
{
  return dimension_;
}

void ProcessImplementation::setDimension(const UnsignedLong dimension)
{
  dimension_ = dimension;
}

/* Description accessor */
void ProcessImplementation::setDescription(const Description & description)
{
  description_ = description;
}

Description ProcessImplementation::getDescription() const
{
  return description_;
}

/* TimeGrid accessor */
RegularGrid ProcessImplementation::getTimeGrid() const
{
  return timeGrid_;
}

void ProcessImplementation::setTimeGrid(const RegularGrid & timeGrid)
{
  timeGrid_ = timeGrid;
}


/* Here is the interface that all derived class must implement */


/* Is the underlying a gaussian process ? */
Bool ProcessImplementation::isNormal() const
{
  return false;
}

/* Is the underlying a stationary process ? */
Bool ProcessImplementation::isStationary() const
{
  return false;
}

/* Is the underlying a composite process ? */
Bool ProcessImplementation::isComposite() const
{
  return false;
}

/* Realization accessor */
TimeSeries ProcessImplementation::getRealization() const
{
  throw NotYetImplementedException(HERE);
}


ProcessSample ProcessImplementation::getSample(const UnsignedLong size) const
{
  ProcessSample result(timeGrid_, size, dimension_);
  for (UnsignedLong i = 0; i < size; ++i) result[i] = getRealization();
  return result;
}

/* Future accessor */
TimeSeries ProcessImplementation::getFuture(const UnsignedLong stepNumber) const
{
  throw NotYetImplementedException(HERE);
}

ProcessSample ProcessImplementation::getFuture(const UnsignedLong stepNumber,
                                               const UnsignedLong size) const
{
  if (size == 0) return ProcessSample(timeGrid_, 0, dimension_);
  ProcessSample result(size, getFuture(stepNumber));
  for (UnsignedLong i = 1; i < size; ++i) result[i] = getFuture(stepNumber);
  return result;
}

/* Get the random vector corresponding to the i-th marginal component */
ProcessImplementation::Implementation ProcessImplementation::getMarginalProcess(const UnsignedLong i) const
{
  throw NotYetImplementedException(HERE);
}

/* Get the marginal random vector corresponding to indices components */
ProcessImplementation::Implementation ProcessImplementation::getMarginalProcess(const Indices & indices) const
{
  throw NotYetImplementedException(HERE);
}


/* Method save() stores the object through the StorageManager */
void ProcessImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "dimension_", dimension_ );
  adv.saveAttribute( "description_", description_ );
  adv.saveAttribute( "timeGrid_", timeGrid_ );
}

/* Method load() reloads the object from the StorageManager */
void ProcessImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "dimension_", dimension_ );
  adv.loadAttribute( "description_", description_ );
  adv.loadAttribute( "timeGrid_", timeGrid_ );
}

END_NAMESPACE_OPENTURNS
