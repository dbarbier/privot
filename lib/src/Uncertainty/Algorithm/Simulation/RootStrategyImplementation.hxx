//                                               -*- C++ -*-
/**
 *  @file  RootStrategyImplementation.hxx
 *  @brief
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
 *  Id      RootStrategyImplementation.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_ROOTSTRATEGYIMPLEMENTATION_HXX
#define OPENTURNS_ROOTSTRATEGYIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "Solver.hxx"
#include "Collection.hxx"
#include "PersistentObject.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class RootStrategyImplementation
 */

class RootStrategyImplementation
  : public PersistentObject
{

  CLASSNAME;
public:

  typedef Collection<NumericalScalar> NumericalScalarCollection;

  /** Default constructor with Brent solver */
  explicit RootStrategyImplementation();

  /** Constructor with parameters */
  RootStrategyImplementation(const Solver & solver);

  /** Constructor with parameters */
  RootStrategyImplementation(const Solver & solver,
                             const NumericalScalar maximumDistance,
                             const NumericalScalar stepSize);


  /** Virtual constructor */
  virtual RootStrategyImplementation * clone() const;

  /** Solve gives all the roots found applying the root strategy */
  virtual NumericalScalarCollection solve(const NumericalMathFunction & function,
                                          const NumericalScalar value);

  /** Solver accessor */
  void setSolver(const Solver & solver);
  Solver getSolver() const;

  /** Maximum distance accessor */
  void setMaximumDistance(const NumericalScalar maximumDistance);
  NumericalScalar getMaximumDistance() const;

  /** Step size accessor */
  void setStepSize(const NumericalScalar stepSize);
  NumericalScalar getStepSize() const;

  /** Value of the performance function at the origin accessor */
  void setOriginValue(const NumericalScalar originValue);
  NumericalScalar getOriginValue() const;

  /** String converter */
  virtual String __repr__() const;

protected:

private:
  /** Univariate root solver for the localization of zeros along a ray */
  Solver solver_;
  /** Distance beyond which the density is negligible */
  NumericalScalar maximumDistance_;
  /** Size of the intervals in which we look for zeros */
  NumericalScalar stepSize_;
  /** Flag to avoid repeated evaluations of the model at the origin */
  mutable Bool isAlreadyComputedOriginValue_;
  /** Value of the model at the origin */
  NumericalScalar originValue_;

} ; /* class RootStrategyImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ROOTSTRATEGYIMPLEMENTATION_HXX */
