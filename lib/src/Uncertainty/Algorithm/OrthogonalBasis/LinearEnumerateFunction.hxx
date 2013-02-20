//                                               -*- C++ -*-
/**
 *  @file  LinearEnumerateFunction.hxx
 *  @brief The bijective function to select polynomials in the orthogonal basis
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
 *  @date   2008-05-21 17:44:02 +0200 (Wed, 21 May 2008)
 *  Id      Object.hxx 818 2008-05-21 15:44:02Z dutka
 */
#ifndef OPENTURNS_LINEARENUMERATEFUNCTION_HXX
#define OPENTURNS_LINEARENUMERATEFUNCTION_HXX

#include "EnumerateFunctionImplementation.hxx"


BEGIN_NAMESPACE_OPENTURNS



/**
 * @class LinearEnumerateFunction
 *
 * The bijective function to select polynomials in the orthogonal basis
 */

class LinearEnumerateFunction
  : public EnumerateFunctionImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  LinearEnumerateFunction();

  /** Parameter constructor */
  LinearEnumerateFunction(const UnsignedLong dimension);

  /** Virtual constrcutor */
  virtual LinearEnumerateFunction * clone() const;

  /** String converter */
  virtual String __repr__() const;

  /** The bijective association between an integer and a set of indices */
  virtual Indices operator() (const UnsignedLong index) const;

  /** The inverse of the association */
  virtual UnsignedLong inverse(const Indices & indices) const;

  /** The cardinal of the given strata */
  virtual UnsignedLong getStrataCardinal(const UnsignedLong strataIndex) const;

  /** The cardinal of the cumulated strata above or equal to the given strata */
  virtual UnsignedLong getStrataCumulatedCardinal(const UnsignedLong strataIndex) const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

private:
  /** Find the smallest n such that Binomial(n, n + dimension) > index and also returns the value of Binomial(n, n + dimension) */
  UnsignedLong findBinomialCoefficient(const UnsignedLong index, const UnsignedLong dimension, UnsignedLong & BinomialCoefficient) const;

} ; /* class LinearEnumerateFunction */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARENUMERATEFUNCTION_HXX */
