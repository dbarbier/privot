//                                               -*- C++ -*-
/**
 *  @file  SafeAndSlow.hxx
 *  @brief Find the roots in a given direction according to the Proban
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
 *  Id      SafeAndSlow.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_SAFEANDSLOW_HXX
#define OPENTURNS_SAFEANDSLOW_HXX

#include "RootStrategyImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class SafeAndSlow
 */

class SafeAndSlow :
  public RootStrategyImplementation
{

  CLASSNAME;
public:


  /** Default constructor */
  explicit SafeAndSlow();

  /** Constructor with parameters */
  SafeAndSlow(const Solver & solver);

  /** Constructor with parameters */
  SafeAndSlow(const Solver & solver,
              const NumericalScalar maximumDistance,
              const NumericalScalar stepSize);


  /** Virtual constructor */
  virtual SafeAndSlow * clone() const;

  /** Solve gives all the roots found applying the root strategy */
  NumericalScalarCollection solve(const NumericalMathFunction & function,
                                  const NumericalScalar value);

  /** String converter */
  String __repr__() const;

private:

} ; /* class SafeAndSlow */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SAFEANDSLOW_HXX */
