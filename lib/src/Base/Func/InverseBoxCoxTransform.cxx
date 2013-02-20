//                                               -*- C++ -*-
/**
 *  @file  InverseBoxCoxTransform.cxx
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
#include "InverseBoxCoxTransform.hxx"
#include "InverseBoxCoxEvaluationImplementation.hxx"
#include "BoxCoxTransform.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(InverseBoxCoxTransform);

/* Default constructor */
InverseBoxCoxTransform::InverseBoxCoxTransform()
  : NumericalMathFunction(InverseBoxCoxEvaluationImplementation())
{
  // Nothing to do
}

/* Standard parameter constructor */
InverseBoxCoxTransform::InverseBoxCoxTransform(const NumericalPoint & lambda)
  : NumericalMathFunction(InverseBoxCoxEvaluationImplementation(lambda))
{
  // Nothing to do
}

/* NumericalScalarCollection parameter constructor */
InverseBoxCoxTransform::InverseBoxCoxTransform(const NumericalPoint & lambda,
                                               const NumericalPoint & shift)
  : NumericalMathFunction(InverseBoxCoxEvaluationImplementation(lambda, shift))
{
  // Nothing to do
}

/* 1D NumericalScalar parameter constructor */
InverseBoxCoxTransform::InverseBoxCoxTransform(const NumericalScalar & lambda)
  : NumericalMathFunction(InverseBoxCoxEvaluationImplementation(NumericalPoint(1, lambda)))
{
  // Nothing to do
}

InverseBoxCoxTransform::InverseBoxCoxTransform(const NumericalScalar & lambda,
                                               const NumericalScalar & shift)
  : NumericalMathFunction(InverseBoxCoxEvaluationImplementation(NumericalPoint(1, lambda), NumericalPoint(1, lambda)))
{
  // Nothing to do
}

/* Virtual constructor */
InverseBoxCoxTransform * InverseBoxCoxTransform::clone() const
{
  return new InverseBoxCoxTransform(*this);
}

/* Lambda accessor */
NumericalPoint InverseBoxCoxTransform::getLambda() const
{
  return dynamic_cast<InverseBoxCoxEvaluationImplementation *>(getEvaluationImplementation().get())->getLambda();
}

/* Shift accessor */
NumericalPoint InverseBoxCoxTransform::getShift() const
{
  return dynamic_cast<InverseBoxCoxEvaluationImplementation *>(getEvaluationImplementation().get())->getShift();
}

/* Inverse accessor */
BoxCoxTransform InverseBoxCoxTransform::getInverse() const
{
  return BoxCoxTransform(getLambda(), getShift());
}

END_NAMESPACE_OPENTURNS
