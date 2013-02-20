//                                               -*- C++ -*-
/**
 *  @file  DualLinearCombinationHessianImplementation.cxx
 *  @brief The hessian part of functional linear combination of vectors
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
#include "DualLinearCombinationHessianImplementation.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "SymmetricMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(DualLinearCombinationHessianImplementation);

static Factory<DualLinearCombinationHessianImplementation> RegisteredFactory("DualLinearCombinationHessianImplementation");


/* Default constructor */
DualLinearCombinationHessianImplementation::DualLinearCombinationHessianImplementation()
  : NumericalMathHessianImplementation(),
    evaluation_()
{
  // Nothing to do
}


/* Parameters constructor */
DualLinearCombinationHessianImplementation::DualLinearCombinationHessianImplementation(const DualLinearCombinationEvaluationImplementation & evaluation)
  : NumericalMathHessianImplementation(),
    evaluation_(evaluation)
{
  // Nothing to do
}

/* Virtual constructor */
DualLinearCombinationHessianImplementation * DualLinearCombinationHessianImplementation::clone() const
{
  return new DualLinearCombinationHessianImplementation(*this);
}


/* Hessian method */
SymmetricTensor DualLinearCombinationHessianImplementation::hessian(const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inP.getDimension();
  const UnsignedLong size(evaluation_.functionsCollection_.getSize());
  const UnsignedLong outputDimension(getOutputDimension());
  SymmetricTensor result(inputDimension, outputDimension);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const SymmetricTensor hessianI(evaluation_.functionsCollection_[i].hessian(inP));
      const NumericalPoint coefficientI(evaluation_.coefficients_[i]);
      for (UnsignedLong j = 0; j < inputDimension; ++j)
        for (UnsignedLong k = 0; k <= j; ++k)
          for (UnsignedLong n = 0; n < outputDimension; ++n)
            result(j, k, n) += hessianI(j, k, 0) * coefficientI[n];
    }
  return result;
}

/* Accessor for input point dimension */
UnsignedLong DualLinearCombinationHessianImplementation::getInputDimension() const
{
  return evaluation_.getInputDimension();
}

/* Accessor for output point dimension */
UnsignedLong DualLinearCombinationHessianImplementation::getOutputDimension() const
{
  return evaluation_.getOutputDimension();
}

/* String converter */
String DualLinearCombinationHessianImplementation::__repr__() const
{
  return OSS() << "class=" << GetClassName()
               << " evaluation=" << evaluation_;
}


/* Method save() stores the object through the StorageManager */
void DualLinearCombinationHessianImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "evaluation_", evaluation_ );
}


/* Method load() reloads the object from the StorageManager */
void DualLinearCombinationHessianImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "evaluation_", evaluation_ );
}

END_NAMESPACE_OPENTURNS
