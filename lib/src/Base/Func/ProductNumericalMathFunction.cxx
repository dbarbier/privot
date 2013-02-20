//                                               -*- C++ -*-
/**
 *  @file  ProductNumericalMathFunction.cxx
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
 *  @author: $LastChangedBy: lebrun $
 *  @date:   $LastChangedDate: 2008-07-03 08:31:34 +0200 (jeu, 03 jui 2008) $
 *  Id:      $Id: ProductNumericalMathFunction.cxx 867 2008-07-03 06:31:34Z lebrun $
 */
#include "ProductNumericalMathFunction.hxx"
#include "NoNumericalMathGradientImplementation.hxx"
#include "NoNumericalMathHessianImplementation.hxx"
#include "ProductNumericalMathEvaluationImplementation.hxx"
#include "ProductNumericalMathGradientImplementation.hxx"
#include "ProductNumericalMathHessianImplementation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ProductNumericalMathFunction);

static Factory<ProductNumericalMathFunction> RegisteredFactory("ProductNumericalMathFunction");

/* Composition constructor */
ProductNumericalMathFunction::ProductNumericalMathFunction(const Implementation & p_left,
                                                           const Implementation & p_right)
  : NumericalMathFunctionImplementation(new ProductNumericalMathEvaluationImplementation(p_left->getEvaluationImplementation(), p_right->getEvaluationImplementation()),
                                        new NoNumericalMathGradientImplementation(),
                                        new NoNumericalMathHessianImplementation()),
    p_leftFunction_(p_left),
    p_rightFunction_(p_right)
{
  //  try{
  GradientImplementation p_gradientImplementation(new ProductNumericalMathGradientImplementation(p_leftFunction_->getEvaluationImplementation(), p_leftFunction_->getGradientImplementation(), p_rightFunction_->getEvaluationImplementation(), p_rightFunction_->getGradientImplementation()));
  setInitialGradientImplementation(p_gradientImplementation);
  setGradientImplementation(p_gradientImplementation);
  //  }
  //  catch(InvalidArgumentException & ex) {
  // Nothing to do
  //  }
  //  try{
  HessianImplementation p_hessianImplementation(new ProductNumericalMathHessianImplementation(p_left->getEvaluationImplementation(), p_left->getGradientImplementation(), p_left->getHessianImplementation(), p_right->getEvaluationImplementation(), p_right->getGradientImplementation(), p_right->getHessianImplementation()));
  setInitialHessianImplementation(p_hessianImplementation);
  setHessianImplementation(p_hessianImplementation);
  //  }
  //  catch(InvalidArgumentException & ex) {
  // Nothing to do
  //  }
}

/* Virtual constructor */
ProductNumericalMathFunction * ProductNumericalMathFunction::clone() const
{
  return new ProductNumericalMathFunction(*this);
}

/* Comparison operator */
Bool ProductNumericalMathFunction::operator ==(const ProductNumericalMathFunction & other) const
{
  return true;
}

/* String converter */
String ProductNumericalMathFunction::__repr__() const
{
  OSS oss;
  oss << "class=" << ProductNumericalMathFunction::GetClassName()
      << " name=" << getName()
      << " description=" << getDescription()
      << " left function=" << p_leftFunction_->__repr__()
      << " right function=" << p_rightFunction_->__repr__();
  return oss;
}

String ProductNumericalMathFunction::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class=" << ProductNumericalMathFunction::GetClassName()
      << " name=" << getName()
      << " description=" << getDescription()
      << " left function=" << p_leftFunction_->__repr__()
      << " right function=" << p_rightFunction_->__repr__();
  return oss;
}

/*
 * Gradient according to the marginal parameters
 * H(x, p) = F(x, pf) . G(x, pg)
 * dH/dp = dF/dp(x, pf) . G(x, pg) + F(x, pf) . dG/dp(x, pg)
 * with
 * p = [pf, pg], dF/dp = [dF/dpf, 0], dG/dp = [0, dG/dpg]
 * thus
 * dH/dp = [dF/dpf(x, pf) . G(x, pg), dG/dpg(x, pg) . F(x, pf)]
 * and the needed gradient is (dH/dp)^t
 */
Matrix ProductNumericalMathFunction::parametersGradient(const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inP.getDimension();
  // Values of the functions
  NumericalScalar leftValue(p_leftFunction_->operator()(inP)[0]);
  NumericalScalar rightValue(p_rightFunction_->operator()(inP)[0]);
  // Parameters gradient of the functions scaled by the value of there product term
  Matrix upper(p_leftFunction_->parametersGradient(inP) * leftValue);
  Matrix lower(p_rightFunction_->parametersGradient(inP) * rightValue);
  // Fill-in the result
  UnsignedLong leftParametersDimension(upper.getNbRows());
  UnsignedLong rightParametersDimension(lower.getNbRows());
  Matrix gradient(rightParametersDimension + leftParametersDimension, 1);
  UnsignedLong rowIndex(0);
  // Gradient according to left parameters
  for (UnsignedLong i = 0; i < leftParametersDimension; ++i)
    {
      gradient(rowIndex, 0) = upper(i, 0);
      ++rowIndex;
    }
  // Gradient accroding to right parameters
  for (UnsignedLong i = 0; i < leftParametersDimension; ++i)
    {
      gradient(rowIndex, 0) = lower(i, 0);
      ++rowIndex;
    }
  return gradient;
}

/* Method save() stores the object through the StorageManager */
void ProductNumericalMathFunction::save(Advocate & adv) const
{
  NumericalMathFunctionImplementation::save(adv);
  adv.saveAttribute( "leftFunction_", *p_leftFunction_ );
  adv.saveAttribute( "rightFunction_", *p_rightFunction_ );
}

/* Method load() reloads the object from the StorageManager */
void ProductNumericalMathFunction::load(Advocate & adv)
{
  TypedInterfaceObject<NumericalMathFunctionImplementation> functionValue;
  NumericalMathFunctionImplementation::load(adv);
  adv.loadAttribute( "leftFunction_", functionValue );
  p_leftFunction_ = functionValue.getImplementation();
  adv.loadAttribute( "rightFunction_", functionValue );
  p_rightFunction_ = functionValue.getImplementation();
}

END_NAMESPACE_OPENTURNS
