//                                               -*- C++ -*-
/**
 *  @file  SecondOrderModelImplementation.cxx
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include "SecondOrderModelImplementation.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"
#include "TimeSeries.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class SecondOrderModelImplementation
 */

CLASSNAMEINIT(SecondOrderModelImplementation);

static Factory<SecondOrderModelImplementation> RegisteredFactory("SecondOrderModelImplementation");

/* Constructor without parameters */
SecondOrderModelImplementation::SecondOrderModelImplementation(const String & name)
  : PersistentObject(name),
    covarianceModel_(),
    spectralModel_()
{
  // Nothing to do
}

SecondOrderModelImplementation::SecondOrderModelImplementation(const CovarianceModel & covarianceModel,
                                                               const SpectralModel & spectralModel,
                                                               const String & name)
  : PersistentObject(name),
    covarianceModel_(),
    spectralModel_()
{
  setModels(covarianceModel, spectralModel);
}

/* Virtual constructor */
SecondOrderModelImplementation * SecondOrderModelImplementation::clone() const
{
  return new SecondOrderModelImplementation(*this);
}

/* Dimension accessor */
UnsignedLong SecondOrderModelImplementation::getDimension() const
{
  return covarianceModel_.getDimension();
}

/* Computation of the covariance function */
CovarianceMatrix SecondOrderModelImplementation::computeCovariance(const NumericalScalar s,
                                                                   const NumericalScalar t) const
{
  return covarianceModel_.computeCovariance(s, t);
}

CovarianceMatrix SecondOrderModelImplementation::computeCovariance(const NumericalScalar tau) const
{
  return covarianceModel_.computeCovariance(tau);
}

/* Computation of the spectral density function */
HermitianMatrix SecondOrderModelImplementation::computeSpectralDensity(const NumericalScalar frequency) const
{
  return spectralModel_.computeSpectralDensity(frequency);
}

/* Discretize the covariance function on a given TimeGrid */
CovarianceMatrix SecondOrderModelImplementation::discretizeCovariance(const RegularGrid & timeGrid) const
{
  return covarianceModel_.discretizeCovariance(timeGrid) ;
}

/* String converter */
String SecondOrderModelImplementation::__repr__() const
{
  OSS oss;
  oss << "class= " << SecondOrderModelImplementation::GetClassName();
  oss << " covariance model=" << covarianceModel_.__repr__()
      << " spectral model=" << spectralModel_.__repr__();
  return oss;
}

/* SpectralModel and Covariance model accessor */
SpectralModel SecondOrderModelImplementation::getSpectralModel() const
{
  return spectralModel_;
}

CovarianceModel SecondOrderModelImplementation::getCovarianceModel() const
{
  return covarianceModel_;
}

void SecondOrderModelImplementation::setModels(const CovarianceModel & covarianceModel,
                                               const SpectralModel & spectralModel)
{
  if (!covarianceModel.isStationary()) throw InvalidArgumentException(HERE) << "Error: the covariance model is not stationary.";
  if (covarianceModel.getDimension() != spectralModel.getDimension()) throw InvalidDimensionException(HERE) << "Error: the spectral model and the covariance model have different dimensions"
                                                                                                            << " spectral dimension = " << spectralModel.getDimension()
                                                                                                            << " covariance dimension = " << covarianceModel.getDimension();
  covarianceModel_ = covarianceModel;
  spectralModel_ = spectralModel;
}

/* String converter */
String SecondOrderModelImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << offset << "class= " << SecondOrderModelImplementation::GetClassName();
  oss << covarianceModel_.__str__(offset) << spectralModel_.__str__(offset);
  return oss;
}

/* Method save() stores the object through the StorageManager */
void SecondOrderModelImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);

  adv.saveAttribute( "spectralModel_", spectralModel_ );
  adv.saveAttribute( "covarianceModel_", covarianceModel_);
}

/* Method load() reloads the object from the StorageManager */
void SecondOrderModelImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);

  adv.loadAttribute( "spectralModel_", spectralModel_ );
  adv.loadAttribute( "covarianceModel_", covarianceModel_ );
}

END_NAMESPACE_OPENTURNS
