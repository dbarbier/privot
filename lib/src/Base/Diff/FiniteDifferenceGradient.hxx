//                                               -*- C++ -*-
/**
 *  @file  FiniteDifferenceGradient.hxx
 *  @brief Class for the creation of a numerical math gradient implementation
 *         form a numerical math evaluation implementation by using centered
 *         finite difference formula.
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
 *  @author slama
 *  @date   2008-11-19 16:10:42 +0100 (mer, 19 nov 2008)
 *  Id      FiniteDifferenceGradient.hxx 1023 2008-11-19 15:10:42Z slama
 */
#ifndef OPENTURNS_FINITEDIFFERENCEGRADIENT_HXX
#define OPENTURNS_FINITEDIFFERENCEGRADIENT_HXX

#include "NumericalMathGradientImplementation.hxx"
#include "NumericalMathEvaluationImplementation.hxx"
#include "Pointer.hxx"
#include "FiniteDifferenceStep.hxx"


BEGIN_NAMESPACE_OPENTURNS




/**
 * @class FiniteDifferenceGradient
 *
 * This class is for the creation of a numerical math gradient implementation
 * form a numerical math evaluation implementation by using centered
 * finite difference formula
 */
class FiniteDifferenceGradient
  : public NumericalMathGradientImplementation
{
  CLASSNAME;
public:

  typedef Pointer<NumericalMathEvaluationImplementation>                          EvaluationImplementation;

  /** Default constructor */
  FiniteDifferenceGradient();

  /** First Parameter constructor  */
  FiniteDifferenceGradient(const NumericalPoint & epsilon,
                           const EvaluationImplementation & p_evaluation);

  /** SecondParameter constructor */
  FiniteDifferenceGradient(const NumericalScalar epsilon,
                           const EvaluationImplementation & p_evaluation);

  /** Constructor with FiniteDifferenceStep */
  FiniteDifferenceGradient(const FiniteDifferenceStep & finiteDifferenceStep,
                           const EvaluationImplementation & p_evaluation);

  /** Comparison operator */
  virtual Bool operator ==(const FiniteDifferenceGradient & other) const;

  /** String converter */
  virtual String  __repr__() const;

  /** Accessor for input point dimension
   * @return The size of the point passed to the gradient method
   */
  virtual UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension
   * @return The size of the point returned by the function whose gradient is computed
   */
  virtual UnsignedLong getOutputDimension() const;

  /** Accessor for the epsilon */
  virtual NumericalPoint getEpsilon() const;

  /** Accessor for the evaluation */
  virtual EvaluationImplementation getEvaluation() const;

  /** Accessor for the finite difference step */
  virtual void setFiniteDifferenceStep(const FiniteDifferenceStep & finiteDifferenceStep);
  virtual FiniteDifferenceStep getFiniteDifferenceStep() const;

  /* Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /* Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

  /* Here is the interface that all derived class must implement */

  /** Virtual Constructor */
  virtual FiniteDifferenceGradient * clone() const = 0;

  /** This method computes the gradient at some point
   * @param in The point where the gradient is computed
   * @result A matrix constructed with the dF_i/dx_j values (Jacobian transposed)
   */
  virtual Matrix gradient(const NumericalPoint & inP) const = 0;

protected:
  EvaluationImplementation p_evaluation_;
  FiniteDifferenceStep finiteDifferenceStep_;


}; /* class FiniteDifferenceGradient */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_FINITEDIFFERENCEGRADIENT_HXX */
