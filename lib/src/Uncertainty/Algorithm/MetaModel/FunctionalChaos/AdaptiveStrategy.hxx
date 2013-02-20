//                                               -*- C++ -*-
/**
 *  @file  AdaptiveStrategy.hxx
 *  @brief This is the interface class for adaptive strategies
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
#ifndef OPENTURNS_ADAPTIVESTRATEGY_HXX
#define OPENTURNS_ADAPTIVESTRATEGY_HXX

#include "TypedInterfaceObject.hxx"
#include "AdaptiveStrategyImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class AdaptiveStrategy
 *
 * This is the interface class for adaptive strategies
 */

class AdaptiveStrategy
  : public TypedInterfaceObject<AdaptiveStrategyImplementation>
{
  CLASSNAME;

public:

  typedef Collection<NumericalMathFunction> NumericalMathFunctionCollection;

  /** Constructor from an orthogonal basis */
  AdaptiveStrategy();

  /** Constructor from an orthogonal basis */
  AdaptiveStrategy(const OrthogonalBasis & basis,
                   const UnsignedLong maximumDimension);

  /** Constructor from implementation */
  AdaptiveStrategy(const AdaptiveStrategyImplementation & implementation);

  /** Maximum dimension accessor */
  void setMaximumDimension(const UnsignedLong maximumDimension);
  UnsignedLong getMaximumDimension() const;

  /** Compute initial basis for the approximation */
  void computeInitialBasis();

  /** Update the basis for the next iteration of approximation */
  void updateBasis(const NumericalPoint & alpha_k_p_,
                   const NumericalScalar residual_p_,
                   const NumericalScalar relativeError_p_);

  /** Basis accessor */
  OrthogonalBasis getBasis() const;

  /** Psi accessor */
  NumericalMathFunctionCollection getPsi() const;

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

protected:

private:

} ; /* class AdaptiveStrategy */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ADAPTIVESTRATEGY_HXX */
