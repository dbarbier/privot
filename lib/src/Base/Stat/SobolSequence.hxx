//                                               -*- C++ -*-
/**
 *  @file  SobolSequence.hxx
 *  @brief Implementation of the Sobol sequence
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
 *  Id      SobolSequence.hxx
 */

#ifndef OPENTURNS_SOBOLSEQUENCE_HXX
#define OPENTURNS_SOBOLSEQUENCE_HXX

#include "LowDiscrepancySequenceImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS
/**
 * @class SobolSequence
 */

class SobolSequence :
  public LowDiscrepancySequenceImplementation
{
  CLASSNAME;

public:
  // this implementation supports dimensions up to 40
  static const UnsignedLong MaximumNumberOfDimension;

  // this implementation has a cycle of 2^62 = ~5e18, thanks to the use of 64 bits integers (Unsigned64BitsInteger)
  static const UnsignedLong MaximumBase2Logarithm;

  // this value is 2^-MaximumBase2Logarithm, precomputed to speed up generation
  static const NumericalScalar Epsilon;

  // maximum number of columns in InitialBase array = 8
  static const UnsignedLong MaximumInitialDegree;

  // numbers used to generate the coefficients of directionNumber_[][] each row corresponds to a component (dimension)
  static const UnsignedLong InitialBase[];

  // a primitive polynomial used to generate the sequence
  static const Unsigned64BitsInteger PrimitivePolynomial[];

public:

  /** Constructor with parameters */
  explicit SobolSequence(const UnsignedLong dimension = 1);

  /** Virtual constructor */
  virtual SobolSequence * clone() const;

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

protected:
  /** The numbers used to generate the sequence */
  Unsigned64BitsIntegerPersistentCollection base_;
  Unsigned64BitsIntegerPersistentCollection coefficients_;

  /** Current seed */
  Unsigned64BitsInteger seed_;

private:
  /** return 2^n */
  static Unsigned64BitsInteger inline power2(const UnsignedLong n);

  /** Returns the position of the lowest '0' in the binary representation of an unsigned integer */
  static UnsignedLong computePositionOfLowest0Bit(const Unsigned64BitsInteger number);

}; /* class SobolSequence */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SOBOLSEQUENCE_HXX */
