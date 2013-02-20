//                                               -*- C++ -*-
/**
 *  @file  LowDiscrepancyExperiment.hxx
 *  @brief Abstract top-level view of an monteCarloExperiment plane
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
 *  @date   2008-06-26 13:50:17 +0200 (jeu, 26 jun 2008)
 *  Id      LowDiscrepancyExperiment.hxx 862 2008-06-26 11:50:17Z dutka
 */
#ifndef OPENTURNS_LOWDISCREPANCYEXPERIMENT_HXX
#define OPENTURNS_LOWDISCREPANCYEXPERIMENT_HXX

#include "WeightedExperiment.hxx"
#include "LowDiscrepancySequence.hxx"
#include "Collection.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class LowDiscrepancyExperiment
 *
 * The class describes the probabilistic concept of monteCarloExperiment plan
 */
class LowDiscrepancyExperiment
  : public WeightedExperiment
{
  CLASSNAME;
public:

  typedef Collection<Distribution> DistributionCollection;

  /** Default constructor */
  explicit LowDiscrepancyExperiment(const String & name = DefaultName);

  /** Parameters constructor */
  explicit LowDiscrepancyExperiment(const UnsignedLong size,
                                    const String & name = DefaultName);

  /** Parameters constructor */
  explicit LowDiscrepancyExperiment(const LowDiscrepancySequence & sequence,
                                    const UnsignedLong size,
                                    const String & name = DefaultName);

  /** Parameters constructor */
  explicit LowDiscrepancyExperiment(const LowDiscrepancySequence & sequence,
                                    const Distribution & distribution,
                                    const UnsignedLong size,
                                    const String & name = DefaultName);

  /** Virtual constructor */
  virtual LowDiscrepancyExperiment * clone() const;

  /** String converter */
  virtual String __repr__() const;

  /** Distribution accessor */
  virtual void setDistribution(const Distribution & distribution);

  /** Sequence accessor */
  LowDiscrepancySequence getSequence() const;

  /* Here is the interface that all derived class must implement */

  /** Sample generation */
  using WeightedExperiment::generate;
  NumericalSample generate();

protected:

private:
  // Marginal distributions
  DistributionCollection marginals_;

  // Low discrepancy sequence
  LowDiscrepancySequence sequence_;

}; /* class LowDiscrepancyExperiment */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LOWDISCREPANCYEXPERIMENT_HXX */
