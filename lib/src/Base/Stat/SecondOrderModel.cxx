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
#include "SecondOrderModel.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(SecondOrderModel);

/* Constructor with parameters */
SecondOrderModel::SecondOrderModel()
  : TypedInterfaceObject<SecondOrderModelImplementation>(new SecondOrderModelImplementation())
{
  // Nothing to do
}

SecondOrderModel::SecondOrderModel(const CovarianceModel & covarianceModel,
                                   const SpectralModel & spectralModel,
                                   const String & name)
  : TypedInterfaceObject<SecondOrderModelImplementation>(new SecondOrderModelImplementation(covarianceModel, spectralModel))
{
  getImplementation()->setName(name);
}

/* Parameters constructor */
SecondOrderModel::SecondOrderModel(const SecondOrderModelImplementation & implementation,
                                   const String & name)
  : TypedInterfaceObject<SecondOrderModelImplementation>(implementation.clone())
{
  getImplementation()->setName(name);
}

/* Constructor from implementation */
SecondOrderModel::SecondOrderModel(const Implementation & p_implementation,
                                   const String & name)
  : TypedInterfaceObject<SecondOrderModelImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* Constructor from implementation pointer */
SecondOrderModel::SecondOrderModel(SecondOrderModelImplementation * p_implementation,
                                   const String & name)
  : TypedInterfaceObject<SecondOrderModelImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* SpectralModel and CovarianceModel accessor */
SpectralModel SecondOrderModel::getSpectralModel() const
{
  return getImplementation()->getSpectralModel();
}

CovarianceModel SecondOrderModel::getCovarianceModel() const
{
  return getImplementation()->getCovarianceModel();
}

void SecondOrderModel::setModels(const CovarianceModel & covarianceModel,
                                 const SpectralModel & spectralModel)
{
  copyOnWrite();
  getImplementation()->setModels(covarianceModel, spectralModel);
}

/* Dimension accessor */
UnsignedLong SecondOrderModel::getDimension() const
{
  return getImplementation()->getDimension();
}

/* Computation of the covariance matrix */
CovarianceMatrix SecondOrderModel::computeCovariance(const NumericalScalar s,
                                                     const NumericalScalar t) const
{
  return getImplementation()->computeCovariance(s, t);
}

/* Computation of the covariance matrix - stationnary case */
CovarianceMatrix SecondOrderModel::computeCovariance(const NumericalScalar tau) const
{
  return getImplementation()->computeCovariance(tau);
}

/* Computation of the spectral density function */
HermitianMatrix SecondOrderModel::computeSpectralDensity(const NumericalScalar frequency) const
{
  return getImplementation()->computeSpectralDensity(frequency);
}

/* Discretize the covariance function on a given TimeGrid */
CovarianceMatrix SecondOrderModel::discretizeCovariance(const RegularGrid & timeGrid) const
{
  return getImplementation()->discretizeCovariance(timeGrid);
}

/* String converter */
String SecondOrderModel::__repr__() const
{
  OSS oss;
  oss << "class=" << SecondOrderModel::GetClassName()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}

/* String converter */
String SecondOrderModel::__str__(const String & offset) const
{
  OSS oss;
  oss << "class=" << SecondOrderModel::GetClassName()
      << " implementation=" << getImplementation()->__str__(offset);
  return oss;
}

END_NAMESPACE_OPENTURNS
