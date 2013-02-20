//                                               -*- C++ -*-
/**
 *  @file  SecondOrderModelImplementation.hxx
 *  @brief This class is enables to build a second order model
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
 *  Id      SecondOrderModelImplementation.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_SECONDORDERMODELIMPLEMENTATION_HXX
#define OPENTURNS_SECONDORDERMODELIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "CovarianceMatrix.hxx"
#include "Pointer.hxx"
#include "HermitianMatrix.hxx"
#include "CovarianceModelImplementation.hxx"
#include "StationaryCovarianceModel.hxx"
#include "SpectralModel.hxx"

BEGIN_NAMESPACE_OPENTURNS



class RegularGrid;

/**
 * @class SecondOrderModelImplementation
 */

class SecondOrderModelImplementation
  : public PersistentObject
{

  CLASSNAME;

public:

  /** Default constructor without parameters */
  SecondOrderModelImplementation(const String & name = DefaultName);

  /** Constructor with covariance model and spectral model */
  SecondOrderModelImplementation(const CovarianceModel & covarianceModel,
                                 const SpectralModel & spectralModel,
                                 const String & name = DefaultName);

  /** Virtual copy constructor */
  virtual SecondOrderModelImplementation * clone() const;

  /** Dimension accessor */
  virtual UnsignedLong getDimension() const;

  /** Computation of the covariance matrix */
  virtual CovarianceMatrix computeCovariance(const NumericalScalar s,
                                             const NumericalScalar t) const;

  virtual CovarianceMatrix computeCovariance(const NumericalScalar tau) const;

  /** SpectralModel and Covariance model accessor */
  SpectralModel getSpectralModel() const;
  CovarianceModel getCovarianceModel() const;
  void setModels(const CovarianceModel & covarianceModel,
                 const SpectralModel & spectralModel);

  /** Discretize the covariance function on a given TimeGrid */
  virtual CovarianceMatrix discretizeCovariance(const RegularGrid & timeGrid) const;

  /** Computation of the spectral density function */
  virtual HermitianMatrix computeSpectralDensity(const NumericalScalar frequency) const;

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

  /** The covariance model part - must be stationary */
  CovarianceModel covarianceModel_;

  /** The spectral model part */
  SpectralModel spectralModel_;

} ; /* class SecondOrderModelImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SECONDORDERMODELIMPLEMENTATION_HXX */
