//                                               -*- C++ -*-
/**
 *  @file  IntegrationStrategy.hxx
 *  @brief An implementation of projection strategy as a leastSquares
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
#ifndef OPENTURNS_INTEGRATIONSTRATEGY_HXX
#define OPENTURNS_INTEGRATIONSTRATEGY_HXX

#include "ProjectionStrategyImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class IntegrationStrategy
 *
 * An implementation of projection strategy as a leastSquares
 */

class IntegrationStrategy
  : public ProjectionStrategyImplementation
{
  CLASSNAME;
public:


  // friend class Factory<IntegrationStrategy>;

  /** Default constructor */
  IntegrationStrategy();

  /** Parameter constructor */
  IntegrationStrategy(const Distribution & measure);

  /** Parameter constructor */
  IntegrationStrategy(const WeightedExperiment & weightedExperiment);

  /** Parameter constructor */
  IntegrationStrategy(const Distribution & measure,
                      const WeightedExperiment & weightedExperiment);

  /** Parameter constructor */
  IntegrationStrategy(const NumericalSample & inputSample,
                      const NumericalPoint & weights,
                      const NumericalSample & outputSample);

  /** Parameter constructor */
  IntegrationStrategy(const NumericalSample & inputSample,
                      const NumericalSample & outputSample);

  /** Virtual constructor */
  virtual IntegrationStrategy * clone() const;

  /** String converter */
  String __repr__() const;

  /** Compute the components alpha_k_p_ by projecting the model on the partial L2 basis */
  void computeCoefficients(const NumericalMathFunction & function,
                           const NumericalMathFunctionCollection & partialBasis,
                           const Indices & indices,
                           const Indices & addedRanks,
                           const Indices & conservedRanks,
                           const Indices & removedRanks,
                           const UnsignedLong marginalIndex = 0);

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


protected:

private:

} ; /* class IntegrationStrategy */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_INTEGRATIONSTRATEGY_HXX */
