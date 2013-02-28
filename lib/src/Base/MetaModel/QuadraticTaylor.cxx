//                                               -*- C++ -*-
/**
 *  @file  QuadraticTaylor.cxx
 *  @brief Second order polynomial response surface by Taylor expansion
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
#include "QuadraticTaylor.hxx"
#include "QuadraticNumericalMathEvaluationImplementation.hxx"
#include "LinearNumericalMathGradientImplementation.hxx"
#include "ConstantNumericalMathHessianImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(QuadraticTaylor);

QuadraticTaylor::QuadraticTaylor(const String & name)
  : PersistentObject(name)
{
  // Nothing to do
}

/* Constructor with parameters */
QuadraticTaylor::QuadraticTaylor(const NumericalPoint & center,
                                 const NumericalMathFunction & inputFunction,
                                 const String & name)
  : PersistentObject(name),
    center_(center),
    inputFunction_(inputFunction)
{
  // Nothing to do
}

/* Virtual constructor */
QuadraticTaylor * QuadraticTaylor::clone() const
{
  return new QuadraticTaylor(*this);
}

/* String converter */
String QuadraticTaylor::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName()
      << " center=" << center_
      << " function=" << inputFunction_
      << " responseSurface=" << responseSurface_
      << " constant=" << constant_
      << " linear=" << linear_
      << " quadratic=" << quadratic_;
  return oss;
}

/* Response surface computation */
void QuadraticTaylor::run()
{
  /* Compute the three first terms of the Taylor expansion */
  constant_ = inputFunction_(center_);
  linear_ = inputFunction_.gradient(center_);
  quadratic_ = inputFunction_.hessian(center_);
  /* Build the several implementations and set it into the response surface */
  responseSurface_.setEvaluationImplementation(new QuadraticNumericalMathEvaluationImplementation(center_, constant_, linear_, quadratic_));
  responseSurface_.setGradientImplementation(new LinearNumericalMathGradientImplementation(center_, linear_, quadratic_));
  responseSurface_.setHessianImplementation(new ConstantNumericalMathHessianImplementation(quadratic_));
  responseSurface_.setDescription(inputFunction_.getDescription());
}

/* Center accessor */
NumericalPoint QuadraticTaylor::getCenter() const
{
  return center_;
}

/* Constant accessor */
NumericalPoint QuadraticTaylor::getConstant() const
{
  return constant_;
}

/* Linear accessor */
Matrix QuadraticTaylor::getLinear() const
{
  return linear_;
}

/* Quadratic accessor */
SymmetricTensor QuadraticTaylor::getQuadratic() const
{
  return quadratic_;
}

/* Function accessor */
NumericalMathFunction QuadraticTaylor::getInputFunction() const
{
  return inputFunction_;
}

/* Response surface accessor */
NumericalMathFunction QuadraticTaylor::getResponseSurface() const
{
  return responseSurface_;
}

END_NAMESPACE_OPENTURNS