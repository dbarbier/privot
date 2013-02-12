//                                               -*- C++ -*-
/**
 *  @file  KPermutations.cxx
 *  @brief Implementation of the kPermutations experiment plane
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
#include <cmath>
#include <algorithm>
#include <iterator>
#include "OTprivate.hxx"
#include "KPermutations.hxx"
#include "SpecFunc.hxx"
#include "Combinations.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(KPermutations);

/* Default constructor */
KPermutations::KPermutations()
  : CombinatorialGeneratorImplementation()
  , k_(0)
  , n_(0)
{
  // Nothing to do
}

/* Constructor with parameters */
KPermutations::KPermutations(const UnsignedLong n)
  : CombinatorialGeneratorImplementation()
  , k_(n)
  , n_(n)
{
  // Nothing to do
}

KPermutations::KPermutations(const UnsignedLong k,
                             const UnsignedLong n)
  : CombinatorialGeneratorImplementation()
  , k_(k)
  , n_(n)
{
  // Nothing to do
}

/* Virtual constructor */
KPermutations * KPermutations::clone() const
{
  return new KPermutations(*this);
}

/* Experiment plane generation :
 *  all the kPermutations of k elements amongst {0, ..., n-1}
 */
CombinatorialGeneratorImplementation::IndicesCollection KPermutations::generate()
{
  /* Quick return for trivial cases */
  if (k_ > n_) return IndicesCollection(0, Indices(k_));
  if (k_ == 0) return IndicesCollection(1, Indices(0));
  Indices indices(k_);
  indices.fill();
  /* Size of the sample to be generated: C(k, n) */
  const UnsignedLong size(static_cast< UnsignedLong >(round(exp(SpecFunc::LogGamma(n_ + 1) - SpecFunc::LogGamma(n_ - k_ + 1)))));
  IndicesCollection allKPermutations(size, indices);
  /* First, generate all the permutations of k integers */
  IndicesCollection allPermutations(static_cast< UnsignedLong >(round(exp(SpecFunc::LogGamma(k_ + 1)))), indices);
  UnsignedLong flatIndex(1);
  while (std::next_permutation(indices.begin(), indices.end()))
    {
      std::copy(indices.begin(), indices.end(), allPermutations[flatIndex].begin());
      ++flatIndex;
    }
  /* Quick return if k == n */
  if (k_ == n_) return allPermutations;
  /* Second, generate all the combinations of k out of n elements */
  IndicesCollection allCombinations(Combinations(k_, n_).generate());
  flatIndex = 0;
  const UnsignedLong combinationSize(allCombinations.getSize());
  const UnsignedLong permutationSize(allPermutations.getSize());
  for (UnsignedLong i = 0; i < combinationSize; ++i)
    {
      /* Base combination */
      const Indices & combination(allCombinations[i]);
      /* Generate all the permutations of the base combination */
      for (UnsignedLong j = 0; j < permutationSize; ++j)
        {
          /* Current permutation */
          const Indices & permutation(allPermutations[j]);
          for (UnsignedLong k = 0; k < k_; ++k) allKPermutations[flatIndex][k] = combination[permutation[k]];
          ++flatIndex;
        }
    }
  return allKPermutations;
} // generate()

/* String converter */
String KPermutations::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName()
      << " k=" << k_
      << " n=" << n_;
  return oss;
}

/* Subset size accessor */
void KPermutations::setK(const UnsignedLong k)
{
  k_ = k;
}

UnsignedLong KPermutations::getK() const
{
  return k_;
}

/* Set size accessor */
void KPermutations::setN(const UnsignedLong n)
{
  n_ = n;
}

UnsignedLong KPermutations::getN() const
{
  return n_;
}

END_NAMESPACE_OPENTURNS
