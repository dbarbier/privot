//                                               -*- C++ -*-
/**
 *  @file  Cobyla.hxx
 *  @brief Cobyla is an actual implementation for
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
 *  Id      Cobyla.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_COBYLA_HXX
#define OPENTURNS_COBYLA_HXX

#include "CobylaSpecificParameters.hxx"
#include "OTprivate.hxx"
#include "NearestPointAlgorithmImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class cobyla
 * cobyla is an actual implementation for
 * NearestPointAlgorithm using the cobyla library
 */
class Cobyla :
  public NearestPointAlgorithmImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  Cobyla();

  /** Constructor with parameters */
  Cobyla(const NumericalMathFunction & levelFunction,
         const Bool verbose = false);


  /** Constructor with parameters */
  Cobyla(const CobylaSpecificParameters & specificParameters,
         const NumericalMathFunction & levelFunction,
         const Bool verbose = false);


  /** Virtual constructor */
  virtual Cobyla * clone() const;

  /** Performs the actual computation. Must be overloaded by the actual optimisation algorithm */
  void run();

  /** Specific parameters accessor */
  CobylaSpecificParameters getSpecificParameters() const;

  /** Specific parameters accessor */
  void setSpecificParameters(const CobylaSpecificParameters & specificParameters);
  /** String converter */
  String __repr__() const;

private:

  /**
   * Function that computes half of the squared of a Vector. It will
   * become the objective function of the minimisation problem
   */
  static int ComputeObjectiveAndConstraint(int n,
                                           int m,
                                           double *x,
                                           double *f,
                                           double *con,
                                           void *state);
  CobylaSpecificParameters specificParameters_;
}; /* class Cobyla */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COBYLA_HXX */
