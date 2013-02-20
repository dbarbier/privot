//                                               -*- C++ -*-
/**
 *  @file  AdaptiveStrategyImplementation.cxx
 *  @brief This is a abstract class for adaptive strategy implementations
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
#include "AdaptiveStrategyImplementation.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(AdaptiveStrategyImplementation);

static Factory<AdaptiveStrategyImplementation> RegisteredFactory("AdaptiveStrategyImplementation");


/* Default constructor */
AdaptiveStrategyImplementation::AdaptiveStrategyImplementation()
  : PersistentObject(),
    basis_(),
    maximumDimension_(0),
    I_p_(0),
    addedPsi_k_ranks_(0),
    conservedPsi_k_ranks_(0),
    removedPsi_k_ranks_(0),
    Psi_k_p_(0)
{
  // Nothing to do
}

/* Constructor from an orthogonal basis */
AdaptiveStrategyImplementation::AdaptiveStrategyImplementation(const OrthogonalBasis & basis,
                                                               const UnsignedLong maximumDimension)
  : PersistentObject(),
    basis_(basis),
    maximumDimension_(maximumDimension),
    I_p_(0),
    addedPsi_k_ranks_(0),
    conservedPsi_k_ranks_(0),
    removedPsi_k_ranks_(0),
    Psi_k_p_(0)
{
  // Nothing to do
}


/* Virtual constructor */
AdaptiveStrategyImplementation * AdaptiveStrategyImplementation::clone() const
{
  return new AdaptiveStrategyImplementation(*this);
}


/* String converter */
String AdaptiveStrategyImplementation::__repr__() const
{
  return OSS() << "class=" << GetClassName()
               << " maximumDimension=" << maximumDimension_;
}


/* Basis accessor */
OrthogonalBasis AdaptiveStrategyImplementation::getBasis() const
{
  return basis_;
}


/* Maximum dimension accessor */
void AdaptiveStrategyImplementation::setMaximumDimension(const UnsignedLong maximumDimension)
{
  maximumDimension_ = maximumDimension;
}

UnsignedLong AdaptiveStrategyImplementation::getMaximumDimension() const
{
  return maximumDimension_;
}


/* Compute initial basis for the approximation */
void AdaptiveStrategyImplementation::computeInitialBasis()
{
  throw NotYetImplementedException(HERE);
}

/* Update the basis for the next iteration of approximation */
void AdaptiveStrategyImplementation::updateBasis(const NumericalPoint & alpha_k_p_,
                                                 const NumericalScalar residual_p_,
                                                 const NumericalScalar relativeError_p_)
{
  throw NotYetImplementedException(HERE);
}

/* Psi accessor */
AdaptiveStrategyImplementation::NumericalMathFunctionCollection AdaptiveStrategyImplementation::getPsi() const
{
  return Psi_k_p_;
}

/* Method save() stores the object through the StorageManager */
void AdaptiveStrategyImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "basis_", basis_ );
}


/* Method load() reloads the object from the StorageManager */
void AdaptiveStrategyImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "basis_", basis_ );
}



END_NAMESPACE_OPENTURNS
