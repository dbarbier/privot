//                                               -*- C++ -*-
/**
 *  @file  FORMResult.hxx
 *  @brief FORMResult implements the First Order Reliability Method
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
 *  @date   2007-12-27 12:24:11 +0100 (jeu, 27 déc 2007)
 *  Id      FORMRESULT.hxx 714 2007-12-27 11:24:11Z lebrun
 */
#ifndef OPENTURNS_FORMRESULT_HXX
#define OPENTURNS_FORMRESULT_HXX

#include "AnalyticalResult.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class FORMResult
 * FORMResult stores the FORM result
 */
class FORMResult:
  public AnalyticalResult
{
  CLASSNAME;
public:


  /** Standard constructor */
  FORMResult(const NumericalPoint & standardSpaceDesignPoint,
             const Event & limitStateVariable,
             const Bool isStandardPointOriginInFailureSpace,
             const String & name = DefaultName);

  /* Default constructor (required by SWIG :-<) */
  explicit FORMResult();

  /** Virtual constructor */
  virtual FORMResult * clone() const;

  /** EventProbability accessor */
  NumericalScalar getEventProbability() const;

  /** GeneralisedReliabilityIndex accessor */
  NumericalScalar getGeneralisedReliabilityIndex() const;

  /** EventProbabilitySensitivity accessor */
  Sensitivity getEventProbabilitySensitivity() const;

  /** HasoferReliabilityIndexSensitivitygraph */
  GraphCollection drawEventProbabilitySensitivity(NumericalScalar width = ResourceMap::GetAsNumericalScalar("AnalyticalResult-DefaultWidth")) const;

  /** String converter */
  String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /** The function that actually evaluates the event probability with FORM approximation */
  void computeEventProbability() const;

  /** The function that actually evaluates the generalised reliability index with FORM approximation */
  void computeGeneralisedReliabilityIndex() const;

  /** The function that actually evaluates the  event probability sensitivity with FORM approximation */
  void computeEventProbabilitySensitivity() const;

  /** EventProbability accessor */
  void setEventProbability(const NumericalScalar & eventProbability);

  /** GeneralisedReliabilityIndex accessor */
  void setGeneralisedReliabilityIndex(const NumericalScalar & generalisedReliabilityIndex);

  /** EventProbabilitySensitivity accessor */
  void setEventProbabilitySensitivity(const Sensitivity & eventProbabilitySensitivity);

  mutable NumericalScalar eventProbability_;
  mutable NumericalScalar generalisedReliabilityIndex_;
  mutable Sensitivity eventProbabilitySensitivity_;
  mutable Bool isAlreadyComputedEventProbabilitySensitivity_;

}; // class FORMResult

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_FORMRESULT_HXX */



