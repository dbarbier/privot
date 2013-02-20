//                                               -*- C++ -*-
/**
 *  @file  CovarianceModel.cxx
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include "CovarianceModel.hxx"
#include "ExponentialModel.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class CovarianceModel
 */

CLASSNAMEINIT(CovarianceModel);

//   static Factory<CovarianceModel> RegisteredFactory("CovarianceModel");

/* Constructor without parameters */
CovarianceModel::CovarianceModel(const String & name)
  : TypedInterfaceObject<CovarianceModelImplementation>(new ExponentialModel(name))
{
  // Nothing to do
}

/* Parameters constructor */
CovarianceModel::CovarianceModel(const CovarianceModelImplementation & implementation,
                                 const String & name)
  : TypedInterfaceObject<CovarianceModelImplementation>(implementation.clone())
{
  getImplementation()->setName(name);
}

/* Constructor from implementation */
CovarianceModel::CovarianceModel(const Implementation & p_implementation,
                                 const String & name)
  : TypedInterfaceObject<CovarianceModelImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* Constructor from implementation pointer */
CovarianceModel::CovarianceModel(CovarianceModelImplementation * p_implementation,
                                 const String & name)
  : TypedInterfaceObject<CovarianceModelImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* Dimension accessor */
UnsignedLong CovarianceModel::getDimension() const
{
  return getImplementation()->getDimension();
}

/* Computation of the covariance function */
CovarianceMatrix CovarianceModel::computeCovariance(const NumericalScalar s,
                                                    const NumericalScalar t) const
{
  return getImplementation()->computeCovariance(s, t);
}

/* Computation of the covariance function */
CovarianceMatrix CovarianceModel::computeCovariance(const NumericalScalar tau) const
{
  return getImplementation()->computeCovariance(tau);
}

/* Discretize the covariance function on a given TimeGrid */
CovarianceMatrix CovarianceModel::discretizeCovariance(const RegularGrid & timeGrid) const
{
  return getImplementation()->discretizeCovariance(timeGrid);
}

/* Is the underlying a stationary covariance model ? */
Bool CovarianceModel::isStationary() const
{
  return getImplementation()->isStationary();
}

/* String converter */
String CovarianceModel::__repr__() const
{
  return getImplementation()->__repr__();
}

/* String converter */
String CovarianceModel::__str__(const String & offset) const
{
  return getImplementation()->__str__(offset);
}

END_NAMESPACE_OPENTURNS
