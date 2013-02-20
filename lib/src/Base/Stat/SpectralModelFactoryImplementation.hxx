//                                               -*- C++ -*-
/**
 *  @file  SpectralModelFactoryImplementation.hxx
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 *  Id      SpectralModelFactoryImplementation.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_SPECTRALMODELFACTORYIMPLEMENTATION_HXX
#define OPENTURNS_SPECTRALMODELFACTORYIMPLEMENTATION_HXX

#include "SpectralModel.hxx"
#include "ProcessSample.hxx"
#include "TimeSeriesImplementation.hxx"
#include "TimeSeries.hxx"
#include "NumericalPoint.hxx"
#include "PersistentObject.hxx"
#include "FFT.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class SpectralModelFactoryImplementation
 */
class SpectralModelFactoryImplementation
  : public PersistentObject
{
  CLASSNAME;
public:

  typedef Pointer<SpectralModelFactoryImplementation>    Implementation;

  /** Default constructor */
  SpectralModelFactoryImplementation(const String & name = DefaultName);

  /** Virtual constructor */
  virtual SpectralModelFactoryImplementation * clone() const;

  /** Frequency grid accessor */
  RegularGrid getFrequencyGrid() const;

  /** Frequency grid accessor */
  void setFrequencyGrid(const RegularGrid & grid);

  /** FFT algorithm accessor */
  FFT getFFTAlgorithm() const;

  /** FFT algorithm accessor */
  void setFFTAlgorithm(const FFT & fft);

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

  /* Here is the interface that all derived class must implement */
  /** Build a a spectral model based on a sample */
  virtual SpectralModelImplementation * build(const ProcessSample & sample) const;

  /** Build a a spectral model based on a TimeSeries */
  virtual SpectralModelImplementation * build(const TimeSeries & timeSerie) const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

  /** Frequency grid */
  RegularGrid frequencyGrid_;

  /** FFT algorithm */
  FFT fftAlgorithm_;

}; /* class SpectralModelFactoryImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SPECTRALMODELFACTORYIMPLEMENTATION_HXX */
