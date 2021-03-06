//                                               -*- C++ -*-
/**
 *  @file   SpectralModelFactory.hxx
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
 *  Id      SpectralModelFactory.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_SPECTRALMODELFACTORY_HXX
#define OPENTURNS_SPECTRALMODELFACTORY_HXX

#include "SpectralModelFactoryImplementation.hxx"
#include "PersistentObject.hxx"
#include "TypedInterfaceObject.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class  SpectralModelFactory
 */
class  SpectralModelFactory
  : public TypedInterfaceObject<SpectralModelFactoryImplementation>
{
  CLASSNAME;
public:

  typedef SpectralModelFactoryImplementation::Implementation    Implementation;

  /** Default constructor */
  SpectralModelFactory(const String & name = DefaultName);

  /** Copy constructors */
  SpectralModelFactory(const SpectralModelFactoryImplementation & implementation,
                       const String & name = DefaultName);

  /** Constructor from implementation */
  SpectralModelFactory(const Implementation & p_implementation,
                       const String & name = DefaultName);

#ifndef SWIG
  /** Constructor from implementation pointer */
  SpectralModelFactory(SpectralModelFactoryImplementation * p_implementation,
                       const String & name = DefaultName);
#endif

  /** Virtual constructor */
  virtual  SpectralModelFactory * clone() const;

  /** Frequency grid accessor */
  RegularGrid getFrequencyGrid() const;

  /** Frequency grid accessor */
  void setFrequencyGrid(const RegularGrid & point);

  /** FFT algorithm accessor */
  FFT getFFTAlgorithm() const;

  /** FFT algorithm accessor */
  void setFFTAlgorithm(FFT & fft);

  /** String converter */
  virtual String __repr__() const;

  /** String converter */

  virtual String __str__(const String & offset = "") const;

  /* Here is the interface that all derived class must implement */
  /** Build a a spectral model based on a sample */
  virtual SpectralModel build(const ProcessSample & sample) const;

  /** Build a a spectral model based on a TimeSeries */
  virtual SpectralModel build(const TimeSeries & timeSerie) const;


protected:

}; /* class  SpectralModelFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SPECTRALMODELFACTORY_HXX */
