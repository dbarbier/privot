//                                               -*- C++ -*-
/**
 *  @file  ComposedNumericalMathFunction.cxx
 *  @brief Abstract top-level class for all distributions
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
#include "ComposedNumericalMathFunction.hxx"
#include "NoNumericalMathGradientImplementation.hxx"
#include "NoNumericalMathHessianImplementation.hxx"
#include "ComposedNumericalMathEvaluationImplementation.hxx"
#include "ComposedNumericalMathGradientImplementation.hxx"
#include "ComposedNumericalMathHessianImplementation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(ComposedNumericalMathFunction);

static Factory<ComposedNumericalMathFunction> RegisteredFactory("ComposedNumericalMathFunction");

/* Default constructor */
ComposedNumericalMathFunction::ComposedNumericalMathFunction()
  : NumericalMathFunctionImplementation(),
    p_leftFunction_(),
    p_rightFunction_()
{
  // Nothing to do
}

/* Composition constructor */
ComposedNumericalMathFunction::ComposedNumericalMathFunction(const Implementation & p_left,
                                                             const Implementation & p_right)
  : NumericalMathFunctionImplementation(new ComposedNumericalMathEvaluationImplementation(p_left->getEvaluationImplementation(), p_right->getEvaluationImplementation()),
                                        new NoNumericalMathGradientImplementation(),
                                        new NoNumericalMathHessianImplementation()),
    p_leftFunction_(p_left),
    p_rightFunction_(p_right)
{
  try
    {
      GradientImplementation p_gradientImplementation(new ComposedNumericalMathGradientImplementation(p_left->getGradientImplementation(), p_right->getEvaluationImplementation(), p_right->getGradientImplementation()));
      setInitialGradientImplementation(p_gradientImplementation);
      setGradientImplementation(p_gradientImplementation);
      setUseDefaultGradientImplementation(p_left->getUseDefaultGradientImplementation() || p_right->getUseDefaultGradientImplementation());
    }
  catch(InvalidArgumentException & ex)
    {
      // Nothing to do
    }
  try
    {
      HessianImplementation p_hessianImplementation(new ComposedNumericalMathHessianImplementation(p_left->getGradientImplementation(), p_left->getHessianImplementation(), p_right->getEvaluationImplementation(), p_right->getGradientImplementation(), p_right->getHessianImplementation()));
      setInitialHessianImplementation(p_hessianImplementation);
      setHessianImplementation(p_hessianImplementation);
      setUseDefaultGradientImplementation(p_left->getUseDefaultHessianImplementation() || p_right->getUseDefaultHessianImplementation());
    }
  catch(InvalidArgumentException & ex)
    {
      // Nothing to do
    }
}

/* Composition constructor */
ComposedNumericalMathFunction::ComposedNumericalMathFunction(const NumericalMathFunction & left,
                                                             const NumericalMathFunction & right)
  : NumericalMathFunctionImplementation(new ComposedNumericalMathEvaluationImplementation(left.getEvaluationImplementation(), right.getEvaluationImplementation()),
                                        new NoNumericalMathGradientImplementation(),
                                        new NoNumericalMathHessianImplementation()),
    p_leftFunction_(left.getImplementation()),
    p_rightFunction_(right.getImplementation())
{
  try
    {
      GradientImplementation p_gradientImplementation(new ComposedNumericalMathGradientImplementation(left.getGradientImplementation(), right.getEvaluationImplementation(), right.getGradientImplementation()));
      setInitialGradientImplementation(p_gradientImplementation);
      setGradientImplementation(p_gradientImplementation);
      setUseDefaultGradientImplementation(left.getUseDefaultGradientImplementation() || right.getUseDefaultGradientImplementation());
    }
  catch(InvalidArgumentException & ex)
    {
      // Nothing to do
    }
  try
    {
      HessianImplementation p_hessianImplementation(new ComposedNumericalMathHessianImplementation(left.getGradientImplementation(), left.getHessianImplementation(), right.getEvaluationImplementation(), right.getGradientImplementation(), right.getHessianImplementation()));
      setInitialHessianImplementation(p_hessianImplementation);
      setHessianImplementation(p_hessianImplementation);
      setUseDefaultGradientImplementation(left.getUseDefaultHessianImplementation() || right.getUseDefaultHessianImplementation());
    }
  catch(InvalidArgumentException & ex)
    {
      // Nothing to do
    }
}

/* Virtual constructor */
ComposedNumericalMathFunction * ComposedNumericalMathFunction::clone() const
{
  return new ComposedNumericalMathFunction(*this);
}

/* Comparison operator */
Bool ComposedNumericalMathFunction::operator ==(const ComposedNumericalMathFunction & other) const
{
  return true;
}

/* String converter */
String ComposedNumericalMathFunction::__repr__() const
{
  OSS oss;
  oss << "class=" << ComposedNumericalMathFunction::GetClassName()
      << " name=" << getName()
      << " description=" << getDescription()
      << " left function=" << p_leftFunction_->__repr__()
      << " right function=" << p_rightFunction_->__repr__();
  return oss;
}

/*
 * Gradient according to the marginal parameters
 *
 * F : RkxRs -> Rn
 *    (y, pf) -> F(y, pf)
 *
 * G : RmxRt -> Rk
 *    (x, pg) -> G(x, pg)
 *
 * Let p = [pg, pf] be the parameter vector of H, with:
 *
 * H : RmxRt+s -> Rn
 *     (x, p)  -> F(G(x, pg), pf)
 *
 * We have:
 *
 * (dH/dp)(x, p) = [(dF/dy)(G(x, pg), pf) . (dG/dpg)(x, pg), 0] + [0, (dF/dpf)(G(x, pg), pf)]
 *
 * the needed gradient is [(dH/dp)(x,p)]^t
 */
Matrix ComposedNumericalMathFunction::parametersGradient(const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inP.getDimension();
  // y = G(x, pg)
  const NumericalPoint y(p_rightFunction_->operator()(inP));
  // (dG/dpg)(x, pg)
  const Matrix rightGradientP(p_rightFunction_->parametersGradient(inP));
  // (dF/dy)(y, pf)
  const Matrix leftGradientY(p_leftFunction_->gradient(y));
  // (dF/dpf)(G(x, pg), pf)
  const Matrix leftGradientP(p_leftFunction_->parametersGradient(y));
  // (dF/dy)(G(x, pg), pf) . (dG/dpg)(x, pg)
  const Matrix upper(rightGradientP * leftGradientY);
  // Build the full gradient
  const UnsignedLong rightParametersDimension(upper.getNbRows());
  const UnsignedLong leftParametersDimension(leftGradientP.getNbRows());
  Matrix gradient(rightParametersDimension + leftParametersDimension, inputDimension);
  UnsignedLong rowIndex(0);
  // Gradient according to left parameters
  for (UnsignedLong i = 0; i < rightParametersDimension; ++i)
    {
      for (UnsignedLong j = 0; j < inputDimension; ++j)
        gradient(rowIndex, j) = upper(i, j);
      ++rowIndex;
    }
  // Gradient according to right parameters
  for (UnsignedLong i = 0; i < leftParametersDimension; ++i)
    {
      for (UnsignedLong j = 0; j < inputDimension; ++j)
        gradient(rowIndex, j) = leftGradientP(i, j);
      ++rowIndex;
    }
  return gradient;
}

/* Method save() stores the object through the StorageManager */
void ComposedNumericalMathFunction::save(Advocate & adv) const
{
  NumericalMathFunctionImplementation::save(adv);
  adv.saveAttribute( "leftFunction_", *p_leftFunction_ );
  adv.saveAttribute( "rightFunction_", *p_rightFunction_ );
}

/* Method load() reloads the object from the StorageManager */
void ComposedNumericalMathFunction::load(Advocate & adv)
{
  TypedInterfaceObject<NumericalMathFunctionImplementation> functionValue;
  NumericalMathFunctionImplementation::load(adv);
  adv.loadAttribute( "leftFunction_", functionValue );
  p_leftFunction_ = functionValue.getImplementation();
  adv.loadAttribute( "rightFunction_", functionValue );
  p_rightFunction_ = functionValue.getImplementation();
}

END_NAMESPACE_OPENTURNS
