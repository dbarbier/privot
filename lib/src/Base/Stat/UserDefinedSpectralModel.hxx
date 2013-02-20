//                                               -*- C++ -*-
/**
 *  @file  UserDefinedSpectralModel.hxx
 *  @brief This class build a spectral model using a frequency grid and a spectral function
 *  given as a collection of Hermitian Matrix
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
 *  Id      UserDefinedSpectralModel.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_USERDEFINEDSPECTRALMODEL_HXX
#define OPENTURNS_USERDEFINEDSPECTRALMODEL_HXX

#include "SpectralModel.hxx"
#include "PersistentCollection.hxx"
#include "Collection.hxx"
#include "TimeSeriesImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class UserDefinedSpectralModel
 */

class UserDefinedSpectralModel
  : public SpectralModelImplementation
{

  CLASSNAME;

public:

  typedef PersistentCollection<HermitianMatrix>           HermitianMatrixPersistentCollection;
  typedef Collection<HermitianMatrix>                     HermitianMatrixCollection;

  /** Default onstructor */
  UserDefinedSpectralModel(const String & name = DefaultName);

  /** Standard onstructor */
  UserDefinedSpectralModel(const RegularGrid & frequencyGrid,
                           const HermitianMatrixCollection & spectralFunction,
                           const String & name = DefaultName);

  /** Virtual copy constructor */
  UserDefinedSpectralModel * clone() const;

  /** Computation of the spectral density function */
  HermitianMatrix computeSpectralDensity(const NumericalScalar frequency) const;

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /** collection of spectral functions */
  HermitianMatrixPersistentCollection DSPCollection_;

} ; /* class UserDefinedSpectralModel */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_USERDEFINEDSPECTRALMODEL_HXX */
