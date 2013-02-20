//                                               -*- C++ -*-
/**
 *  @file  ConstantNumericalMathGradientImplementation.cxx
 *  @brief Class for a constant numerical math gradient implementation
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
#include "ConstantNumericalMathGradientImplementation.hxx"
#include "PersistentObjectFactory.hxx"
#include "Os.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(ConstantNumericalMathGradientImplementation);

static Factory<ConstantNumericalMathGradientImplementation> RegisteredFactory("ConstantNumericalMathGradientImplementation");


/* Default constructor */
ConstantNumericalMathGradientImplementation::ConstantNumericalMathGradientImplementation()
  : NumericalMathGradientImplementation()
{
  // Nothing to do
}

/* Parameter constructor */
ConstantNumericalMathGradientImplementation::ConstantNumericalMathGradientImplementation(const Matrix & constant)
  : NumericalMathGradientImplementation(),
    constant_(constant)
{
  /* Check if the dimensions of the constant term is compatible with the linear term */
}

/* Virtual constructor */
ConstantNumericalMathGradientImplementation * ConstantNumericalMathGradientImplementation::clone() const
{
  return new ConstantNumericalMathGradientImplementation(*this);
}

/* Comparison operator */
Bool ConstantNumericalMathGradientImplementation::operator ==(const ConstantNumericalMathGradientImplementation & other) const
{
  return (constant_ == other.constant_);
}

/* String converter */
String ConstantNumericalMathGradientImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << ConstantNumericalMathGradientImplementation::GetClassName()
      << " name=" << getName()
      << " constant=" << constant_;
  return oss;
}

/* String converter */
String ConstantNumericalMathGradientImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  const UnsignedLong inputDimension(getInputDimension());
  Description description(getDescription());
  if (description.getSize() == 0) for (UnsignedLong i = 0; i < inputDimension; ++i) description.add(OSS() << "x" << i);
  if (hasVisibleName()) oss << offset << getName() << ":" << Os::GetEndOfLine();
  for (UnsignedLong i = 0; i < inputDimension; ++i)
    {
      if (i == 0) oss << offset << "  (";
      oss << description[i];
      if (i == inputDimension - 1) oss << ")";
      else oss << ", ";
    }
  oss << " ->" << Os::GetEndOfLine();
  oss << constant_.__str__(offset + "  ");
  return oss;
}

/* Accessor for the constant term */
Matrix ConstantNumericalMathGradientImplementation::getConstant() const
{
  return constant_;
}

/* Here is the interface that all derived class must implement */

/* Operator () */
Matrix ConstantNumericalMathGradientImplementation::gradient(const NumericalPoint & inP) const
{
  if (inP.getDimension() != constant_.getNbRows()) throw InvalidArgumentException(HERE) << "Invalid input dimension";
  ++callsNumber_;
  return constant_;
}

/* Accessor for input point dimension */
UnsignedLong ConstantNumericalMathGradientImplementation::getInputDimension() const
{
  return constant_.getNbRows();
}

/* Accessor for output point dimension */
UnsignedLong ConstantNumericalMathGradientImplementation::getOutputDimension() const
{
  return constant_.getNbColumns();
}

/* Method save() stores the object through the StorageManager */
void ConstantNumericalMathGradientImplementation::save(Advocate & adv) const
{
  NumericalMathGradientImplementation::save(adv);
  adv.saveAttribute( "constant_", constant_ );
}

/* Method load() reloads the object from the StorageManager */
void ConstantNumericalMathGradientImplementation::load(Advocate & adv)
{
  NumericalMathGradientImplementation::load(adv);
  adv.loadAttribute( "constant_", constant_ );
}

END_NAMESPACE_OPENTURNS
