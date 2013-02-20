//                                               -*- C++ -*-
/**
 *  @file  SpectralModel.hxx
 *  @brief This class is enables to build a spectral model using its spectral density function
 *  This is the interface class
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
 *  Id      SpectralModel.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_SPECTRALMODEL_HXX
#define OPENTURNS_SPECTRALMODEL_HXX

#include "SpectralModelImplementation.hxx"
#include "TypedInterfaceObject.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class SpectralModel
 */

class SpectralModel
  : public TypedInterfaceObject<SpectralModelImplementation>
{

  CLASSNAME;

public:

  typedef SpectralModelImplementation::Implementation Implementation;

  /** Default constructor */
  SpectralModel(const String & name = DefaultName);

  /** Copy constructors */
  SpectralModel(const SpectralModelImplementation & implementation,
                const String & name = DefaultName);

  /** Constructor from implementation */
  SpectralModel(const Implementation & p_implementation,
                const String & name = DefaultName);

#ifndef SWIG
  /** Constructor from implementation pointer */
  SpectralModel(SpectralModelImplementation * p_implementation,
                const String & name = DefaultName);
#endif


  /** Dimension accessor */
  UnsignedLong getDimension() const;
  void setDimension(const UnsignedLong dimension);

  /** Computation of the spectral density function */
  virtual HermitianMatrix computeSpectralDensity(const NumericalScalar frequency) const;

  /** Frequency grid accessors */
  virtual RegularGrid getFrequencyGrid() const;
  virtual void setFrequencyGrid(const RegularGrid & frequencyGrid);

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

} ; /* class SpectralModel */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SPECTRALMODEL_HXX */
