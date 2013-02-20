//                                               -*- C++ -*-
/**
 *  @file  SpectralModel.cxx
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
#include "SpectralModel.hxx"
#include "CauchyModel.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(SpectralModel);

//   static Factory<SpectralModel> RegisteredFactory("SpectralModel");

/* Constructor with parameters */
SpectralModel::SpectralModel(const String & name)
  : TypedInterfaceObject<SpectralModelImplementation>(new CauchyModel(name))
{
  // Nothing to do
}

/* Parameters constructor */
SpectralModel::SpectralModel(const SpectralModelImplementation & implementation,
                             const String & name)
  : TypedInterfaceObject<SpectralModelImplementation>(implementation.clone())
{
  getImplementation()->setName(name);
}

/* Constructor from implementation */
SpectralModel::SpectralModel(const Implementation & p_implementation,
                             const String & name)
  : TypedInterfaceObject<SpectralModelImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* Constructor from implementation pointer */
SpectralModel::SpectralModel(SpectralModelImplementation * p_implementation,
                             const String & name)
  : TypedInterfaceObject<SpectralModelImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}


/* Dimension accessor */
UnsignedLong SpectralModel::getDimension() const
{
  return getImplementation()->getDimension();
}

/* Dimension accessor */
void SpectralModel::setDimension(const UnsignedLong dimension)
{
  copyOnWrite();
  getImplementation()->setDimension(dimension);
}

/* Frequency grid accessors */
RegularGrid SpectralModel::getFrequencyGrid() const
{
  return getImplementation()->getFrequencyGrid();
}

void SpectralModel::setFrequencyGrid(const RegularGrid & frequencyGrid)
{
  copyOnWrite();
  getImplementation()->setFrequencyGrid(frequencyGrid);
}

/* Computation of the spectral density function */
HermitianMatrix SpectralModel::computeSpectralDensity(const NumericalScalar frequency) const
{
  return getImplementation()->computeSpectralDensity(frequency);
}


/* String converter */
String SpectralModel::__repr__() const
{
  return getImplementation()->__repr__();
}

/* String converter */
String SpectralModel::__str__(const String & offset) const
{
  return getImplementation()->__str__(offset);
}

END_NAMESPACE_OPENTURNS
