//                                               -*- C++ -*-
/**
 *  @file  AdaptiveStrategyImplementation.hxx
 *  @brief This is a abstract class for adaptive strategy implementations
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
#ifndef OPENTURNS_ADAPTIVESTRATEGYIMPLEMENTATION_HXX
#define OPENTURNS_ADAPTIVESTRATEGYIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "OrthogonalBasis.hxx"
#include "NumericalPoint.hxx"
#include "Indices.hxx"
#include "Collection.hxx"
#include "NumericalMathFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class AdaptiveStrategyImplementation
 *
 * This is a abstract class for adaptive strategy implementations
 */

class AdaptiveStrategyImplementation
  : public PersistentObject
{
  CLASSNAME;
  // Make the FunctionalChaosAlgorithm class a friend of the AdaptiveStrategyImplementation class
  // as this last class is in charge of managing data that are part of the FunctionalChaosAlgorithm
  // state
  friend class FunctionalChaosAlgorithm;
  // Make the ProjectionStrategyImplementation class a friend of the AdaptiveStrategyImplementation class
  // as this last class is in charge of data needed by the ProjectionStrategyImplementation to update the state of the FunctionalChaosAlgorithm class
  friend class ProjectionStrategyImplementation;

public:

  typedef Collection<NumericalMathFunction> NumericalMathFunctionCollection;
  /** Default constructor */
  AdaptiveStrategyImplementation();

  /** Constructor from an orthogonal basis */
  AdaptiveStrategyImplementation(const OrthogonalBasis & basis,
                                 const UnsignedLong maximumDimension);

  /** Virtual constructor */
  virtual AdaptiveStrategyImplementation * clone() const;


  /** Maximum dimension accessor */
  void setMaximumDimension(const UnsignedLong maximumDimension);
  UnsignedLong getMaximumDimension() const;

  /** Compute initial basis for the approximation */
  virtual void computeInitialBasis();

  /** Update the basis for the next iteration of approximation */
  virtual void updateBasis(const NumericalPoint & alpha_k_p_,
                           const NumericalScalar residual_p_,
                           const NumericalScalar relativeError_p_);

  /** String converter */
  virtual String __repr__() const;

  /** Basis accessor */
  OrthogonalBasis getBasis() const;

  /** Psi accessor */
  NumericalMathFunctionCollection getPsi() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

  friend class Factory<AdaptiveStrategyImplementation>;

  /** The orthogonal basis */
  OrthogonalBasis basis_;

  /** Maximum dimension of the basis to be generated */
  UnsignedLong maximumDimension_;

  /** The collection of indices of the vectors in the partial basis with respect to the orthogonal basis */
  Indices I_p_;

  /** The ranks of the added vectors in the current partial basis */
  Indices addedPsi_k_ranks_;

  /** The ranks of the vectors common between the previous partial basis and the current partial basis, as found in the previous partial basis */
  Indices conservedPsi_k_ranks_;

  /** The ranks of the removed vectors in the previous partial basis */
  Indices removedPsi_k_ranks_;

  /** The collection of vectors upon which we project the (composed) model */
  NumericalMathFunctionCollection Psi_k_p_;

private:

} ; /* class AdaptiveStrategyImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ADAPTIVESTRATEGYIMPLEMENTATION_HXX */
