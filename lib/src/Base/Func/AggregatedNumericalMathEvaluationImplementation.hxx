//                                               -*- C++ -*-
/**
 *  @file  AggregatedNumericalMathEvaluationImplementation.hxx
 *  @brief The evaluation part of an aggregation of functions from R^n to R^p_1,...,R^n to R^p_k
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
#ifndef OPENTURNS_AGGREGATEDNUMERICALMATHEVALUATIONIMPLEMENTATION_HXX
#define OPENTURNS_AGGREGATEDNUMERICALMATHEVALUATIONIMPLEMENTATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"
#include "Collection.hxx"
#include "PersistentCollection.hxx"
#include "NumericalMathFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class AggregatedNumericalMathEvaluationImplementation
 *
 * The evaluation part of an aggregation of functions from R^n to R^p_1,...,R^n to R^p_k
 */

class AggregatedNumericalMathEvaluationImplementation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:

  typedef Collection<NumericalMathFunction>                              NumericalMathFunctionCollection;
  typedef PersistentCollection<NumericalMathFunction>                    NumericalMathFunctionPersistentCollection;

  // friend class Factory<AggregatedNumericalMathEvaluationImplementation>;

  /** Default constructor */
  AggregatedNumericalMathEvaluationImplementation();

  /** Parameter constructor */
  AggregatedNumericalMathEvaluationImplementation(const NumericalMathFunctionCollection & functionsCollection);

  /** Virtual constructor */
  virtual AggregatedNumericalMathEvaluationImplementation * clone() const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Evaluation operator */
  NumericalPoint operator () (const NumericalPoint & inP) const;
  NumericalSample operator () (const NumericalSample & inS) const;

  /** Functions accessor */
  NumericalMathFunctionCollection getFunctionsCollection() const;
  void setFunctionsCollection(const NumericalMathFunctionCollection & functionsCollection);

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
  friend class AggregatedGradientImplementation;
  friend class AggregatedHessianImplementation;

  // The functions to be aggregated
  NumericalMathFunctionPersistentCollection functionsCollection_;

  // The output dimension
  UnsignedLong outputDimension_;

} ; /* class AggregatedNumericalMathEvaluationImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_AGGREGATEDNUMERICALMATHEVALUATIONIMPLEMENTATION_HXX */
