//                                               -*- C++ -*-
/**
 *  @file  Factorial.hxx
 *  @brief Factorial experiment plane
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 *  Id      Factorial.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_FACTORIAL_HXX
#define OPENTURNS_FACTORIAL_HXX

#include "OTprivate.hxx"
#include "StratifiedExperiment.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class Factorial
 *
 * The class describes the probabilistic concept of factorial experiment plan
 */
class Factorial
  : public StratifiedExperiment
{
  CLASSNAME;
public:


  /** Default constructor */
  explicit Factorial(const String & name = DefaultName);

  /** Constructor with parameters */
  explicit Factorial(const NumericalPoint & center,
                     const NumericalPoint & levels,
                     const String & name = DefaultName);

  /** Constructor with parameters */
  explicit Factorial(const UnsignedLong dimension,
                     const NumericalPoint & levels,
                     const String & name = DefaultName);

  /** Virtual constructor */
  virtual Factorial * clone() const;

  /** Experiment plane generation */
  virtual NumericalSample generate();

  /** String converter */
  virtual String __repr__() const;

}; /* class Factorial */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_FACTORIAL_HXX */
