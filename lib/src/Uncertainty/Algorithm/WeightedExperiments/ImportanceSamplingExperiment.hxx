//                                               -*- C++ -*-
/**
 *  @file  ImportanceSamplingExperiment.hxx
 *  @brief Abstract top-level view of an importanceSamplingExperiment plane
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
 *  Id      ImportanceSamplingExperiment.hxx 862 2008-06-26 11:50:17Z dutka
 */
#ifndef OPENTURNS_IMPORTANCESAMPLINGEXPERIMENT_HXX
#define OPENTURNS_IMPORTANCESAMPLINGEXPERIMENT_HXX

#include "WeightedExperiment.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class ImportanceSamplingExperiment
 *
 * The class describes the probabilistic concept of importanceSamplingExperiment plan
 */
class ImportanceSamplingExperiment
  : public WeightedExperiment
{
  CLASSNAME;
public:


  /** Default constructor */
  explicit ImportanceSamplingExperiment(const String & name = DefaultName);

  /** Parameters constructor */
  explicit ImportanceSamplingExperiment(const Distribution & importanceDistribution,
                                        const UnsignedLong size,
                                        const String & name = DefaultName);

  /** Parameters constructor */
  explicit ImportanceSamplingExperiment(const Distribution & distribution,
                                        const Distribution & importanceDistribution,
                                        const UnsignedLong size,
                                        const String & name = DefaultName);

  /** Virtual constructor */
  virtual ImportanceSamplingExperiment * clone() const;

  /** Importance distribution accessor */
  Distribution getImportanceDistribution() const;

  /** String converter */
  virtual String __repr__() const;

  /* Here is the interface that all derived class must implement */

  /** Sample generation */
  NumericalSample generate();
  NumericalSample generate(NumericalPoint & weights);

protected:

private:
  // The importance distribution according to which the sample is built
  Distribution importanceDistribution_;

}; /* class ImportanceSamplingExperiment */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_IMPORTANCESAMPLINGEXPERIMENT_HXX */
