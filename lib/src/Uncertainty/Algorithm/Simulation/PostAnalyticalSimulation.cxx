//                                               -*- C++ -*-
/**
 *  @file  PostAnalyticalSimulation.cxx
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
 *  @date   2008-06-04 11:47:22 +0200 (mer, 04 jun 2008)
 */
#include "PostAnalyticalSimulation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/*
 * @class PostAnalyticalSimulation
 */

CLASSNAMEINIT(PostAnalyticalSimulation);

/* Constructor with parameters */
PostAnalyticalSimulation::PostAnalyticalSimulation(const AnalyticalResult & analyticalResult)
  : Simulation(analyticalResult.getLimitStateVariable()),
    analyticalResult_(analyticalResult),
    standardEvent_(StandardEvent(getEvent())),
    standardDistribution_(standardEvent_.getImplementation()->getAntecedent()->getDistribution())
{
  // Compute the probability associated to the analytical result
  controlProbability_ = standardDistribution_.getMarginal(0).computeCDF(-analyticalResult.getHasoferReliabilityIndex());
}

/* Analytical result accessor */
AnalyticalResult PostAnalyticalSimulation::getAnalyticalResult() const
{
  return analyticalResult_;
}

/* Control probability accessor */
NumericalScalar PostAnalyticalSimulation::getControlProbability() const
{
  return controlProbability_;
}

/* String converter */
String PostAnalyticalSimulation::__repr__() const
{
  OSS oss;
  oss << "class=" << PostAnalyticalSimulation::GetClassName()
      << " analyticalResult=" << analyticalResult_
      << " controlProbability_=" << controlProbability_;
  return oss;
}

END_NAMESPACE_OPENTURNS
