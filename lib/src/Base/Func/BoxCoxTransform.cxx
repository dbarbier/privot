//                                               -*- C++ -*-
/**
 *  @file  BoxCoxTransform.cxx
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
#include "BoxCoxTransform.hxx"
#include "BoxCoxEvaluationImplementation.hxx"
#include "InverseBoxCoxTransform.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(BoxCoxTransform);

/* Default constructor */
BoxCoxTransform::BoxCoxTransform()
  : NumericalMathFunction(BoxCoxEvaluationImplementation())
{
  // Nothing to do
}

/* Standard parameter constructor */
BoxCoxTransform::BoxCoxTransform(const NumericalPoint & lambda)
  : NumericalMathFunction(BoxCoxEvaluationImplementation(lambda))
{
  // Nothing to do
}

/* NumericalScalarCollection parameter constructor */
BoxCoxTransform::BoxCoxTransform(const NumericalPoint & lambda,
				 const NumericalPoint & shift)
  : NumericalMathFunction(BoxCoxEvaluationImplementation(lambda, shift))
{
  // Nothing to do
}

/* 1D NumericalScalar parameter constructor */
BoxCoxTransform::BoxCoxTransform(const NumericalScalar & lambda)
  : NumericalMathFunction(BoxCoxEvaluationImplementation(NumericalPoint(1, lambda)))
{
  // Nothing to do
}

BoxCoxTransform::BoxCoxTransform(const NumericalScalar & lambda,
				 const NumericalScalar & shift)
  : NumericalMathFunction(BoxCoxEvaluationImplementation(NumericalPoint(1, lambda), NumericalPoint(1, lambda)))
{
  // Nothing to do
}

/* Virtual constructor */
BoxCoxTransform * BoxCoxTransform::clone() const
{
  return new BoxCoxTransform(*this);
}

/* Lambda accessor */
NumericalPoint BoxCoxTransform::getLambda() const
{
  return dynamic_cast< BoxCoxEvaluationImplementation* >(getEvaluationImplementation().get())->getLambda();
}

/* Shift accessor */
NumericalPoint BoxCoxTransform::getShift() const
{
  return dynamic_cast< BoxCoxEvaluationImplementation* >(getEvaluationImplementation().get())->getShift();
}

/* Inverse accessor */
InverseBoxCoxTransform BoxCoxTransform::getInverse() const
{
  return InverseBoxCoxTransform(getLambda(), getShift());
}

END_NAMESPACE_OPENTURNS
