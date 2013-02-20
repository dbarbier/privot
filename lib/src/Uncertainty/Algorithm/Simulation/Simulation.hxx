//                                               -*- C++ -*-
/**
 *  @file  Simulation.hxx
 *  @brief Simulation is a generic view of simulation methods for computing
 * probabilities and related quantities by sampling and estimation
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
 *  Id      Simulation.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_SIMULATION_HXX
#define OPENTURNS_SIMULATION_HXX

#include "PersistentObject.hxx"
#include "HistoryStrategy.hxx"
#include "Compact.hxx"
#include "Last.hxx"
#include "Graph.hxx"
#include "SimulationResult.hxx"
#include "Event.hxx"


BEGIN_NAMESPACE_OPENTURNS




/**
 * @class Simulation
 */

class Simulation
  : public PersistentObject
{


  CLASSNAME;
public:


  /** Constructor with parameters */
  explicit Simulation(const Event & event,
                      const Bool verbose = true,
                      const HistoryStrategy & convergenceStrategy = Compact());

  /** Event accessor */
  Event getEvent() const;

  /** Result accessor */
  SimulationResult getResult() const;

  /** Maximum sample size accessor */
  void setMaximumOuterSampling(const UnsignedLong maximumOuterSampling);
  UnsignedLong getMaximumOuterSampling() const;

  /** Maximum coefficient of variation accessor */
  void setMaximumCoefficientOfVariation(const NumericalScalar maximumCoefficientOfVariation);
  NumericalScalar getMaximumCoefficientOfVariation() const;

  /** Maximum standard deviation accessor */
  void setMaximumStandardDeviation(const NumericalScalar maximumStandardDeviation);
  NumericalScalar getMaximumStandardDeviation() const;

  /** Block size accessor */
  void setBlockSize(const UnsignedLong blockSize);
  UnsignedLong getBlockSize() const;

  /** Verbosity accessor */
  void setVerbose(const Bool verbose);
  Bool getVerbose() const;

  /** Convergence strategy accessor */
  void setConvergenceStrategy(const HistoryStrategy & convergenceStrategy);
  HistoryStrategy getConvergenceStrategy() const;

  /** String converter */
  String __repr__() const;

  /** Performs the actual computation. */
  virtual void run();

  /** Draw the probability convergence at the given level */
  Graph drawProbabilityConvergence(const NumericalScalar level = ResourceMap::GetAsNumericalScalar( "SimulationResultImplementation-DefaultConfidenceLevel" )) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  /** Result accessor */
  void setResult(const SimulationResult & result);

  /** Compute the block sample and the points that realized the event */
  virtual NumericalSample computeBlockSample() = 0;

protected:

  /** For save/load mechanism*/
  Simulation(const Bool verbose = true,
             const HistoryStrategy & convergenceStrategy = Compact()) {};

  friend class Factory<Simulation>;

  /** History strategy for the probability and variance estimate */
  HistoryStrategy convergenceStrategy_;

  // Size of the atomic blocks of computation
  UnsignedLong blockSize_;

  // The event we are computing the probability of
  Event event_;

  // Result of the simulation
  SimulationResult result_;

private:

  // Maximum number of outer iteration allowed
  UnsignedLong maximumOuterSampling_;

  // Maximum coefficient of variation allowed for convergence
  NumericalScalar maximumCoefficientOfVariation_;

  // Maximum standard deviation allowed for convergence
  NumericalScalar maximumStandardDeviation_;

  // Do we have to echo all the intermediate results?
  Bool verbose_;

} ; /* class Simulation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SIMULATION_HXX */
