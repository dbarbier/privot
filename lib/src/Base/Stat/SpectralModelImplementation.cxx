//                                               -*- C++ -*-
/**
 *  @file  SpectralModelImplementation.cxx
 *  @brief
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include "SpectralModelImplementation.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(SpectralModelImplementation);
static Factory<SpectralModelImplementation> RegisteredFactory("SpectralModelImplementation");

/* Constructor with parameters */
SpectralModelImplementation::SpectralModelImplementation(const String & name)
  : PersistentObject(name),
    dimension_(0),
    frequencyGrid_()
{
  // Nothing to do
}

/* Virtual constructor */
SpectralModelImplementation * SpectralModelImplementation::clone() const
{
  return new SpectralModelImplementation(*this);
}

/* Dimension accessor */
UnsignedLong SpectralModelImplementation::getDimension() const
{
  return dimension_;
}

/* Dimension accessor */
void SpectralModelImplementation::setDimension(const UnsignedLong dimension)
{
  dimension_ = dimension;
}

/* Frequency grid accessors */
RegularGrid SpectralModelImplementation::getFrequencyGrid() const
{
  return frequencyGrid_;
}

void SpectralModelImplementation::setFrequencyGrid(const RegularGrid & frequencyGrid)
{
  frequencyGrid_ = frequencyGrid;
}

/* Computation of the spectral density function */
HermitianMatrix SpectralModelImplementation::computeSpectralDensity(const NumericalScalar frequency) const
{
  throw NotYetImplementedException(HERE);
}


/* String converter */
String SpectralModelImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << SpectralModelImplementation::GetClassName();
  return oss;
}

/* String converter */
String SpectralModelImplementation::__str__(const String & offset) const
{
  OSS oss;
  oss << "class=" << SpectralModelImplementation::GetClassName();
  return oss;
}


/* Method save() stores the object through the StorageManager */
void SpectralModelImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute("dimension_", dimension_);
  adv.saveAttribute("frequencyGrid_", frequencyGrid_);
}

/* Method load() reloads the object from the StorageManager */
void SpectralModelImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute("dimension_", dimension_);
  adv.loadAttribute("frequencyGrid_", frequencyGrid_);
}

END_NAMESPACE_OPENTURNS
