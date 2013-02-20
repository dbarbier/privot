//                                               -*- C++ -*-
/**
 *  @file  OrthogonalDirection.hxx
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 *  Id      OrthogonalDirection.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_ORTHOGONALDIRECTION_HXX
#define OPENTURNS_ORTHOGONALDIRECTION_HXX

#include "SamplingStrategyImplementation.hxx"
#include "Indices.hxx"
#include "Matrix.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class OrthogonalDirection
 */

class OrthogonalDirection :
  public SamplingStrategyImplementation
{

  CLASSNAME;

public:


  /** Default constructor */
  explicit OrthogonalDirection();

  /** Constructor with parameters */
  OrthogonalDirection(const UnsignedLong dimension,
                      const UnsignedLong size);


  /** Virtual constructor */
  virtual OrthogonalDirection * clone() const;

  /** Generate a set of directions */
  NumericalSample generate() const;

  /** String converter */
  String __repr__() const;

  /** Generate a random realization of an orientation matrix in SO(dimension) uniformly distributed relatively to the Haar mesure of SO(dimension) */
  Matrix getUniformOrientationRealization() const;

private:

  /** Generate the next permutation of indices in-place in the size_ first elements */
  void nextCombination(Indices & indices) const;

  /** Add the 2^size linear combinations of columns of Q indicated in the
   * size first elements of indices by affecting all the choices of sign
   * to the coefficients of the linear combination */
  void computePartialSample(const Indices & indices,
                            const Matrix & Q,
                            NumericalSample & result) const;
  /** Size of the direction subset */
  UnsignedLong size_;

} ; /* class OrthogonalDirection */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ORTHOGONALDIRECTION_HXX */
