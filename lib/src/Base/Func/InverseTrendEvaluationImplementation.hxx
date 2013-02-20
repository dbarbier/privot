//                                               -*- C++ -*-
/**
 * @file  InverseTrendEvaluationImplementation.hxx
 * @brief Class for Box Cox function implementation
 *
 * Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 *
 * @author schueller
 * @date   2012-01-04 12:26:21 +0100 (mer. 04 janv. 2012)
 */

#ifndef OPENTURNS_INVERSETRENDEVALUATIONIMPLEMENTATION_HXX
#define OPENTURNS_INVERSETRENDEVALUATIONIMPLEMENTATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"
#include "NumericalMathFunction.hxx"
#include "NumericalPoint.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class InverseTrendEvaluationImplementation
 *
 * This class offers an easy evaluation of the Box Cox function defined by :
 *  h(x) = \frac{x^\lambda - 1}{\lambda} for \lambda non zero, log(x) oherwise
 *  Care that x should be positive
 */

class InverseTrendEvaluationImplementation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  InverseTrendEvaluationImplementation();

  /** Parameter constructor */
  InverseTrendEvaluationImplementation(const NumericalMathFunction & function);

  /** Virtual constructor */
  InverseTrendEvaluationImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const InverseTrendEvaluationImplementation & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Operator () */
  using NumericalMathEvaluationImplementation::operator();
  NumericalPoint operator() (const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  UnsignedLong getOutputDimension() const;

  /** Accessor for the 1D function */
  NumericalMathFunction getFunction() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  /** The 1D function underlying the InverseTrend transform */
  NumericalMathFunction function_;


}; /* class InverseTrendEvaluationImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_INVERSETRENDEVALUATIONIMPLEMENTATION_HXX */
