//                                               -*- C++ -*-
/**
 *  @file  SpectralModelFactoryImplementation.cxx
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
#include "PersistentObjectFactory.hxx"
#include "SpectralModelFactoryImplementation.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(SpectralModelFactoryImplementation);
static Factory<SpectralModelFactoryImplementation> RegisteredFactory("SpectralModelFactoryImplementation");

typedef NotYetImplementedException NotYetImplementedException;

/* Default constructor */
SpectralModelFactoryImplementation::SpectralModelFactoryImplementation(const String & name)
  : PersistentObject(name),
    frequencyGrid_(),
    fftAlgorithm_()
{
  // Nothing to do
}

/* Virtual constructor */
SpectralModelFactoryImplementation * SpectralModelFactoryImplementation::clone() const
{
  return new SpectralModelFactoryImplementation(*this);
}


/* Frequency grid accessor */
RegularGrid SpectralModelFactoryImplementation::getFrequencyGrid() const
{
  return frequencyGrid_;
}

/* Frequency grid accessor */
void SpectralModelFactoryImplementation::setFrequencyGrid(const RegularGrid & grid)
{
  frequencyGrid_ = grid;
}

/* FFT algorithm accessor */
FFT SpectralModelFactoryImplementation::getFFTAlgorithm() const
{
  return fftAlgorithm_;
}

/* FFT algorithm accessor */
void SpectralModelFactoryImplementation::setFFTAlgorithm(const FFT & fft)
{
  fftAlgorithm_ = fft;
}


/* String converter */
String SpectralModelFactoryImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << SpectralModelFactoryImplementation::GetClassName();
  return oss;
}

/* String converter */
String SpectralModelFactoryImplementation::__str__(const String & offset) const
{
  return this->getClassName();
}

/* Here is the interface that all derived class must implement */

SpectralModelImplementation * SpectralModelFactoryImplementation::build(const ProcessSample & sample) const
{
  throw NotYetImplementedException(HERE);
}

SpectralModelImplementation * SpectralModelFactoryImplementation::build(const TimeSeries & timeSeries) const
{
  throw NotYetImplementedException(HERE);
}

/* Method save() stores the object through the StorageManager */
void SpectralModelFactoryImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "fftAlgorithm_", fftAlgorithm_);
}

/* Method load() reloads the object from the StorageManager */
void SpectralModelFactoryImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "fftAlgorithm_", fftAlgorithm_);
}
END_NAMESPACE_OPENTURNS
