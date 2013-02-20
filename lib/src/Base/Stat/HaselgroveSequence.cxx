//                                               -*- C++ -*-
/**
 *  @file  HaselgroveSequence.cxx
 *  @brief Implementation of the Haselgrove' sequence
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
#include <cmath>

#include "HaselgroveSequence.hxx"
#include "ResourceMap.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(HaselgroveSequence);

/* Constructor with parameters */
HaselgroveSequence::HaselgroveSequence(const UnsignedLong dimension) :
  LowDiscrepancySequenceImplementation(dimension)
{
  initialize(dimension);
}

/* Constructor with parameters */
HaselgroveSequence::HaselgroveSequence(const NumericalPoint & base) :
  LowDiscrepancySequenceImplementation(base.getDimension())
{
  dimension_ = base.getDimension();
  base_ = base;
  seed_ = ResourceMap::GetAsUnsignedLong( "HaselgroveSequence-InitialSeed" );
}

/* Virtual constructor */
HaselgroveSequence * HaselgroveSequence::clone() const
{
  return new HaselgroveSequence(*this);
}

/* Initialize the sequence */
void HaselgroveSequence::initialize(const UnsignedLong dimension)
{
  if (dimension == 0) throw InvalidArgumentException(HERE) << "Dimension must be > 0.";
  dimension_ = dimension;
  const Unsigned64BitsIntegerCollection directions(ComputeFirstPrimeNumbers(dimension));
  base_ = NumericalPoint(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i) base_[i] = sqrt(directions[i]);
  seed_ = ResourceMap::GetAsUnsignedLong( "HaselgroveSequence-InitialSeed" );
}

/* Generate a pseudo-random vector of independant numbers uniformly distributed over [0, 1[ */
NumericalPoint HaselgroveSequence::generate()
{
  NumericalPoint realization(dimension_);
  // Loop over the components
  NumericalScalar scratch(0.0);
  for (UnsignedLong i = 0; i < dimension_; ++i) realization[i] = modf(seed_ * base_[i], &scratch);
  ++seed_;
  return realization;
}

/* String converter */
String HaselgroveSequence::__repr__() const
{
  OSS oss;
  oss << "class=" << HaselgroveSequence::GetClassName()
      << " derived from " << LowDiscrepancySequenceImplementation::__repr__()
      << " base=" << base_
      << " seed=" << seed_;
  return oss;
}

/* String converter */
String HaselgroveSequence::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class=" << HaselgroveSequence::GetClassName()
      << " derived from " << LowDiscrepancySequenceImplementation::__str__(offset)
      << " base=" << base_
      << " seed=" << seed_;
  return oss;
}

END_NAMESPACE_OPENTURNS
