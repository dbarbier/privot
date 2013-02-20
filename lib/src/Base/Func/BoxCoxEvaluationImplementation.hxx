//                                               -*- C++ -*-
/**
 * @file  BoxCoxEvaluationImplementation.hxx
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
 * @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */

#ifndef OPENTURNS_BOXCOXEVALUATIONIMPLEMENTATION_HXX
#define OPENTURNS_BOXCOXEVALUATIONIMPLEMENTATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"
#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class BoxCoxEvaluationImplementation
 *
 * This class offers an easy evaluation of the Box Cox function defined by :
 *  h(x) = \frac{x^\lambda - 1}{\lambda} for \lambda non zero, log(x) oherwise
 *  Care that x should be positive
 */

class BoxCoxEvaluationImplementation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  BoxCoxEvaluationImplementation();

  /** Parameter constructor */
  BoxCoxEvaluationImplementation(const NumericalPoint & lamda);

  /** Virtual constructor */
  BoxCoxEvaluationImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const BoxCoxEvaluationImplementation & other) const;

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

  /** Accessor for the lambda point */
  NumericalPoint getLambda() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  /** lambda vector of the box cox transform */
  NumericalPoint lambda_;


}; /* class BoxCoxEvaluationImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BOXCOXEVALUATIONIMPLEMENTATION_HXX */
