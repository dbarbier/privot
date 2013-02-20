//                                               -*- C++ -*-
/**
 *  @file  ReverseHaltonSequence.cxx
 *  @brief Implementation of the ReverseHalton' sequence
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include <cstdlib>

#include "ReverseHaltonSequence.hxx"
#include "ResourceMap.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ReverseHaltonSequence);


/* Constructor with parameters */
ReverseHaltonSequence::ReverseHaltonSequence(const UnsignedLong dimension) :
  LowDiscrepancySequenceImplementation(dimension)
{
  initialize(dimension);
}


/* Virtual constructor */
ReverseHaltonSequence * ReverseHaltonSequence::clone() const
{
  return new ReverseHaltonSequence(*this);
}


/* Initialize the sequence */
void ReverseHaltonSequence::initialize(const UnsignedLong dimension)
{
  if (dimension == 0) throw InvalidArgumentException(HERE) << "Dimension must be > 0.";
  dimension_ = dimension;
  base_ = ComputeFirstPrimeNumbers(dimension);
  seed_ = ResourceMap::GetAsUnsignedLong( "ReverseHaltonSequence-InitialSeed" );
}

/* Generate a pseudo-random vector of independant numbers uniformly distributed over [0, 1[
   See Bart Vandewoestyne, Ronald Cools, "Good permutations for deterministic scrambled Halton sequences in terms of L2-discrepancy", Journal of Computational and Applied Mathematics, 189, 341-361 (2006) */
NumericalPoint ReverseHaltonSequence::generate()
{
  NumericalPoint realization(dimension_);
  // Loop over the components
  for (UnsignedLong i = 0; i < dimension_; ++i)
    {
      NumericalScalar xI(0.0);
      const Unsigned64BitsInteger radix(base_[i]);
      const NumericalScalar inverseRadix(1.0 / radix);
      NumericalScalar inverseRadixN(inverseRadix);
      Unsigned64BitsInteger currentSeed(seed_);
      while (currentSeed > 0)
        {
          const UnsignedLong digit(currentSeed % radix);
          if (digit != 0) xI += (radix - digit) * inverseRadixN;
          currentSeed /= radix;
          inverseRadixN *= inverseRadix;
        }
      realization[i] = xI;
    }
  ++seed_;
  return realization;
}


/* String converter */
String ReverseHaltonSequence::__repr__() const
{
  OSS oss;
  oss << "class=" << ReverseHaltonSequence::GetClassName()
      << " derived from " << LowDiscrepancySequenceImplementation::__repr__()
      << " base=" << base_
      << " seed=" << seed_;
  return oss;
}

void ReverseHaltonSequence::save(Advocate & adv) const
{
  LowDiscrepancySequenceImplementation::save(adv);
  adv.saveAttribute( "base_", base_);
  adv.saveAttribute( "seed_", seed_);
}


/* Method load() reloads the object from the StorageManager */
void ReverseHaltonSequence::load(Advocate & adv)
{
  LowDiscrepancySequenceImplementation::load(adv);
  initialize(dimension_);
  adv.loadAttribute( "base_", base_);
  adv.loadAttribute( "seed_", seed_);
}


END_NAMESPACE_OPENTURNS
