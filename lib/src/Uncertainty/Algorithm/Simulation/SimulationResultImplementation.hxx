//                                               -*- C++ -*-
/**
 *  @file  SimulationResultImplementation.hxx
 *  @brief Implementation of SimulationResult
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
 *  @date   2008-05-23 13:46:12 +0200 (Fri, 23 May 2008)
 *  Id      SimulationResultImplementation.hxx 821 2008-05-23 11:46:12Z schueller
 */
#ifndef OPENTURNS_SIMULATIONRESULTIMPLEMENTATION_HXX
#define OPENTURNS_SIMULATIONRESULTIMPLEMENTATION_HXX

#include <cmath>
#include "PersistentObject.hxx"
#include "Event.hxx"
#include "ResourceMap.hxx"
#include "NumericalPoint.hxx"
#include "NumericalPointWithDescription.hxx"
#include "Graph.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class SimulationResultImplementation
 * Implementation of SimulationResult
 */
class SimulationResultImplementation
  : public PersistentObject
{

  CLASSNAME;
public:


  /** Default constructor */
  explicit SimulationResultImplementation();

  /** Standard constructor */
  explicit SimulationResultImplementation(const Event & event,
                                          const NumericalScalar probabilityEstimate,
                                          const NumericalScalar varianceEstimate,
                                          const UnsignedLong outerSampling,
                                          const UnsignedLong blockSize);

  /** Virtual constructor */
  virtual SimulationResultImplementation * clone() const;

  /** Event accessor */
  Event getEvent() const;
  void setEvent(const Event & event);

  /** Probability estimate accessor */
  NumericalScalar getProbabilityEstimate() const;
  void setProbabilityEstimate(const NumericalScalar probabilityEstimate);

  /** Variance estimate accessor */
  NumericalScalar getVarianceEstimate() const;
  void setVarianceEstimate(const NumericalScalar varianceEstimate);

  /** Coefficient of variation estimate accessor */
  virtual NumericalScalar getCoefficientOfVariation() const;

  /** Standard deviation estimate accessor */
  virtual NumericalScalar getStandardDeviation() const;

  /** Mean point conditioned to the event realization accessor */
  NumericalPoint getMeanPointInEventDomain() const;

  /** Importance factors accessor */
  NumericalPointWithDescription getImportanceFactors() const;

  /** Draw the importance factors */
  Graph drawImportanceFactors() const;

  /** Sample size accessor */
  UnsignedLong getOuterSampling() const;
  void setOuterSampling(const UnsignedLong outerSampling);

  /** Block size accessor */
  UnsignedLong getBlockSize() const;
  void setBlockSize(const UnsignedLong blockSize);

  /** Confidence length */
  virtual NumericalScalar getConfidenceLength(const NumericalScalar level = ResourceMap::GetAsNumericalScalar( "SimulationResultImplementation-DefaultConfidenceLevel" )) const;

  /** String converter */
  virtual String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

  /* The event we are interested in */
  Event event_;
  NumericalScalar probabilityEstimate_;
  NumericalScalar varianceEstimate_;
  UnsignedLong outerSampling_;
  UnsignedLong blockSize_;

}; // class SimulationResultImplementation

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SIMULATIONRESULTIMPLEMENTATION_HXX */
