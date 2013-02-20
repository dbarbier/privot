//                                               -*- C++ -*-
/**
 *  @file  SpectralModelFactory.cxx
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
 *  @author schueller
 *  @author schueller
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include "SpectralModelFactory.hxx"


BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(SpectralModelFactory);

/* Default constructor */
SpectralModelFactory::SpectralModelFactory(const String & name)
  : TypedInterfaceObject<SpectralModelFactoryImplementation>(new SpectralModelFactoryImplementation(name))
{
  // Nothing to do
}

/* Parameters constructor */
SpectralModelFactory::SpectralModelFactory(const SpectralModelFactoryImplementation & implementation,
                                           const String & name)
  : TypedInterfaceObject<SpectralModelFactoryImplementation>(implementation.clone())
{
  getImplementation()->setName(name);
}

/* Constructor from implementation */
SpectralModelFactory::SpectralModelFactory(const Implementation & p_implementation,
                                           const String & name)
  : TypedInterfaceObject<SpectralModelFactoryImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* Constructor from implementation pointer */
SpectralModelFactory::SpectralModelFactory(SpectralModelFactoryImplementation * p_implementation,
                                           const String & name)
  : TypedInterfaceObject<SpectralModelFactoryImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}


/* Virtual constructor */
SpectralModelFactory * SpectralModelFactory::clone() const
{
  return new SpectralModelFactory(*this);
}

/* Frequency grid accessor */
RegularGrid SpectralModelFactory::getFrequencyGrid() const
{
  return getImplementation()->getFrequencyGrid();
}

/* Frequency grid accessor */
void SpectralModelFactory::setFrequencyGrid(const RegularGrid & grid)
{
  copyOnWrite();
  getImplementation()->setFrequencyGrid(grid);
}

/* FFT algorithm accessor */
FFT SpectralModelFactory::getFFTAlgorithm() const
{
  return getImplementation()->getFFTAlgorithm();
}

/* FFT algorithm accessor */
void SpectralModelFactory::setFFTAlgorithm(FFT & fft)
{
  copyOnWrite();
  getImplementation()->setFFTAlgorithm(fft);
}

/* String converter */
String SpectralModelFactory::__repr__() const
{
  OSS oss;
  oss << "class=" << SpectralModelFactory::GetClassName()
      << " implementation = " << getImplementation()->__repr__();
  return oss;
}

/* String converter */
String SpectralModelFactory::__str__(const String & offset) const
{
  return getImplementation()->__str__();
}

SpectralModel SpectralModelFactory::build(const ProcessSample & sample) const
{
  return getImplementation()->build(sample);
}

SpectralModel SpectralModelFactory::build(const TimeSeries & timeSeries) const
{
  return getImplementation()->build(timeSeries);
}

END_NAMESPACE_OPENTURNS
