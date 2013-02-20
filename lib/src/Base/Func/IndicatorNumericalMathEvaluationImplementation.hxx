//                                               -*- C++ -*-
/**
 * @file  IndicatorNumericalMathEvaluationImplementation.hxx
 * @brief The class that implements the composition between numerical
 *        math functions implementations
 *
 * Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 *
 * @author lebrun
 * @date   2008-07-03 08:31:34 +0200 (jeu, 03 jui 2008)
 */

#ifndef OPENTURNS_INDICATORNUMERICALMATHFUNCTIONIMPLEMENTATION_HXX
#define OPENTURNS_INDICATORNUMERICALMATHFUNCTIONIMPLEMENTATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"
#include "ComparisonOperator.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class IndicatorNumericalMathEvaluationImplementation
 *
 * The class that implement the composition of two numerical math functions implementations.
 */
class IndicatorNumericalMathEvaluationImplementation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:

  /* Some typedefs for easy reading */
  typedef Pointer<NumericalMathEvaluationImplementation>           EvaluationImplementation;

  /** Default constructor */
  IndicatorNumericalMathEvaluationImplementation();

  /** Default constructor */
  IndicatorNumericalMathEvaluationImplementation(const EvaluationImplementation & p_evaluation,
                                                 const ComparisonOperator & comparisonOperator,
                                                 const NumericalScalar threshold);

  /** Virtual constructor */
  virtual IndicatorNumericalMathEvaluationImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const IndicatorNumericalMathEvaluationImplementation & other) const;

  /** String converter */
  virtual String __repr__() const;

  /** Operator () */
  using NumericalMathEvaluationImplementation::operator();
  NumericalPoint operator() (const NumericalPoint & inP) const;

  /** Accessor for the underlying evaluation */
  EvaluationImplementation getEvaluationImplementation() const;
  void setEvaluationImplementation(const EvaluationImplementation & p_evaluation);

  /** Accessor for the comparison operator */
  ComparisonOperator getComparisonOperator() const;
  void setComparisonOperator(const ComparisonOperator & comparisonOperator);

  /** Accessor for the threshold */
  NumericalScalar getThreshold() const;
  void setThreshold(const NumericalScalar threshold);

  /** Accessor for input point dimension */
  UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  UnsignedLong getOutputDimension() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  EvaluationImplementation p_evaluation_;
  ComparisonOperator comparisonOperator_;
  NumericalScalar threshold_;

}; /* class IndicatorNumericalMathEvaluationImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_INDICATORNUMERICALMATHFUNCTIONIMPLEMENTATION_HXX */
