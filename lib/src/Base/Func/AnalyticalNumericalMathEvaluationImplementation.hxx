//                                               -*- C++ -*-
/**
 * @file  AnalyticalNumericalMathEvaluationImplementation.hxx
 * @brief The class that implements the evaluation of an analytical function.
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

#ifndef OPENTURNS_ANALYTICALNUMERICALMATHEVALUATIONIMPLEMENTATION_HXX
#define OPENTURNS_ANALYTICALNUMERICALMATHEVALUATIONIMPLEMENTATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"
#include "AnalyticalParser.hxx"
#include "Pointer.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class AnalyticalNumericalMathEvaluationImplementation
 *
 * The class that implement the evaluation of an analytical function.
 */
class AnalyticalNumericalMathEvaluationImplementation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:

  /* Some typedefs for easy reading */
  typedef AnalyticalParser            Parser;
  typedef Collection<Parser>          ParserCollection;
  typedef Collection<NumericalScalar> NumericalScalarCollection;

  /** Default constructor */
  AnalyticalNumericalMathEvaluationImplementation();

  /** Default constructor */
  AnalyticalNumericalMathEvaluationImplementation(const Description & inputVariablesNames,
                                                  const Description & outputVariablesNames,
                                                  const Description & formulas);

  /** Virtual constructor */
  virtual AnalyticalNumericalMathEvaluationImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const AnalyticalNumericalMathEvaluationImplementation & other) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Operator () */
  using NumericalMathEvaluationImplementation::operator();
  NumericalPoint operator() (const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  UnsignedLong getOutputDimension() const;

  /** Get the i-th marginal function */
  Implementation getMarginal(const UnsignedLong i) const;

  /** Get the function corresponding to indices components */
  Implementation getMarginal(const Indices & indices) const;

  /** Accessor to the input variables names */
  Description getInputVariablesNames() const;

  /** Accessor to the output variables names */
  Description getOutputVariablesNames() const;

  /** Accessor to the formulas */
  Description getFormulas() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

private:
  friend class AnalyticalNumericalMathGradientImplementation;
  friend class AnalyticalNumericalMathHessianImplementation;
  /* Must initialize the parser at the first call to operator() as the
     reference associated with the variables may have change after the construction */
  void initialize() const;

  mutable Bool isInitialized_;
  mutable NumericalScalarCollection inputVariables_;
  Description inputVariablesNames_;
  Description outputVariablesNames_;
  Description formulas_;

  /** A mathematical expression parser from the muParser library */
  mutable ParserCollection parsers_;

}; /* class AnalyticalNumericalMathEvaluationImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ANALYTICALNUMERICALMATHEVALUATIONIMPLEMENTATION_HXX */
