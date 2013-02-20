//                                               -*- C++ -*-
/**
 *  @file  RandomizedQuasiMonteCarlo.hxx
 *  @brief Monte Carlo simulation using low-discrepancy sequences
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
 *  @date   2007-05-02 14:59:25 +0200 (Wed, 02 May 2007)
 *  Id      RandomizedQuasiMonteCarlo.hxx 424 2007-05-02 12:59:25Z dutka
 */
#ifndef OPENTURNS_RANDOMIZEDQUASIMONTECARLO_HXX
#define OPENTURNS_RANDOMIZEDQUASIMONTECARLO_HXX

#include "Simulation.hxx"
#include "LowDiscrepancySequence.hxx"
#include "SobolSequence.hxx"
#include "Collection.hxx"
#include "PersistentCollection.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class RandomizedQuasiMonteCarlo
 */

class RandomizedQuasiMonteCarlo :
  public Simulation
{
  CLASSNAME;

public:
  typedef PersistentCollection<Distribution> PersistentMarginals;
  typedef Collection<Distribution>           Marginals;

  /** Default constructor */
  RandomizedQuasiMonteCarlo();

  /** Constructor with parameters */
  RandomizedQuasiMonteCarlo(const Event & event,
                            const LowDiscrepancySequence & lowDiscrepancySequence = SobolSequence());

  /** Virtual constructor */
  virtual RandomizedQuasiMonteCarlo * clone() const;

  /** String converter */
  String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:
  /** Compute the block sample */
  NumericalSample computeBlockSample();

protected:
  /** Antecedent dimension */
  UnsignedLong dimension_;

  /** Sequence used to generate the low discrepancy points */
  LowDiscrepancySequence lowDiscrepancySequence_;

  /** Marginal distributions */
  PersistentMarginals marginals_;

} ; /* class RandomizedQuasiMonteCarlo */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_RANDOMIZEDQUASIMONTECARLO_HXX */
