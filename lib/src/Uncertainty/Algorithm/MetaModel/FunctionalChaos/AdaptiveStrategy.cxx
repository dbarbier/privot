//                                               -*- C++ -*-
/**
 *  @file  AdaptiveStrategy.cxx
 *  @brief This is the interface class for adaptive strategies
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
 *  @author dutka
 *  @date   2008-05-21 11:21:38 +0200 (Wed, 21 May 2008)
 */
#include "AdaptiveStrategy.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "FixedStrategy.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(AdaptiveStrategy);

/* Constructor from an orthogonal basis */
AdaptiveStrategy::AdaptiveStrategy()
  : TypedInterfaceObject<AdaptiveStrategyImplementation>(new AdaptiveStrategyImplementation)
{
  // Nothing to do
}


/* Constructor from an orthogonal basis */
AdaptiveStrategy::AdaptiveStrategy(const OrthogonalBasis & basis,
                                   const UnsignedLong maximumDimension)
  : TypedInterfaceObject<AdaptiveStrategyImplementation>(new FixedStrategy(basis, maximumDimension))
{
  // Nothing to do
}


/* Constructor from implementation */
AdaptiveStrategy::AdaptiveStrategy(const AdaptiveStrategyImplementation & implementation)
  : TypedInterfaceObject<AdaptiveStrategyImplementation>(implementation.clone())
{
  // Nothing to do
}


/* Basis accessor */
OrthogonalBasis AdaptiveStrategy::getBasis() const
{
  return getImplementation()->getBasis();
}


/* Maximum dimension accessor */
void AdaptiveStrategy::setMaximumDimension(const UnsignedLong maximumDimension)
{
  copyOnWrite();
  getImplementation()->setMaximumDimension(maximumDimension);
}

UnsignedLong AdaptiveStrategy::getMaximumDimension() const
{
  return getImplementation()->getMaximumDimension();
}


/* Compute initial basis for the approximation */
void AdaptiveStrategy::computeInitialBasis()
{
  getImplementation()->computeInitialBasis();
}


/* Update the basis for the next iteration of approximation */
void AdaptiveStrategy::updateBasis(const NumericalPoint & alpha_k_p_,
                                   const NumericalScalar residual_p_,
                                   const NumericalScalar relativeError_p_)

{
  getImplementation()->updateBasis(alpha_k_p_, residual_p_, relativeError_p_);
}


/* Psi accessor */
AdaptiveStrategy::NumericalMathFunctionCollection AdaptiveStrategy::getPsi() const
{
  return getImplementation()->getPsi();
}


/* String converter */
String AdaptiveStrategy::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " implementation=" << getImplementation()->__repr__();
}


/* String converter */
String AdaptiveStrategy::__str__(const String & offset) const
{
  return __repr__();
}




END_NAMESPACE_OPENTURNS
