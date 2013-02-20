//                                               -*- C++ -*-
/**
 *  @file  NearestPointAlgorithmImplementation.hxx
 *  @brief NearestPointAlgorithmImplementation implements an algorithm for
 *         finding the point of an implicitely defined manifold the nearest
 *         to the origin.
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
 *  Id      NearestPointAlgorithmImplementation.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_NEARESTPOINTALGORITHMIMPLEMENTATION_HXX
#define OPENTURNS_NEARESTPOINTALGORITHMIMPLEMENTATION_HXX

#include "OTprivate.hxx"
#include "Threadable.hxx"
#include "PersistentObject.hxx"
#include "NumericalMathFunction.hxx"
#include "NearestPointAlgorithmImplementationResult.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class NearestPointAlgorithmImplementation
 * NearestPointAlgorithmImplementation implements an algorithm for finding the
 * point the nearest to the origin of an implicitely defined manifold
 */

class NearestPointAlgorithmImplementation
  : public PersistentObject,
    public Threadable
{

  CLASSNAME;
public:

  typedef NearestPointAlgorithmImplementationResult              Result;

  /** Default constructor */
  explicit NearestPointAlgorithmImplementation();

  /** Constructor with parameters */
  NearestPointAlgorithmImplementation(const NumericalMathFunction & levelFunction,
                                      const Bool verbose = false);


  /** Virtual constructor */
  virtual NearestPointAlgorithmImplementation * clone() const;

  /** Performs the actual computation. Must be overloaded by the actual optimisation algorithm */
  virtual void run();

  /** Starting point accessor */
  NumericalPoint getStartingPoint() const;

  /** Starting point accessor */
  void setStartingPoint(const NumericalPoint & startingPoint);

  /** Level function accessor */
  NumericalMathFunction getLevelFunction() const;

  /** Level function accessor */
  void setLevelFunction(const NumericalMathFunction & levelFunction);

  /** Level value accessor */
  NumericalScalar getLevelValue() const;

  /** Level value accessor */
  void setLevelValue(const NumericalScalar levelValue);

  /** Result accessor */
  Result getResult() const;

  /** Result accessor */
  void setResult(const Result & result);

  /** Maximum iterations number accessor */
  UnsignedLong getMaximumIterationsNumber() const;

  /** Maximum iterations number accessor */
  void setMaximumIterationsNumber(const UnsignedLong maximumIterationsNumber);

  /** Maximum absolute error accessor */
  NumericalScalar getMaximumAbsoluteError() const;

  /** Maximum absolute error accessor */
  void setMaximumAbsoluteError(const NumericalScalar maximumAbsoluteError);

  /** Maximum relative error accessor */
  NumericalScalar getMaximumRelativeError() const;

  /** Maximum relative error accessor */
  void setMaximumRelativeError(const NumericalScalar maximumRelativeError);

  /** Maximum residual error accessor */
  NumericalScalar getMaximumResidualError() const;

  /** Maximum residual error accessor */
  void setMaximumResidualError(const NumericalScalar maximumResidualError);

  /** Maximum constraint error accessor */
  NumericalScalar getMaximumConstraintError() const;

  /** Maximum constraint error accessor */
  void setMaximumConstraintError(const NumericalScalar maximumConstraintError);

  /** String converter */
  String __repr__() const;

  /** Verbose accessor */
  Bool getVerbose() const;
  void setVerbose(const Bool verbose);

protected:
  Result result_;

private:
  NumericalMathFunction levelFunction_;
  NumericalPoint startingPoint_;
  NumericalScalar levelValue_;
  UnsignedLong    maximumIterationsNumber_; /**< Number of outermost iterations (in case of nested iterations) */
  NumericalScalar maximumAbsoluteError_;    /**< Value of ||x_n - x_{n-1}|| */
  NumericalScalar maximumRelativeError_;    /**< Value of ||x_n - x_{n-1}|| / ||x_n|| */
  NumericalScalar maximumResidualError_;    /**< Value of ||objectiveFunction(x_n) - objectiveFunction(x_{n-1})|| */
  NumericalScalar maximumConstraintError_;  /**< Value of ||constraints(x_n)|| for the active constraints */
  Bool verbose_;

} ; /* class NearestPointAlgorithmImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NEARESTPOINTALGORITHMIMPLEMENTATION_HXX */
