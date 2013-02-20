//                                               -*- C++ -*-
/**
 *  @file  MonteCarloExperiment.hxx
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
 *  Id      MonteCarloExperiment.hxx 862 2008-06-26 11:50:17Z dutka
 */
#ifndef OPENTURNS_MONTECARLOEXPERIMENT_HXX
#define OPENTURNS_MONTECARLOEXPERIMENT_HXX

#include "WeightedExperiment.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class MonteCarloExperiment
 *
 * The class describes the probabilistic concept of monteCarloExperiment plan
 */
class MonteCarloExperiment
  : public WeightedExperiment
{
  CLASSNAME;
public:


  /** Default constructor */
  explicit MonteCarloExperiment(const String & name = DefaultName);

  /** Parameters constructor */
  explicit MonteCarloExperiment(const UnsignedLong size,
                                const String & name = DefaultName);

  /** Parameters constructor */
  explicit MonteCarloExperiment(const Distribution & distribution,
                                const UnsignedLong size,
                                const String & name = DefaultName);

  /** Virtual constructor */
  virtual MonteCarloExperiment * clone() const;

  /** String converter */
  virtual String __repr__() const;

  /* Here is the interface that all derived class must implement */

  /** Sample generation */
  using WeightedExperiment::generate;
  NumericalSample generate();

protected:

private:

}; /* class MonteCarloExperiment */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_MONTECARLOEXPERIMENT_HXX */
