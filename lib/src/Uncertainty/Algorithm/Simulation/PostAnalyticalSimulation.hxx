//                                               -*- C++ -*-
/**
 *  @file  PostAnalyticalSimulation.hxx
 *  @brief PostAnalyticalSimulation is a generic view of postAnalyticalSimulation methods for computing
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
 *  @author lebrun
 *  @date   2008-05-16 12:56:34 +0200 (ven, 16 mai 2008)
 *  Id      PostAnalyticalSimulation.hxx 808 2008-05-16 10:56:34Z lebrun
 */
#ifndef OPENTURNS_POSTANALYTICALSIMULATION_HXX
#define OPENTURNS_POSTANALYTICALSIMULATION_HXX

#include "Simulation.hxx"
#include "AnalyticalResult.hxx"
#include "StandardEvent.hxx"
#include "Distribution.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class PostAnalyticalSimulation
 */

class PostAnalyticalSimulation
  : public Simulation
{

  CLASSNAME;
public:


  /** Constructor with parameters */
  explicit PostAnalyticalSimulation(const AnalyticalResult & analyticalResult);

  /** Event accessor */
  AnalyticalResult getAnalyticalResult() const;

  /** Control probability accessor */
  NumericalScalar getControlProbability() const;

  /** String converter */
  String __repr__() const;

protected:

  /** FORM result upon which one compute a correction */
  AnalyticalResult analyticalResult_;

  /** Standard event associated to analytical result */
  StandardEvent standardEvent_;

  /** Standard distribution associated to the standard event */
  Distribution standardDistribution_;

  /** Probability associated to the control */
  NumericalScalar controlProbability_;

} ; /* class PostAnalyticalSimulation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_POSTANALYTICALSIMULATION_HXX */
