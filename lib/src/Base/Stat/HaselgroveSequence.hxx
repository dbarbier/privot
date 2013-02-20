//                                               -*- C++ -*-
/**
 *  @file  HaselgroveSequence.hxx
 *  @brief Implementation of the Haselgrove sequence
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
 *  Id      HaselgroveSequence.hxx
 */

#ifndef OPENTURNS_HASELGROVESEQUENCE_HXX
#define OPENTURNS_HASELGROVESEQUENCE_HXX

#include "LowDiscrepancySequenceImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS
/**
 * @class HaselgroveSequence
 */

class HaselgroveSequence :
  public LowDiscrepancySequenceImplementation
{
  CLASSNAME;

public:

  /** Constructor with parameters */
  explicit HaselgroveSequence(const UnsignedLong dimension = 1);

  /** Constructor with parameters */
  explicit HaselgroveSequence(const NumericalPoint & base);

  /** Virtual constructor */
  virtual HaselgroveSequence * clone() const;

  /** Initialize the sequence */
  void initialize(const UnsignedLong dimension);

  /** Generate a quasi-random vector of numbers uniformly distributed over [0, 1[ */
  using LowDiscrepancySequenceImplementation::generate;
  NumericalPoint generate();

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

private:

  /** Base for the generation of the sequence */
  NumericalPoint base_;

  /** Current seed into the sequence */
  Unsigned64BitsInteger seed_;

}; /* class HaselgroveSequence */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_HASELGROVESEQUENCE_HXX */
