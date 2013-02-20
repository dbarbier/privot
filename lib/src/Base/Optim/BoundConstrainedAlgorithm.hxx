//                                               -*- C++ -*-
/**
 *  @file  BoundConstrainedAlgorithm.hxx
 *  @brief BoundConstrainedAlgorithm implements an algorithm for finding the
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 *  Id      BoundConstrainedAlgorithm.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_BOUNDCONSTRAINEDALGORITHM_HXX
#define OPENTURNS_BOUNDCONSTRAINEDALGORITHM_HXX

#include "TypedInterfaceObject.hxx"
#include "BoundConstrainedAlgorithmImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class BoundConstrainedAlgorithm
 * BoundConstrainedAlgorithm implements an interface to an algorithm for finding the
 * point of an interval that minimize the given objective function
 */

class BoundConstrainedAlgorithm
  : public TypedInterfaceObject<BoundConstrainedAlgorithmImplementation>
{

  CLASSNAME;
public:

  typedef Pointer<BoundConstrainedAlgorithmImplementation>          Implementation;
  typedef BoundConstrainedAlgorithmImplementation::OptimizationProblem      OptimizationProblem;
#ifndef SWIG
  typedef BoundConstrainedAlgorithmImplementation::Result                   Result;
#else
  typedef BoundConstrainedAlgorithmImplementationResult                     Result;
#endif

  /** Default constructor */
  explicit BoundConstrainedAlgorithm();

  /** Constructor from an implementation */
  BoundConstrainedAlgorithm(const BoundConstrainedAlgorithmImplementation & implementationconst);

  /** Constructor from a Pointer to an implementation */
  BoundConstrainedAlgorithm(const Implementation & p_implementation);

  /** Constructor with parameters: no constraint, starting from the origin */
  BoundConstrainedAlgorithm(const NumericalMathFunction & objectiveFunction,
                            const Bool verbose = false);

  /** Constructor with parameters: bound constraints, starting from the given point */
  BoundConstrainedAlgorithm(const NumericalMathFunction & objectiveFunction,
                            const Interval & boundConstraints,
                            const NumericalPoint & startingPoint,
                            const OptimizationProblem optimization  = BoundConstrainedAlgorithmImplementationResult::MINIMIZATION,
                            const Bool verbose = false);

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
  virtual String __repr__() const;

  /** Verbose accessor */
  Bool getVerbose() const;
  void setVerbose(const Bool verbose);

} ; /* class BoundConstrainedAlgorithm */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BOUNDCONSTRAINEDALGORITHM_HXX */
