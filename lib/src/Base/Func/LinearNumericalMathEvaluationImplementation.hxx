//                                               -*- C++ -*-
/**
 * @file  LinearNumericalMathEvaluationImplementation.hxx
 * @brief Class for a linear numerical math function implementation
 *        of the form y = constant + <linear, x - c> where c is a
 *        dim(x) numerical point, linear a dim(x) by dim(y) matrix
 *        and  <linear, x - c> means Transpose(linear).(x - c)
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

#ifndef OPENTURNS_LINEARNUMERICALMATHFUNCTIONIMPLEMENTATION_HXX
#define OPENTURNS_LINEARNUMERICALMATHFUNCTIONIMPLEMENTATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"
#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"
#include "Matrix.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class LinearNumericalMathEvaluationImplementation
 *
 * This class offers an interface for an real numerical mathematical function
 * implementations into the platform. It realizes the computation of the image vector
 * of a vector (aka NumericalPoint) through the function. The
 * function may be loaded in an external way through a wrapper.
 */
class LinearNumericalMathEvaluationImplementation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  LinearNumericalMathEvaluationImplementation();

  /** Parameter constructor */
  LinearNumericalMathEvaluationImplementation(const NumericalPoint & center,
                                              const NumericalPoint & constant,
                                              const Matrix & linear);

  /** Virtual constructor */
  virtual LinearNumericalMathEvaluationImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const LinearNumericalMathEvaluationImplementation & other) const;

  /** String converter */
  virtual String __repr__() const;



  /* Here is the interface that all derived class must implement */

  /** Operator () */
  virtual NumericalPoint operator() (const NumericalPoint & inP) const;

  /** Operator () */
  virtual NumericalSample operator() (const NumericalSample & inS) const;

  /** Accessor for input point dimension */
  virtual UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  virtual UnsignedLong getOutputDimension() const;

  /** Accessor for the center */
  NumericalPoint getCenter() const;

  /** Accessor for the constant term */
  NumericalPoint getConstant() const;

  /** Accessor for the linear term */
  Matrix getLinear() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:


private:
  NumericalPoint center_;
  NumericalPoint constant_;
  Matrix linear_;
}; /* class LinearNumericalMathEvaluationImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARNUMERICALMATHFUNCTIONIMPLEMENTATION_HXX */
