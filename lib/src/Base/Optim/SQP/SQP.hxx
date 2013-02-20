//                                               -*- C++ -*-
/**
 *  @file  SQP.hxx
 *  @brief SQP is an actual implementation for
 *         NearestPointAlgorithm using the SQP algorithm.
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
 *  Id      SQP.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_SQP_HXX
#define OPENTURNS_SQP_HXX

#include "OTprivate.hxx"
#include "SQPSpecificParameters.hxx"
#include "NearestPointAlgorithmImplementation.hxx"
#include "NumericalPoint.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class SQP
 * SQP is an actual implementation for
 * NearestPointAlgorithm
 */

class SQP :
  public NearestPointAlgorithmImplementation
{
  CLASSNAME;

public:


  /** Default constructor */
  SQP();

  /** Constructor with parameters */
  SQP(const SQPSpecificParameters & specificParameters,
      const NumericalMathFunction & levelFunction,
      const Bool verbose = false);


  /** Virtual constructor */
  virtual SQP * clone() const;

  /** Performs the actual computation. Must be overloaded by the actual optimisation algorithm */
  void run();

  /** Specific parameters accessor */
  SQPSpecificParameters getSpecificParameters() const;

  /** Specific parameters accessor */
  void setSpecificParameters(const SQPSpecificParameters & specificParameters);

  /** String converter */
  String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


private:

  /** Perform a line search in the given direction */
  NumericalScalar computeLineSearch();

  /** Initialize internal state */
  void initialize();

  /** SQP specific parameters */
  SQPSpecificParameters specificParameters_;

  /** SQP current penalization factor */
  NumericalScalar currentSigma_;

  /** SQP current point */
  NumericalPoint currentPoint_;

  /** SQP current direction */
  NumericalPoint currentDirection_;

  /** SQP current level value */
  NumericalScalar currentLevelValue_;

  /** SQP current gradient as a column vector */
  NumericalPoint currentGradient_;

  /** SQP current hessian as a symmetric tensor */
  SymmetricMatrix currentHessian_;

  /** SQP current system matrix as a symmetric matrix */
  SymmetricMatrix currentSystemMatrix_;

  /** SQP current system second member as a column vector */
  NumericalPoint currentSecondMember_;

  /** SQP current Lagrange multiplier */
  NumericalScalar currentLambda_;

}; /* class SQP */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SQP_HXX */
