//                                               -*- C++ -*-
/**
 *  @file  KPermutations.hxx
 *  @brief KPermutations experiment plane
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
 *  Id      KPermutations.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_KPERMUTATIONS_HXX
#define OPENTURNS_KPERMUTATIONS_HXX

#include "CombinatorialGeneratorImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class KPermutations
 *
 * The class describes the concept of kPermutations generator
 */
class KPermutations
  : public CombinatorialGeneratorImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  KPermutations();

  KPermutations(const UnsignedLong n);

  KPermutations(const UnsignedLong k,
                const UnsignedLong n);

  /** Virtual constructor */
  virtual KPermutations * clone() const;

  /** Experiment plane generation :
   *  all the kPermutations of k elements amongst {0, ..., n-1}
   */
  virtual IndicesCollection generate();

  /** String converter */
  virtual String __repr__() const;

  /** Subset size accessor */
  void setK(const UnsignedLong k);

  UnsignedLong getK() const;

  /** Set size accessor */
  void setN(const UnsignedLong n);

  UnsignedLong getN() const;

private:
  /** Size of the subsets */
  UnsignedLong k_;

  /** Size of the set */
  UnsignedLong n_;

}; /* class KPermutations */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_KPERMUTATIONS_HXX */
