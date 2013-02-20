//                                               -*- C++ -*-
/**
 *  @file  ReverseHaltonSequence.hxx
 *  @brief Implementation of the ReverseHalton sequence
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
 *  Id      ReverseHaltonSequence.hxx
 */

#ifndef OPENTURNS_REVERSEHALTONSEQUENCE_HXX
#define OPENTURNS_REVERSEHALTONSEQUENCE_HXX

#include "LowDiscrepancySequenceImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS
/**
 * @class ReverseHaltonSequence
 */

class ReverseHaltonSequence :
  public LowDiscrepancySequenceImplementation
{
  CLASSNAME;

public:

  /** Constructor with parameters */
  ReverseHaltonSequence(const UnsignedLong dimension = 1);

  /** Virtual constructor */
  virtual ReverseHaltonSequence * clone() const;

  /** Initialize the sequence */
  void initialize(const UnsignedLong dimension);

  /** Generate a quasi-random vector of numbers uniformly distributed over [0, 1[ */
  using LowDiscrepancySequenceImplementation::generate;
  NumericalPoint generate();

  /** String converter */
  String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:
  /** Base for the generation of the sequence */
  Unsigned64BitsIntegerPersistentCollection base_;

  /** Current seed into the sequence */
  Unsigned64BitsInteger seed_;

}; /* class ReverseHaltonSequence */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_REVERSEHALTONSEQUENCE_HXX */
