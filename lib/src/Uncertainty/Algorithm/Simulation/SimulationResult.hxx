//                                               -*- C++ -*-
/**
 *  @file  SimulationResult.hxx
 *  @brief SimulationResult stores the simulation result
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
 *  Id      SimulationResult.hxx 821 2008-05-23 11:46:12Z schueller
 */
#ifndef OPENTURNS_SIMULATIONRESULT_HXX
#define OPENTURNS_SIMULATIONRESULT_HXX

#include "SimulationResultImplementation.hxx"
#include "Pointer.hxx"
#include "TypedInterfaceObject.hxx"
#include "NumericalPointWithDescription.hxx"
#include "Graph.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class SimulationResult
 * SimulationResult stores the simulation result
 */
class SimulationResult
  : public TypedInterfaceObject<SimulationResultImplementation>
{

  CLASSNAME;
public:

  typedef Pointer<SimulationResultImplementation>                       Implementation;

  /** Default constructor */
  explicit SimulationResult();

  /** Standard constructor */
  explicit SimulationResult(const Event & event,
                            const NumericalScalar probabilityEstimate,
                            const NumericalScalar varianceEstimate,
                            const UnsignedLong outerSampling,
                            const UnsignedLong blockSize);

  /** Constructor with parameters */
  SimulationResult(const SimulationResultImplementation & implementation);

  /** Constructor with parameters */
  SimulationResult(Implementation & p_implementation);

  /** Event accessor */
  Event getEvent() const;

  /** Probability estimate accessor */
  NumericalScalar getProbabilityEstimate() const;
  void setProbabilityEstimate(const NumericalScalar probabilityEstimate);

  /** Variance estimate accessor */
  NumericalScalar getVarianceEstimate() const;
  void setVarianceEstimate(const NumericalScalar varianceEstimate);

  /** Coefficient of variation estimate accessor */
  NumericalScalar getCoefficientOfVariation() const;

  /** Standard deviation estimate accessor */
  NumericalScalar getStandardDeviation() const;

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
  NumericalScalar getConfidenceLength(const NumericalScalar level = ResourceMap::GetAsNumericalScalar( "SimulationResultImplementation-DefaultConfidenceLevel" )) const;

  /** String converter */
  String __repr__() const;

private:

}; // class Result

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SIMULATIONRESULT_HXX */
