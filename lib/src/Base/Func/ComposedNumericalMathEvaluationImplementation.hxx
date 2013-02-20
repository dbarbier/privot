//                                               -*- C++ -*-
/**
 * @file  ComposedNumericalMathEvaluationImplementation.hxx
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
 * @author schueller
 * @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */

#ifndef OPENTURNS_COMPOSEDNUMERICALMATHFUNCTIONIMPLEMENTATION_HXX
#define OPENTURNS_COMPOSEDNUMERICALMATHFUNCTIONIMPLEMENTATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class ComposedNumericalMathEvaluationImplementation
 *
 * The class that implement the composition of two numerical math functions implementations.
 */
class ComposedNumericalMathEvaluationImplementation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:

  /* Some typedefs for easy reading */
  typedef Pointer<NumericalMathEvaluationImplementation>          EvaluationImplementation;

  /** Default constructor */
  ComposedNumericalMathEvaluationImplementation(const EvaluationImplementation & p_leftFunction,
                                                const EvaluationImplementation & p_rightFunction);


  /** Virtual constructor */
  virtual ComposedNumericalMathEvaluationImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const ComposedNumericalMathEvaluationImplementation & other) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Operator () */
  NumericalPoint operator() (const NumericalPoint & inP) const;

  NumericalSample operator() (const NumericalSample & inS) const;

  /** Parameters value and description accessor */
  virtual NumericalPointWithDescription getParameters() const;
  virtual void setParameters(const NumericalPointWithDescription & parameters);

  /** Accessor for input point dimension */
  UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  UnsignedLong getOutputDimension() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

  /** Composed implementation accessor */
  EvaluationImplementation getLeftEvaluationImplementation() const;
  EvaluationImplementation getRightEvaluationImplementation() const;

protected:

  ComposedNumericalMathEvaluationImplementation() {};
  friend class Factory<ComposedNumericalMathEvaluationImplementation>;

private:

  /** The function f in h = f o g */
  EvaluationImplementation p_leftFunction_;

  /** The function g in h = f o g */
  EvaluationImplementation p_rightFunction_;

}; /* class ComposedNumericalMathEvaluationImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COMPOSEDNUMERICALMATHFUNCTIONIMPLEMENTATION_HXX */
