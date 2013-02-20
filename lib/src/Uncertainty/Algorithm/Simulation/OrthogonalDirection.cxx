//                                               -*- C++ -*-
/**
 *  @file  OrthogonalDirection.cxx
 *  @brief
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#include <cmath>
#include "OrthogonalDirection.hxx"
#include "Log.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class OrthogonalDirection
 */

CLASSNAMEINIT(OrthogonalDirection);

/* Default constructor */
OrthogonalDirection::OrthogonalDirection():
  SamplingStrategyImplementation(0),
  size_(1)
{
  // Nothing to do
}

/* Constructor with parameters */
OrthogonalDirection::OrthogonalDirection(const UnsignedLong dimension,
                                         const UnsignedLong size):
  SamplingStrategyImplementation(dimension),
  size_(size)
{
  // Nothing to do
}

/* Virtual constructor */
OrthogonalDirection * OrthogonalDirection::clone() const
{
  return new OrthogonalDirection(*this);
}

/* Generate the next permutation of indices in-place in the size_ first elements */
void OrthogonalDirection::nextCombination(Indices & indices) const
{
  UnsignedLong i(size_ - 1);
  while (indices[i] == dimension_ - size_ + i) --i;
  ++indices[i];
  for (UnsignedLong j = i + 1; j < size_; ++j) indices[j] = indices[i] + j - i;
}

/* Generate a random realization of an orientation matrix in SO(dimension) uniformly
   distributed relatively to the Haar mesure of SO(dimension).
   The algorithm generate an element of SO(n) with the desired properties from an
   element of SO(n-1) by the application of a Householder reflexion associated to a
   uniform random vector on the hypersphere Sn. The starting transformation on
   SO(1) = {-1, 1} is taken equal to the identity Id or its opposite according to the
   parity of dimension in order to generate elements of SO(dimension) and not of
   O(dimension) \ SO(dimension). For an explanation of why it works, please refer to:
   Francesco Mezzadri, "How to Generate Random Matrices from the Classical Compact Groups",
   notices of the AMS, Volume 54, Number 5., May 2007, available online at:
   https://www.ams.org/journals/notices/200705/fea-mezzadri-web.pdf
*/
Matrix OrthogonalDirection::getUniformOrientationRealization() const
{
  Matrix Q(dimension_, dimension_);
  // Initialization according to the parity of dimension
  Q(0, 0) = (dimension_ % 2 == 0 ? -1.0 : 1.0);
  Matrix column(dimension_, 1);
  for (UnsignedLong indexDimension = 1; indexDimension < dimension_; indexDimension++)
    {
      Q(indexDimension, indexDimension) = 1.0;
      NumericalPoint v(getUniformUnitVectorRealization(indexDimension + 1));
      for (UnsignedLong index = 0; index <= indexDimension; ++index) column(index, 0) = v[index];
      Q = Q - ((2.0 * column) * (column.transpose() * Q));
    }
  return Q;
}

/* Add the 2^size linear combinations of columns of Q indicated in the
 * size first elements of indices by affecting all the choices of sign
 * to the coefficients of the linear combination */
void OrthogonalDirection::computePartialSample(const Indices & indices,
                                               const Matrix & Q,
                                               NumericalSample & result) const
{
  // Normalization factor of the linear combination
  NumericalScalar factor(1.0 / sqrt(size_));
  // We have 2^size linear combinations to generate
  UnsignedLong indexLinearCombinationMax(2 << size_);
  // For each combination
  for (UnsignedLong indexLinearCombination = 0; indexLinearCombination < indexLinearCombinationMax; ++indexLinearCombination)
    {
      NumericalPoint direction(dimension_);
      // The combination index is used as a mask to select the coefficients equal to 1.0 or to -1.0
      UnsignedLong mask(indexLinearCombination);
      for (UnsignedLong index = 0; index < size_; ++index)
        {
          // Which column of Q corresponds to the index position of indices?
          UnsignedLong column(indices[index]);
          // Sign affected to this column
          NumericalScalar sign(1.0 - 2.0 * (mask % 2));
          // Summation
          for (UnsignedLong row = 0; row < dimension_; ++row) direction[row] += sign * Q(row, column);
          // Next bit of the mask
          mask /= 2;
        }
      // Normalize the linear combination
      result.add(factor * direction);
    }
}

/* Generate a set of directions */
NumericalSample OrthogonalDirection::generate() const
{
  NumericalSample result(0, dimension_);
  Matrix Q(getUniformOrientationRealization());
  Indices indices(dimension_);
  // Start with the first lexicographic combination
  for (UnsignedLong i = 0; i < dimension_; ++i) indices[i] = i;
  computePartialSample(indices, Q, result);
  while(indices[0] != dimension_ - size_)
    {
      nextCombination(indices);
      computePartialSample(indices, Q, result);
    }
  LOGDEBUG(OSS() << "OrthogonalDirection::generate: directions=\n" << result);
  return result;
}

/* String converter */
String OrthogonalDirection::__repr__() const
{
  OSS oss;
  oss << "class=" << OrthogonalDirection::GetClassName()
      << " derived from " << SamplingStrategyImplementation::__repr__()
      << " size=" << size_;
  return oss;
}

END_NAMESPACE_OPENTURNS

