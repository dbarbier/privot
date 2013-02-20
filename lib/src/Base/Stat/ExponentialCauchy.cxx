//                                               -*- C++ -*-
/**
 *  @file  ExponentialCauchy.cxx
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include "ExponentialCauchy.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"
#include "ExponentialModel.hxx"
#include "CauchyModel.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ExponentialCauchy
 */

CLASSNAMEINIT(ExponentialCauchy);

static Factory<ExponentialCauchy> RegisteredFactory("ExponentialCauchy");

/* Constructor with parameters */
ExponentialCauchy::ExponentialCauchy(const String & name)
  : SecondOrderModelImplementation(ExponentialModel(), CauchyModel(), name)
{
  // Nothing to do
}

ExponentialCauchy::ExponentialCauchy(const NumericalPoint & amplitude,
                                     const NumericalPoint & scale,
                                     const String & name)
  : SecondOrderModelImplementation(ExponentialModel(amplitude, scale), CauchyModel(amplitude, scale), name)
{
  // Nothing to do
}

ExponentialCauchy::ExponentialCauchy(const NumericalPoint & amplitude,
                                     const NumericalPoint & scale,
                                     const CorrelationMatrix & spatialCorrelation,
                                     const String & name)
  : SecondOrderModelImplementation(ExponentialModel(amplitude, scale, spatialCorrelation), CauchyModel(amplitude, scale, spatialCorrelation), name)
{
  // Nothing to do
}

ExponentialCauchy::ExponentialCauchy(const NumericalPoint & scale,
                                     const CovarianceMatrix & spatialCovariance,
                                     const String & name)
  : SecondOrderModelImplementation(ExponentialModel(scale, spatialCovariance), CauchyModel(scale, spatialCovariance), name)
{
  // Nothing to do
}

/* Virtual constructor */
ExponentialCauchy * ExponentialCauchy::clone() const
{
  return new ExponentialCauchy(*this);
}

/* String converter */
String ExponentialCauchy::__repr__() const
{
  OSS oss;
  oss << "class=" << ExponentialCauchy::GetClassName()
      << " derived from " << SecondOrderModelImplementation::__repr__();
  return oss;
}

/* String converter */
String ExponentialCauchy::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << "class=" << ExponentialCauchy::GetClassName();
  oss << " amplitude=" << getAmplitude().__str__(offset)
      << " scale=" << getScale().__str__(offset)
      << " spatial correlation=" << getSpatialCorrelation().__str__(offset);
  return oss;
}

/* Amplitude accessor */
NumericalPoint ExponentialCauchy::getAmplitude() const
{
  return static_cast<ExponentialModel*>(covarianceModel_.getImplementation().get())->getAmplitude();
}

/* Scale accessor */
NumericalPoint ExponentialCauchy::getScale() const
{
  return static_cast<ExponentialModel*>(covarianceModel_.getImplementation().get())->getScale();
}

/* Spatial correlation accessor */
CorrelationMatrix ExponentialCauchy::getSpatialCorrelation() const
{
  return static_cast<ExponentialModel*>(covarianceModel_.getImplementation().get())->getSpatialCorrelation();
}

/* Method save() stores the object through the StorageManager */
void ExponentialCauchy::save(Advocate & adv) const
{
  SecondOrderModelImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void ExponentialCauchy::load(Advocate & adv)
{
  SecondOrderModelImplementation::load(adv);
}
END_NAMESPACE_OPENTURNS
