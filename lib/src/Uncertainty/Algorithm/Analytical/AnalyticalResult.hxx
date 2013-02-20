//                                               -*- C++ -*-
/**
 *  @file  AnalyticalResult.hxx
 *  @brief AnalyticalResult implements an algorithm to find the design point
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
 *  @date   2008-07-03 08:31:34 +0200 (jeu, 03 jui 2008)
 *  Id      AnalyticalResult.hxx 867 2008-07-03 06:31:34Z lebrun
 */
#ifndef OPENTURNS_ANALYTICALRESULT_HXX
#define OPENTURNS_ANALYTICALRESULT_HXX

#include "PersistentObject.hxx"
#include "NumericalPoint.hxx"
#include "NumericalPointWithDescription.hxx"
#include "Collection.hxx"
#include "Event.hxx"
#include "Graph.hxx"
#include "Description.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class AnalyticalResult
 * AnalyticalResult stores the Analytical result
 */
class AnalyticalResult
  : public PersistentObject
{
  CLASSNAME;
public:

  typedef Collection<Graph>                         GraphCollection;
  typedef Collection<NumericalPoint>                NumericalPointCollection;
  typedef Collection<NumericalPointWithDescription> NumericalPointWithDescriptionCollection;
  typedef NumericalPointWithDescriptionCollection   Sensitivity;

  /** Standard constructor */
  AnalyticalResult(const NumericalPoint & standardSpaceDesignPoint,
                   const Event & limitStateVariable,
                   const Bool isStandardPointOriginInFailureSpace,
                   const String & name = DefaultName);

  /* Default constructor (required by SWIG :-<) */
  explicit AnalyticalResult();

  /** Virtual constructor */
  virtual AnalyticalResult * clone() const;

  /** StandardSpaceDesignPoint accessor */
  NumericalPoint getStandardSpaceDesignPoint() const;

  /** PhysicalSpaceDesignPoint accessor */
  void setStandardSpaceDesignPoint(const NumericalPoint & standardSpaceDesignPoint);

  /** PhysicalSpaceDesignPoint accessor */
  NumericalPoint getPhysicalSpaceDesignPoint() const;

  /** LimitStateVariable accessor */
  Event getLimitStateVariable() const;

  /** IsStandardPointOriginInFailureSpace  accessor */
  Bool getIsStandardPointOriginInFailureSpace() const;

  /** IsStandardPointOriginInFailureSpace  accessor */
  void setIsStandardPointOriginInFailureSpace(const Bool isStandardPointOriginInFailureSpace);

  /** Mean point conditioned to the standard event realization accessor */
  NumericalPoint getMeanPointInStandardEventDomain() const;
  void setMeanPointInStandardEventDomain(const NumericalPoint & meanPointInStandardEventDomain);

  /** ImportanceFactors accessor */
  NumericalPointWithDescription getImportanceFactors(const Bool classical = false) const;

  /** ImportanceFactors graph */
  Graph drawImportanceFactors(const Bool classical = false) const;

  /**  HasoferReliabilityIndex accessor */
  NumericalScalar getHasoferReliabilityIndex() const;

  /**  HasoferReliabilityIndexSensitivity accessor */
  Sensitivity getHasoferReliabilityIndexSensitivity() const;

  /** HasoferReliabilityIndexSensitivitygraph */
  GraphCollection drawHasoferReliabilityIndexSensitivity(NumericalScalar width = ResourceMap::GetAsNumericalScalar( "AnalyticalResult-DefaultWidth" )) const;

  /** String converter */
  String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:


  /** Sensitivitygraph */
  Graph drawSensitivity(const Sensitivity & sensitivity,
                        const NumericalScalar width) const;

private:

  /** PhysicalSpaceDesignPoint evaluation */
  void computePhysicalSpaceDesignPoint() const;

  /** HasoferReliabilityIndex evaluation */
  void computeHasoferReliabilityIndex() const;

  /** ImportanceFactors evaluation */
  void computeImportanceFactors() const;

  /** ImportanceFactors evaluation */
  void computeClassicalImportanceFactors() const;

  /** HasoferReliabilityIndexSensitivity evaluation */
  void computeHasoferReliabilityIndexSensitivity() const;

  /** MeanPointInStandardEventDomain evaluation */
  void computeMeanPointInStandardEventDomain() const;

  /** PhysicalSpaceDesignPoint accessor */
  void setPhysicalSpaceDesignPoint(const NumericalPoint & physicalSpaceDesignPoint);

  /** HasoferReliabilityIndex accessor */
  void setHasoferReliabilityIndex(const NumericalScalar & hasoferReliabilityIndex);

  /** Returns the position of the given (value, name) into the NumericalPoint or the dimension of the NumericalPoint if failed */
  UnsignedLong computePosition(const String & marginalName,
                               const String & marginalParameterName,
                               const Description & parameterSetNames) const;

  NumericalPoint    standardSpaceDesignPoint_;
  mutable NumericalPoint    physicalSpaceDesignPoint_;
  Event             limitStateVariable_;
  Bool isStandardPointOriginInFailureSpace_;
  mutable NumericalScalar hasoferReliabilityIndex_;
  mutable NumericalPointWithDescription importanceFactors_;
  mutable NumericalPointWithDescription classicalImportanceFactors_;
  mutable Sensitivity hasoferReliabilityIndexSensitivity_;
  mutable NumericalPoint meanPointInStandardEventDomain_;
  mutable Bool isAlreadyComputedImportanceFactors_;
  mutable Bool isAlreadyComputedClassicalImportanceFactors_;
  mutable Bool isAlreadyComputedHasoferReliabilityIndexSensitivity_;
  mutable Bool isAlreadyComputedMeanPointInStandardEventDomain_;
}; // class AnalyticalResult
END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ANALYTICALRESULT_HXX */
