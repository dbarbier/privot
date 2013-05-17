//                                               -*- C++ -*-
/**
 *  @file  RandomWalkMetropolisHastings.hxx
 *  @brief RandomWalkMetropolisHastings updates the chain
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
 *  Id      RandomWalkMetropolisHastings.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_RANDOMWALKMETROPOLISHASTINGS_HXX
#define OPENTURNS_RANDOMWALKMETROPOLISHASTINGS_HXX

#include "OTprivate.hxx"
#include "MCMC.hxx"
#include "CalibrationStrategy.hxx"
#include "Interval.hxx"
#include "ResourceMap.hxx"


BEGIN_NAMESPACE_OPENTURNS




/**
 * @class RandomWalkMetropolisHastings
 *
 * @brief The class implements the concept of comparison operator defined in Sampler.
 *
 * @see Sampler
 */
class RandomWalkMetropolisHastings
  : public MCMC
{
  CLASSNAME;
public:
  typedef Collection<Distribution> DistributionCollection;
  typedef PersistentCollection<Distribution> DistributionPersistentCollection;
  typedef Collection<CalibrationStrategy> CalibrationStrategyCollection;
  typedef PersistentCollection<CalibrationStrategy> CalibrationStrategyPersistentCollection;

  /** Default constructor */
  RandomWalkMetropolisHastings();

  /** Constructor with parameters*/
  RandomWalkMetropolisHastings(const Distribution & prior,
                               const Distribution & conditional,
                               const NumericalSample & observations,
                               const NumericalPoint & initialState,
                               const DistributionCollection & proposal);

  /** Constructor with parameters*/
  RandomWalkMetropolisHastings(const Distribution & prior,
                               const Distribution & conditional,
                               const NumericalMathFunction & model,
                               const NumericalSample & observations,
                               const NumericalPoint & initialState,
                               const DistributionCollection & proposal);

  /** String converter */
  virtual String __repr__() const;

  /* Here is the interface that all derived class must implement */

  /** Virtual constructor */
  virtual RandomWalkMetropolisHastings * clone() const;

  /** @copydoc Sampler::getRealization() const */
  virtual NumericalPoint getRealization() const;

  /** Calibration strategy accessor */
  void setCalibrationStrategy(const CalibrationStrategy & calibrationStrategy);
  void setCalibrationStrategyPerComponent(const CalibrationStrategyCollection & calibrationStrategy);
  CalibrationStrategyCollection getCalibrationStrategyPerComponent() const;

  /** Proposal accessor */
  void setProposal(const DistributionCollection & proposal);
  DistributionCollection getProposal() const;

  /** Acceptance rate accessor*/
  NumericalPoint getAcceptanceRate() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:
  /// proposal densities of the markov chain
  DistributionPersistentCollection proposal_;

  /// how to calibrate the each chain component
  CalibrationStrategyPersistentCollection calibrationStrategy_;

  /// number of samples
  mutable UnsignedLong samplesNumber_;

  /// number of samples accepted
  mutable Indices acceptedNumber_;

}; /* class RandomWalkMetropolisHastings */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_RANDOMWALKMETROPOLISHASTINGS_HXX */