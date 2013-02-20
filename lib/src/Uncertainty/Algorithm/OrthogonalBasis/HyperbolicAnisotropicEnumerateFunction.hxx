//                                               -*- C++ -*-
/**
 *  @file  HyperbolicAnisotropicEnumerateFunction.hxx
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
#ifndef OPENTURNS_HYPERBOLICANISOTROPICENUMERATEFUNCTION_HXX
#define OPENTURNS_HYPERBOLICANISOTROPICENUMERATEFUNCTION_HXX

#include <list>

#include "EnumerateFunctionImplementation.hxx"
#include "NumericalPoint.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class HyperbolicAnisotropicEnumerateFunction
 *
 * The bijective function to select polynomials in the orthogonal basis
 */

class HyperbolicAnisotropicEnumerateFunction
  : public EnumerateFunctionImplementation
{
  CLASSNAME;
public:

  typedef std::pair< Indices , NumericalScalar > ValueType;
  typedef std::list<ValueType>                   IndiceCache;
  typedef Collection<Indices>                    IndicesCollection;

  /** Default constructor */
  HyperbolicAnisotropicEnumerateFunction();

  /** Parameter constructor */
  HyperbolicAnisotropicEnumerateFunction(const UnsignedLong dimension,
                                         const NumericalScalar q = ResourceMap::GetAsNumericalScalar("HyperbolicAnisotropicEnumerateFunction-DefaultQ"));

  /** Parameter constructor */
  HyperbolicAnisotropicEnumerateFunction(const NumericalPoint & weight,
                                         const NumericalScalar q = ResourceMap::GetAsNumericalScalar("HyperbolicAnisotropicEnumerateFunction-DefaultQ"));

  /** Virtual constrcutor */
  virtual HyperbolicAnisotropicEnumerateFunction * clone() const;

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

  /** Q accessor */
  NumericalScalar getQ() const;

  /** Weight accessor */
  NumericalPoint getWeight() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:
  /** Initialize the generation of indices */
  void initialize();

  /** Returns the q-norm of the indice set */
  NumericalScalar computeQNorm(const Indices & indices) const;

  /** Tells if indice1 is a neighbor of indice2 */
  Bool isNeighbor(const Indices & indice1,
                  const Indices & indice2) const;

private:

  /** Weight of each component */
  NumericalPoint weight_;

  /** Q-Norm q term */
  NumericalScalar q_;

  /** Cache of already generated indices */
  mutable IndiceCache candidates_;

  /** Candidate indices */
  mutable IndicesCollection cache_;

  /** Cumulated strata cardinals */
  mutable Indices strataCumulatedCardinal_;

} ; /* class HyperbolicAnisotropicEnumerateFunction */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_HYPERBOLICANISOTROPICENUMERATEFUNCTION_HXX */
