//                                               -*- C++ -*-
/**
 *  @file  ProductNumericalMathHessianImplementation.hxx
 *  @brief The class that implements the composition between numerical math hessians
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
 *  @author: $LastChangedBy: dutka $
 *  @date:   $LastChangedDate: 2007-05-10 16:43:31 +0200 (jeu, 10 mai 2007) $
 *  Id:      $Id: ProductNumericalMathHessianImplementation.hxx 434 2007-05-10 14:43:31Z dutka $
 */
#ifndef OPENTURNS_PRODUCTNUMERICALMATHHESSIANIMPLEMENTATION_HXX
#define OPENTURNS_PRODUCTNUMERICALMATHHESSIANIMPLEMENTATION_HXX

#include "NumericalMathHessianImplementation.hxx"
#include "NumericalMathGradientImplementation.hxx"
#include "NumericalMathEvaluationImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class ProductNumericalMathHessianImplementation
 *
 * The class that implement the composition of two numerical math hessians
 */
class ProductNumericalMathHessianImplementation
  : public NumericalMathHessianImplementation
{
  CLASSNAME;
public:

  /* Some typedefs for easy reading */
  typedef Pointer<NumericalMathHessianImplementation>          HessianImplementation;
  typedef Pointer<NumericalMathGradientImplementation>         GradientImplementation;
  typedef Pointer<NumericalMathEvaluationImplementation>       EvaluationImplementation;

  /** Default constructor */
  ProductNumericalMathHessianImplementation(const EvaluationImplementation & p_leftEvaluation,
                                            const GradientImplementation & p_leftGradient,
                                            const HessianImplementation & p_leftHessian,
                                            const EvaluationImplementation & p_rightEvaluation,
                                            const GradientImplementation & p_rightGradient,
                                            const HessianImplementation & p_rightHessian);

  /** Virtual constructor */
  virtual ProductNumericalMathHessianImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const ProductNumericalMathHessianImplementation & other) const;

  /** String converter */
  virtual String __repr__() const;

  /** Method hessian() returns the symmetric tensor of the function at point */
  SymmetricTensor hessian(const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  UnsignedLong getOutputDimension() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  ProductNumericalMathHessianImplementation() {};
  friend class Factory<ProductNumericalMathHessianImplementation>;
private:

  /** The function of f in h = f . g */
  EvaluationImplementation p_leftEvaluation_;

  /** The gradient of f in h = f . g */
  GradientImplementation p_leftGradient_;

  /** The hessian of f in h = f . g */
  HessianImplementation p_leftHessian_;

  /** The function g in h = f . g */
  EvaluationImplementation p_rightEvaluation_;

  /** The gradient of g in h = f . g */
  GradientImplementation p_rightGradient_;

  /** The hessian of g in h = f . g */
  HessianImplementation p_rightHessian_;

}; /* class ProductNumericalMathHessianImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PRODUCTNUMERICALMATHHESSIANIMPLEMENTATION_HXX */
