//                                               -*- C++ -*-
/**
 * @file  AnalyticalNumericalMathGradientImplementation.hxx
 * @brief The class that implements the gradient of an analytical function.
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
 * @date   2010-05-31 07:26:50 +0200 (lun. 31 mai 2010)
 */

#ifndef OPENTURNS_ANALYTICALNUMERICALMATHGRADIENTIMPLEMENTATION_HXX
#define OPENTURNS_ANALYTICALNUMERICALMATHGRADIENTIMPLEMENTATION_HXX

#include "NumericalMathGradientImplementation.hxx"
#include "AnalyticalNumericalMathEvaluationImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class AnalyticalNumericalMathGradientImplementation
 *
 * The class that implement the gradient of an analytical function.
 */
class AnalyticalNumericalMathGradientImplementation
  : public NumericalMathGradientImplementation
{
  CLASSNAME;
public:

  /* Some typedefs for easy reading */
  typedef AnalyticalNumericalMathEvaluationImplementation::Parser Parser;
  typedef Collection<Parser>          ParserCollection;
  typedef Collection<NumericalScalar> NumericalScalarCollection;

  /** Default constructor */
  AnalyticalNumericalMathGradientImplementation();

  /** Default constructor */
  AnalyticalNumericalMathGradientImplementation(const AnalyticalNumericalMathEvaluationImplementation & evaluation);

  /** Virtual constructor */
  virtual AnalyticalNumericalMathGradientImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const AnalyticalNumericalMathGradientImplementation & other) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Operator () */
  using NumericalMathGradientImplementation::gradient;
  Matrix gradient(const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  UnsignedLong getOutputDimension() const;

  /** Accessor to a specific formula */
  String getFormula(const UnsignedLong i,
                    const UnsignedLong j) const;

  /** Get the i-th marginal function */
  Implementation getMarginal(const UnsignedLong i) const;

  /** Get the function corresponding to indices components */
  Implementation getMarginal(const Indices & indices) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

private:

  /* Must initialize the parser at the first call to operator() as the
     reference associated with the variables may have change after the construction */
  void initialize() const;

  mutable Bool isInitialized_;
  mutable Bool isAnalytical_;
  mutable NumericalScalarCollection inputVariables_;
  AnalyticalNumericalMathEvaluationImplementation evaluation_;

  /** A mathematical expression parser from the muParser library */
  mutable ParserCollection parsers_;

}; /* class AnalyticalNumericalMathGradientImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ANALYTICALNUMERICALMATHGRADIENTIMPLEMENTATION_HXX */
