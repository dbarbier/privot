//                                               -*- C++ -*-
/**
 *  @file  DualLinearCombinationEvaluationImplementation.hxx
 *  @brief The evaluation part of functional linear combination of vectors
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
 *  @date   2008-05-21 17:44:02 +0200 (Wed, 21 May 2008)
 *  Id      Object.hxx 818 2008-05-21 15:44:02Z dutka
 */
#ifndef OPENTURNS_DUALLINEARCOMBINATIONEVALUATIONIMPLEMENTATION_HXX
#define OPENTURNS_DUALLINEARCOMBINATIONEVALUATIONIMPLEMENTATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"
#include "Collection.hxx"
#include "PersistentCollection.hxx"
#include "NumericalMathFunction.hxx"
#include "NumericalSample.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class DualLinearCombinationEvaluationImplementation
 *
 * The evaluation part of functional linear combination of vectors
 */

class DualLinearCombinationEvaluationImplementation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:

  typedef Collection<NumericalMathFunction>                              NumericalMathFunctionCollection;
  typedef PersistentCollection<NumericalMathFunction>                    NumericalMathFunctionPersistentCollection;

  // friend class Factory<DualLinearCombinationEvaluationImplementation>;

  /** Default constructor */
  DualLinearCombinationEvaluationImplementation();

public:
  /** Parameter constructor */
  DualLinearCombinationEvaluationImplementation(const NumericalMathFunctionCollection & functionsCollection,
                                                const NumericalSample & coefficients);

  /** Virtual constructor */
  virtual DualLinearCombinationEvaluationImplementation * clone() const;

  /** Description accessor */
  void setDescription(const Description & description);

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Evaluation operator */
  NumericalPoint operator () (const NumericalPoint & inP) const;

  /** Coefficients accessor */
  NumericalSample getCoefficients() const;

  /** Functions accessor */
  NumericalMathFunctionCollection getFunctionsCollection() const;
  void setFunctionsCollectionAndCoefficients(const NumericalMathFunctionCollection & functionsCollection,
                                             const NumericalSample & coefficients);

  /** Input dimension accessor */
  UnsignedLong getInputDimension() const;

  /** Output dimension accessor */
  UnsignedLong getOutputDimension() const;

  /** Gradient according to the marginal parameters */
  Matrix parametersGradient(const NumericalPoint & inP) const;

  /** Parameters value and description accessor */
  NumericalPointWithDescription getParameters() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);


protected:

private:
  // Make the gradient and the hessian friend classes of the evaluation in order to share the functions and the coefficients
  friend class DualLinearCombinationGradientImplementation;
  friend class DualLinearCombinationHessianImplementation;

  // The functional coefficients of the combination
  NumericalMathFunctionPersistentCollection functionsCollection_;

  // The vectors of the combination
  NumericalSample coefficients_;
} ; /* class DualLinearCombinationEvaluationImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DUALLINEARCOMBINATIONEVALUATIONIMPLEMENTATION_HXX */
