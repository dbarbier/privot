//                                               -*- C++ -*-
/**
 *  @file  BoundConstrainedAlgorithmImplementation.hxx
 *  @brief BoundConstrainedAlgorithmImplementation implements an algorithm for finding the
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
 *  @author schueller
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 *  Id      BoundConstrainedAlgorithmImplementation.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_BOUNDCONSTRAINEDALGORITHMIMPLEMENTATION_HXX
#define OPENTURNS_BOUNDCONSTRAINEDALGORITHMIMPLEMENTATION_HXX

#include "Threadable.hxx"
#include "PersistentObject.hxx"
#include "NumericalMathFunction.hxx"
#include "Interval.hxx"
#include "StorageManager.hxx"
#include "BoundConstrainedAlgorithmImplementationResult.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class BoundConstrainedAlgorithmImplementation
 * BoundConstrainedAlgorithm implements an algorithm for finding the
 * point of an interval that minimize the given objective function
 */

class BoundConstrainedAlgorithmImplementation
  : public PersistentObject,
    public Threadable
{

  CLASSNAME;
public:

  typedef BoundConstrainedAlgorithmImplementationResult Result;
  typedef Result::OptimizationProblem      OptimizationProblem;
  //typedef enum {MINIMIZATION = 0, MAXIMIZATION} OptimizationProblem;

  /** Default constructor */
  explicit BoundConstrainedAlgorithmImplementation();

  /** Constructor with parameters: no constraint, starting from the origin */
  BoundConstrainedAlgorithmImplementation(const NumericalMathFunction & objectiveFunction,
                                          const Bool verbose = true);

  /** Constructor with parameters: bound constraints, starting from the given point */
  BoundConstrainedAlgorithmImplementation(const NumericalMathFunction & objectiveFunction,
                                          const Interval & boundConstraints,
                                          const NumericalPoint & startingPoint,
                                          const OptimizationProblem optimization  = Result::MINIMIZATION,
                                          const Bool verbose = true);

  /** Virtual constructor */
  virtual BoundConstrainedAlgorithmImplementation * clone() const;

  /** Performs the actual computation. Must be overloaded by the actual optimisation algorithm */
  virtual void run();

  /** Starting point accessor */
  NumericalPoint getStartingPoint() const;
  void setStartingPoint(const NumericalPoint & startingPoint);

  /** Objective function accessor */
  NumericalMathFunction getObjectiveFunction() const;
  void setObjectiveFunction(const NumericalMathFunction & objectiveFunction);

  /** Bound constraints accessor */
  Interval getBoundConstraints() const;
  void setBoundConstraints(const Interval & boundConstraints);

  /** Optimization problem accessor */
  OptimizationProblem getOptimizationProblem() const;
  void setOptimizationProblem(const OptimizationProblem optimization);

  /** Result accessor */
  Result getResult() const;
  void setResult(const Result & result);

  /** Maximum iterations number accessor */
  UnsignedLong getMaximumEvaluationsNumber() const;
  void setMaximumEvaluationsNumber(const UnsignedLong maximumEvaluationsNumber);

  /** Maximum absolute error accessor */
  NumericalScalar getMaximumAbsoluteError() const;
  void setMaximumAbsoluteError(const NumericalScalar maximumAbsoluteError);

  /** Maximum relative error accessor */
  NumericalScalar getMaximumRelativeError() const;
  void setMaximumRelativeError(const NumericalScalar maximumRelativeError);

  /** Maximum objective error accessor */
  NumericalScalar getMaximumObjectiveError() const;
  void setMaximumObjectiveError(const NumericalScalar maximumObjectiveError);

  /** Maximum constraint error accessor */
  NumericalScalar getMaximumConstraintError() const;
  void setMaximumConstraintError(const NumericalScalar maximumConstraintError);

  /** String converter */
  String __repr__() const;

  /** Verbose accessor */
  Bool getVerbose() const;
  void setVerbose(const Bool verbose);

protected:

private:
  NumericalMathFunction objectiveFunction_;
  Interval boundConstraints_;
  NumericalPoint startingPoint_;
  OptimizationProblem optimization_;
  UnsignedLong    maximumEvaluationsNumber_; /**< Number of outermost iterations (in case of nested iterations) */
  NumericalScalar maximumAbsoluteError_;    /**< Value of ||x_n - x_{n-1}|| */
  NumericalScalar maximumRelativeError_;    /**< Value of ||x_n - x_{n-1}|| / ||x_n|| */
  NumericalScalar maximumObjectiveError_;    /**< Value of ||objectiveFunction(x_n) - objectiveFunction(x_{n-1})|| */
  NumericalScalar maximumConstraintError_;  /**< Value of ||constraints(x_n)|| for the active constraints */
  Result result_;
  Bool verbose_;
} ; /* class BoundConstrainedAlgorithmImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BOUNDCONSTRAINEDALGORITHMIMPLEMENTATION_HXX */
