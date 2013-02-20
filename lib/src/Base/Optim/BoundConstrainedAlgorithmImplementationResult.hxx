//                                               -*- C++ -*-
/**
 *  @file  BoundConstrainedAlgorithmImplementationResult.hxx
 *  @brief BoundConstrainedAlgorithmImplementationResult implements the result of an algorithm for finding the
 *         point of an interval that minimize the given objective function
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
 *  @date   2009-05-28 14:47:53 +0200 (jeu. 28 mai 2009)
 *  Id      BoundConstrainedAlgorithmImplementationResult.hxx 1262 2009-05-28 12:47:53Z dutka
 */
#ifndef OPENTURNS_BOUNDCONSTRAINEDALGORITHMIMPLEMENTATIONRESULT_HXX
#define OPENTURNS_BOUNDCONSTRAINEDALGORITHMIMPLEMENTATIONRESULT_HXX

#include "PersistentObject.hxx"
#include "NumericalPoint.hxx"
#include "StorageManager.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class BoundConstrainedAlgorithmImplementationResult
 * BoundConstrainedAlgorithmImplementationResult implements the result of a BoundConstrainedAlgorithmImplementation, i.e. the location of a point that minimize or maximize a given objective function over an interval, as well as the optimal value, the number of evaluations of the objective function, the kind of optimization and the various errors achieved at the optimum.
 */

class BoundConstrainedAlgorithmImplementationResult
  : public PersistentObject
{
  CLASSNAME;

public:

  typedef enum {MINIMIZATION = 0, MAXIMIZATION} OptimizationProblem;

  /** Default constructor */
  explicit BoundConstrainedAlgorithmImplementationResult();

  /** Standard constructor */
  BoundConstrainedAlgorithmImplementationResult(const NumericalPoint & optimizer,
                                                const NumericalScalar optimalValue,
                                                const OptimizationProblem optimization,
                                                const UnsignedLong evaluationsNumber,
                                                const NumericalScalar absoluteError,
                                                const NumericalScalar relativeError,
                                                const NumericalScalar objectiveError,
                                                const NumericalScalar constraintError);


  /** Virtual constructor */
  virtual BoundConstrainedAlgorithmImplementationResult * clone() const;

  /** Optimizer accessors */
  NumericalPoint getOptimizer() const;

  /** Optimal value accessor */
  NumericalScalar getOptimalValue() const;

  /** Optimization problem accessor */
  OptimizationProblem getOptimizationProblem() const;

  /** Iterations number accessor */
  UnsignedLong getEvaluationsNumber() const;

  /** Absolute error accessor */
  NumericalScalar getAbsoluteError() const;

  /** Relative error accessor */
  NumericalScalar getRelativeError() const;

  /** Objective error accessor */
  NumericalScalar getObjectiveError() const;

  /** Constraint error accessor */
  NumericalScalar getConstraintError() const;

  /** String converter */
  virtual String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  /** Optimizer accessors */
  void setOptimizer(const NumericalPoint & optimizer);

  /** Iterations number accessor */
  void setEvaluationsNumber(const UnsignedLong evaluationsNumber);

  /** Optimal value accessor */
  void setOptimalValue(const NumericalScalar optimalValue);

  /** Optimization problem accessor */
  void setOptimizationProblem(const OptimizationProblem optimization);

  /** Absolute error accessor */
  void setAbsoluteError(const NumericalScalar absoluteError);

  /** Relative error accessor */
  void setRelativeError(const NumericalScalar relativeError);

  /** Objective error accessor */
  void setObjectiveError(const NumericalScalar objectiveError);

  /** Constraint error accessor */
  void setConstraintError(const NumericalScalar constraintError);

private:

  NumericalPoint  optimizer_;
  NumericalScalar optimalValue_;
  OptimizationProblem optimization_;
  UnsignedLong    evaluationsNumber_;       /**< Number of outermost iterations (in case of nested iterations) */
  NumericalScalar absoluteError_;   /**< Value of ||x_n - x_{n-1}|| */
  NumericalScalar relativeError_;   /**< Value of ||x_n - x_{n-1}|| / ||x_n|| */
  NumericalScalar objectiveError_;   /**< Value of ||objectiveFunction(x_n) - objectiveFunction(x_{n-1})|| */
  NumericalScalar constraintError_; /**< Value of ||constraints(x_n)|| for the active constraints */
}; // class BoundConstrainedAlgorithmImplementationResult

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BOUNDCONSTRAINEDALGORITHMIMPLEMENTATIONRESULT_HXX */
