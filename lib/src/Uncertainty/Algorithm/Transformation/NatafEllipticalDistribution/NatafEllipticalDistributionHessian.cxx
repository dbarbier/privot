//                                               -*- C++ -*-
/**
 *  @file  NatafEllipticalDistributionHessian.cxx
 *  @brief Class for the Nataf transformation hessian for elliptical
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
#include "OTprivate.hxx"
#include "NatafEllipticalDistributionHessian.hxx"
#include "SymmetricTensor.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/*
 * @class NatafEllipticalDistributionHessian
 *
 * This class offers an interface for the Nataf function for elliptical distributions
 */

CLASSNAMEINIT(NatafEllipticalDistributionHessian);

static Factory<NatafEllipticalDistributionHessian> RegisteredFactory("NatafEllipticalDistributionHessian");

/* Default constructor */
NatafEllipticalDistributionHessian::NatafEllipticalDistributionHessian():
  ConstantNumericalMathHessianImplementation()
{
  // Nothing to do
}

/* Parameter constructor */
NatafEllipticalDistributionHessian::NatafEllipticalDistributionHessian(const UnsignedLong dimension):
  ConstantNumericalMathHessianImplementation(SymmetricTensor(dimension, dimension))
{
  // Nothing to do
}

/* String converter */
String NatafEllipticalDistributionHessian::__repr__() const
{
  return OSS() << "class=" << NatafEllipticalDistributionHessian::GetClassName();
}

/* Method save() stores the object through the StorageManager */
void NatafEllipticalDistributionHessian::save(Advocate & adv) const
{
  ConstantNumericalMathHessianImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void NatafEllipticalDistributionHessian::load(Advocate & adv)
{
  ConstantNumericalMathHessianImplementation::load(adv);
}

END_NAMESPACE_OPENTURNS
