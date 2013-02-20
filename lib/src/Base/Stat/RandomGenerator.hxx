//                                               -*- C++ -*-
/**
 *  @file  RandomGenerator.hxx
 *  @brief RandomGenerator implements methods to control the random generator
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
 *  Id      RandomGenerator.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_RANDOMGENERATOR_HXX
#define OPENTURNS_RANDOMGENERATOR_HXX

#include "OTprivate.hxx"
#include "Collection.hxx"
#include "NumericalPoint.hxx"
#include "dsfmt.h"

BEGIN_NAMESPACE_OPENTURNS


/** A couple (internal state array, picking index) */
struct RandomGeneratorState
{
  typedef Collection<UnsignedLong> UnsignedLongCollection;
  UnsignedLongCollection state_;
  UnsignedLong index_;
  RandomGeneratorState() : state_(0), index_(0) {}
  RandomGeneratorState(const UnsignedLongCollection state, const UnsignedLong index) : state_(state), index_(index) {}
  ~RandomGeneratorState() {}
  const String __repr__() const
  {
    return OSS(true) << "RandomGeneratorState("
                     << state_ << ", "
                     << index_ << ")";
  }
  const String __str__(const String & offset = "") const
  {
    return OSS(false) << "RandomGeneratorState("
                      << state_ << ", "
                      << index_ << ")";
  }
}; /* end struct RandomGeneratorState */


/**
 * @class RandomGenerator
 *
 * RandomGenerator implements methods to control the random generator
 */

class RandomGenerator
{
public:

  typedef Collection<UnsignedLong> UnsignedLongCollection;
  typedef tutils::dsfmt19937             MersenneTwister;

  /** Default constructor */
  RandomGenerator();

  /** Seed accessor */
  static void SetSeed(const UnsignedLong seed);

  /** State accessor */
  static void SetState(const RandomGeneratorState & state);
  static RandomGeneratorState GetState();

  /** Generate a pseudo-random number uniformly distributed over [0, 1[ */
  static NumericalScalar Generate();
  /** Generate a pseudo-random integer uniformly distributed over [[0,...,n-1]] */
  static UnsignedLong IntegerGenerate(const UnsignedLong n);

  /** Generate a pseudo-random vector of numbers uniformly distributed over [0, 1[ */
  static NumericalPoint Generate(const UnsignedLong size);
  /** Generate a pseudo-random vector of integers uniformly distributed over [[0,...,n-1]] */
  static UnsignedLongCollection IntegerGenerate(const UnsignedLong size, const UnsignedLong n);

private:
  static Bool IsInitialized;
  static MersenneTwister Generator;

}; /* class RandomGenerator */


#ifndef SWIG
// Stream operator for State objects
std::ostream & operator << (std::ostream & os, const RandomGeneratorState & state);

// Comparison operator for State objects
Bool operator == (const RandomGeneratorState & lhs, const RandomGeneratorState & rhs);
#endif

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_RANDOMGENERATOR_HXX */
