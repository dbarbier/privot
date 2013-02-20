//                                               -*- C++ -*-
/**
 *  @file  NearestPointAlgorithmImplementationResult.hxx
 *  @brief NearestPointAlgorithmImplementationResult implements an algorithm for
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
 *  @author dutka
 *  @date   2010-02-04 16:44:49 +0100 (jeu. 04 févr. 2010)
 *  Id      NearestPointAlgorithmImplementationResult.hxx 1473 2010-02-04 15:44:49Z dutka
 */
#ifndef OPENTURNS_NEARESTPOINTALGORITHMIMPLEMENTATIONRESULT_HXX
#define OPENTURNS_NEARESTPOINTALGORITHMIMPLEMENTATIONRESULT_HXX

#include "OTprivate.hxx"
#include "Threadable.hxx"
#include "PersistentObject.hxx"
#include "NumericalMathFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS


/**
 * @class NearestPointAlgorithmImplementationResult
 * NearestPointAlgorithmImplementationResult stores the optimization result
 */
class NearestPointAlgorithmImplementationResult
  : public PersistentObject
{
  CLASSNAME;

public:


  /** Default constructor */
  explicit NearestPointAlgorithmImplementationResult();


  /** Standard constructor */
  NearestPointAlgorithmImplementationResult(const NumericalPoint & minimizer,
                                            const UnsignedLong iterationsNumber,
                                            const NumericalScalar absoluteError,
                                            const NumericalScalar relativeError,
                                            const NumericalScalar residualError,
                                            const NumericalScalar constraintError);

  /** Virtual constructor */
  virtual NearestPointAlgorithmImplementationResult * clone() const;

  /** Minimizer accessors */
  NumericalPoint getMinimizer() const;

  /** Iterations number accessor */
  UnsignedLong getIterationsNumber() const;

  /** Absolute error accessor */
  NumericalScalar getAbsoluteError() const;
  NumericalSample getAbsoluteErrorHistory() const;

  /** Relative error accessor */
  NumericalScalar getRelativeError() const;
  NumericalSample getRelativeErrorHistory() const;

  /** Residual error accessor */
  NumericalScalar getResidualError() const;
  NumericalSample getResidualErrorHistory() const;

  /** Constraint error accessor */
  NumericalScalar getConstraintError() const;
  NumericalSample getConstraintErrorHistory() const;

  /** Input / output sample accessor */
  NumericalSample getInputSample() const;
  NumericalSample getOutputSample() const;

  /** String converter */
  virtual String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

  /** Update current state */
  void update(const NumericalPoint & minimizer, UnsignedLong iterationNumber);

  /** Incremental history storage */
  void store(const NumericalPoint & inP,
             const NumericalPoint & outP,
             const NumericalScalar absoluteError,
             const NumericalScalar relativeError,
             const NumericalScalar residualError,
             const NumericalScalar constraintError);

  /** Draw error history graph */
  Graph getErrorHistory() const;

protected:

  /** Minimizer accessors */
  void setMinimizer(const NumericalPoint & minimizer);

  /** Iterations number accessor */
  void setIterationsNumber(const UnsignedLong iterationsNumber);

  /** Absolute error accessor */
  void setAbsoluteError(const NumericalScalar absoluteError);
  void setAbsoluteErrorHistory(const NumericalSample & absoluteError);

  /** Relative error accessor */
  void setRelativeError(const NumericalScalar relativeError);
  void setRelativeErrorHistory(const NumericalSample & relativeError);

  /** Residual error accessor */
  void setResidualError(const NumericalScalar residualError);
  void setResidualErrorHistory(const NumericalSample & residualError);

  /** Constraint error accessor */
  void setConstraintError(const NumericalScalar constraintError);
  void setConstraintErrorHistory(const NumericalSample & constraintError);


private:

  NumericalPoint  minimizer_;
  UnsignedLong    iterationsNumber_;       /**< Number of outermost iterations (in case of nested iterations) */
  NumericalScalar absoluteError_;   /**< Value of ||x_n - x_{n-1}|| */
  NumericalScalar relativeError_;   /**< Value of ||x_n - x_{n-1}|| / ||x_n|| */
  NumericalScalar residualError_;   /**< Value of ||objectiveFunction(x_n) - objectiveFunction(x_{n-1})|| */
  NumericalScalar constraintError_; /**< Value of ||constraints(x_n)|| for the active constraints */
  NumericalSample absoluteErrorHistory_;
  NumericalSample relativeErrorHistory_;
  NumericalSample residualErrorHistory_;
  NumericalSample constraintErrorHistory_;
  NumericalSample inputSample_;
  NumericalSample outputSample_;

}; // class NearestPointAlgorithmImplementationResult


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NEARESTPOINTALGORITHMIMPLEMENTATIONRESULT_HXX */
