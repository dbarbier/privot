//                                               -*- C++ -*-
/**
 *  @file  InverseNatafEllipticalDistributionHessian.cxx
 *  @brief Class for the InverseNataf transformation hessian for elliptical
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
#include "InverseNatafEllipticalDistributionHessian.hxx"
#include "SymmetricTensor.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/*
 * @class InverseNatafEllipticalDistributionHessian
 *
 * This class offers an interface for the InverseNataf function for elliptical distributions
 */

CLASSNAMEINIT(InverseNatafEllipticalDistributionHessian);

static Factory<InverseNatafEllipticalDistributionHessian> RegisteredFactory("InverseNatafEllipticalDistributionHessian");

/* Default constructor */
InverseNatafEllipticalDistributionHessian::InverseNatafEllipticalDistributionHessian():
  ConstantNumericalMathHessianImplementation()
{
  // Nothing to do
}

/* Parameter constructor */
InverseNatafEllipticalDistributionHessian::InverseNatafEllipticalDistributionHessian(const UnsignedLong dimension):
  ConstantNumericalMathHessianImplementation(SymmetricTensor(dimension, dimension))
{
  // Nothing to do
}

/* String converter */
String InverseNatafEllipticalDistributionHessian::__repr__() const
{
  return OSS() << "class=" << InverseNatafEllipticalDistributionHessian::GetClassName();
}

/* Method save() stores the object through the StorageManager */
void InverseNatafEllipticalDistributionHessian::save(Advocate & adv) const
{
  ConstantNumericalMathHessianImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void InverseNatafEllipticalDistributionHessian::load(Advocate & adv)
{
  ConstantNumericalMathHessianImplementation::load(adv);
}

END_NAMESPACE_OPENTURNS
