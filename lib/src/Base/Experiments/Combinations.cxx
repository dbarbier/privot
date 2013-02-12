//                                               -*- C++ -*-
/**
 *  @file  Combinations.cxx
 *  @brief Implementation of the combinations experiment plane
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
#include "OTprivate.hxx"
#include "Combinations.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Combinations);

/* Default constructor */
Combinations::Combinations()
  : CombinatorialGeneratorImplementation()
  , k_(0)
  , n_(0)
{
  // Nothing to do
}

/* Constructor with parameters */
Combinations::Combinations(const UnsignedLong k,
                           const UnsignedLong n)
  : CombinatorialGeneratorImplementation()
  , k_(k)
  , n_(n)
{
  // Nothing to do
}

/* Virtual constructor */
Combinations * Combinations::clone() const
{
  return new Combinations(*this);
}

/* Experiment plane generation :
 *  all the combinations of k elements amongst {0, ..., n-1}
 */
CombinatorialGeneratorImplementation::IndicesCollection Combinations::generate()
{
  /* Quick return for trivial cases */
  if (k_ > n_) return IndicesCollection(0, Indices(k_));
  if (k_ == 0) return IndicesCollection(1, Indices(0));
  Indices indices(k_);
  indices.fill();
  if (k_ == n_) return IndicesCollection(1, indices);
  /* Size of the sample to be generated: C(k, n) */
  const UnsignedLong size(static_cast< UnsignedLong >(round(exp(SpecFunc::LogGamma(n_ + 1) - SpecFunc::LogGamma(k_ + 1) - SpecFunc::LogGamma(n_ - k_ + 1)))));
  IndicesCollection allCombinations(size, indices);
  for (UnsignedLong flatIndex = 0; flatIndex < size; ++flatIndex)
    {
      allCombinations[flatIndex] = indices;
      /* Update the indices */
      UnsignedLong t(k_ - 1);
      while ((t != 0) && (indices[t] == n_ + t - k_)) --t;
      ++indices[t];
      for (UnsignedLong i = t + 1; i < k_; ++i) indices[i] = indices[i - 1] + 1;
    }
  return allCombinations;
} // generate()

/* String converter */
String Combinations::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName()
      << " k=" << k_
      << " n=" << n_;
  return oss;
}

/* Subset size accessor */
void Combinations::setK(const UnsignedLong k)
{
  k_ = k;
}

UnsignedLong Combinations::getK() const
{
  return k_;
}

/* Set size accessor */
void Combinations::setN(const UnsignedLong n)
{
  n_ = n;
}

UnsignedLong Combinations::getN() const
{
  return n_;
}

END_NAMESPACE_OPENTURNS
