//                                               -*- C++ -*-
/**
 *  @file  SequentialStrategy.hxx
 *  @brief An implementation returning the set of polynomials in sequence
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
 *  @author dutka
 *  @date   2008-05-21 17:44:02 +0200 (Wed, 21 May 2008)
 *  Id      Object.hxx 818 2008-05-21 15:44:02Z dutka
 */
#ifndef OPENTURNS_SEQUENTIALSTRATEGY_HXX
#define OPENTURNS_SEQUENTIALSTRATEGY_HXX

#include "AdaptiveStrategyImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class SequentialStrategy
 *
 * An implementation returning the set of polynomials in sequence
 */

class SequentialStrategy
  : public AdaptiveStrategyImplementation
{
  CLASSNAME;
public:


  /** Constructor from an orthogonal basis */
  SequentialStrategy(const OrthogonalBasis & basis,
                     const UnsignedLong maximumDimension,
                     const Bool verbose = false);

  /** Virtual constructor */
  virtual SequentialStrategy * clone() const;

  /** Compute initial basis for the approximation */
  void computeInitialBasis();

  /** Update the basis for the next iteration of approximation */
  void updateBasis(const NumericalPoint & alpha_k_p_,
                   const NumericalScalar residual_p_,
                   const NumericalScalar relativeError_p_);

  /** String converter */
  virtual String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);


protected:

  friend class Factory<SequentialStrategy>;

  /** Default constructor */
  SequentialStrategy();

private:
  // Index of the next vector to be generated
  UnsignedLong currentVectorIndex_;

  // Verbose
  Bool verbose_;

} ; /* class SequentialStrategy */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SEQUENTIALSTRATEGY_HXX */
