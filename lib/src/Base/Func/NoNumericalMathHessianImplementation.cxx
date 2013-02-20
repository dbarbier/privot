//                                               -*- C++ -*-
/**
 *  @file  NoNumericalMathHessianImplementation.cxx
 *  @brief Abstract top-level class for all numerical math hessian implementations
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
#include "NoNumericalMathHessianImplementation.hxx"
#include "PersistentObjectFactory.hxx"


BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(NoNumericalMathHessianImplementation);

static Factory<NoNumericalMathHessianImplementation> RegisteredFactory("NoNumericalMathHessianImplementation");

/* Default constructor */
NoNumericalMathHessianImplementation::NoNumericalMathHessianImplementation()
  : NumericalMathHessianImplementation()
{
  // Nothing to do
}

/* Virtual constructor */
NoNumericalMathHessianImplementation * NoNumericalMathHessianImplementation::clone() const
{
  return new NoNumericalMathHessianImplementation(*this);
}

/* Comparison operator */
Bool NoNumericalMathHessianImplementation::operator ==(const NoNumericalMathHessianImplementation & other) const
{
  return true;
}

/* String converter */
String NoNumericalMathHessianImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << NoNumericalMathHessianImplementation::GetClassName()
      << " name=" << getName();
  return oss;
}

/* Test for actual implementation */
Bool NoNumericalMathHessianImplementation::isActualImplementation() const
{
  return false;
}









/* Here is the interface that all derived class must implement */

/* Hessian method */
SymmetricTensor NoNumericalMathHessianImplementation::hessian(const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inP.getDimension();
  ++callsNumber_;
  return SymmetricTensor();
}

/* Accessor for input point dimension */
UnsignedLong NoNumericalMathHessianImplementation::getInputDimension() const
{
  return 0;
}

/* Accessor for output point dimension */
UnsignedLong NoNumericalMathHessianImplementation::getOutputDimension() const
{
  return 0;
}

END_NAMESPACE_OPENTURNS
