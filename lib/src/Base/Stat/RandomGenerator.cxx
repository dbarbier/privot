//                                               -*- C++ -*-
/**
 *  @file  RandomGenerator.cxx
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
 */
#include <cstdlib>

#include "RandomGenerator.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

Bool RandomGenerator::IsInitialized = false;

RandomGenerator::MersenneTwister RandomGenerator::Generator(ResourceMap::GetAsUnsignedLong( "RandomGenerator-InitialSeed" ));

/* Sub-classes methods */

// Stream operator for State objects
std::ostream & operator << (std::ostream & os, const RandomGeneratorState & state)
{
  return os << "state=" << state.state_ << ", index=" << state.index_ << ")";
}

// Comparison operator for State objects
Bool operator == (const RandomGeneratorState & lhs, const RandomGeneratorState & rhs)
{
  return (lhs.state_ == rhs.state_) && (lhs.index_ == rhs.index_);
}

/* DefaultConstructor */
RandomGenerator::RandomGenerator()
{
  // Nothing to do
}

/* Seed accessor */
void RandomGenerator::SetSeed(const UnsignedLong seed)
{
  Generator.init((uint32_t)(seed));
  IsInitialized = true;
}

/* State accessor */
void RandomGenerator::SetState(const RandomGeneratorState & state)
{
  UnsignedLong size(state.state_.getSize());
  UnsignedLong stateSize(Generator.get_state_length_32());
  /* The unusual case, the given seed is too small. It is completed with 0 */
  UnsignedLongCollection stateArray(state.state_);
  for (UnsignedLong i = size; i < stateSize; i++) stateArray.add(0);
  // Set the state array
  Generator.set_state(&stateArray[0]);
  // Set the index
  Generator.set_index(state.index_);
  IsInitialized = true;
  return;
}

/* Seed accessor */
RandomGeneratorState RandomGenerator::GetState()
{
  UnsignedLong size((UnsignedLong)(Generator.get_state_length_32()));
  // Create the state and get the index at the same time
  RandomGeneratorState state(UnsignedLongCollection(size, 0), (UnsignedLong)(Generator.get_index()));
  // Get the state array
  Generator.get_state(&state.state_[0]);
  return state;
}

/* Generate a pseudo-random number uniformly distributed over ]0, 1[ */
NumericalScalar RandomGenerator::Generate()
{
  if (!IsInitialized)
    {
      SetSeed(ResourceMap::GetAsUnsignedLong( "RandomGenerator-InitialSeed" ));
      IsInitialized = true;
    }
  return Generator.gen();
}

/* Generate a pseudo-random integer uniformly distributed over [[0,...,n-1]] */
UnsignedLong RandomGenerator::IntegerGenerate(const UnsignedLong n)
{
  if (!IsInitialized)
    {
      SetSeed(ResourceMap::GetAsUnsignedLong( "RandomGenerator-InitialSeed" ));
      IsInitialized = true;
    }
  return Generator.igen((uint32_t)(n));
}

/* Generate a pseudo-random vector of numbers uniformly distributed over ]0, 1[ */
NumericalPoint RandomGenerator::Generate(const UnsignedLong size)
{
  NumericalPoint result(size);
  if (!IsInitialized)
    {
      SetSeed(ResourceMap::GetAsUnsignedLong( "RandomGenerator-InitialSeed" ));
      IsInitialized = true;
    }
  for (UnsignedLong i = 0; i < size; i++)
    {
      result[i] = Generator.gen();
    }
  return result;
}

/* Generate a pseudo-random vector of numbers uniformly distributed over ]0, 1[ */
RandomGenerator::UnsignedLongCollection RandomGenerator::IntegerGenerate(const UnsignedLong size, const UnsignedLong n)
{
  UnsignedLongCollection result(size);
  if (!IsInitialized)
    {
      SetSeed(ResourceMap::GetAsUnsignedLong( "RandomGenerator-InitialSeed" ));
      IsInitialized = true;
    }
  for (UnsignedLong i = 0; i < size; i++)
    {
      result[i] = Generator.igen((uint32_t)(n));
    }
  return result;
}

END_NAMESPACE_OPENTURNS
