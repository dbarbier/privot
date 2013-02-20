//                                               -*- C++ -*-
/**
 *  @file  CalibrationStrategyImplementation.hxx
 *  @brief Default CalibrationStrategyImplementation
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#ifndef OPENTURNS_CALIBRATIONSTRATEGYIMPLEMENTATION_HXX
#define OPENTURNS_CALIBRATIONSTRATEGYIMPLEMENTATION_HXX

#include "OTprivate.hxx"
#include "PersistentObject.hxx"
#include "Interval.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class CalibrationStrategyImplementation
 *
 * @brief The class implements the concept of comparison operator defined in CalibrationStrategy.
 *
 * This class is abstract so it can not be instanciated. It must be derived.
 * @see CalibrationStrategy
 */
class CalibrationStrategyImplementation
  : public PersistentObject
{
  CLASSNAME;
public:

  /** Default constructor with parameters */
  CalibrationStrategyImplementation(const Interval & range = Interval(ResourceMap::GetAsNumericalScalar("CalibrationStrategyImplementation-DefaultLowerBound"),
                                                                      ResourceMap::GetAsNumericalScalar("CalibrationStrategyImplementation-DefaultUpperBound")),
                                    const NumericalScalar expansionFactor = ResourceMap::GetAsNumericalScalar("CalibrationStrategyImplementation-DefaultExpansionFactor"),
                                    const NumericalScalar shrinkFactor = ResourceMap::GetAsNumericalScalar("CalibrationStrategyImplementation-DefaultShrinkFactor"),
                                    const UnsignedLong calibrationStep = ResourceMap::GetAsUnsignedLong("CalibrationStrategyImplementation-DefaultCalibrationStep"));

  /** String converter */
  virtual String __repr__() const;

  /** Range accessor */
  void setRange(const Interval & range);
  Interval getRange() const;

  /** Expansion factor accessor */
  void setExpansionFactor(NumericalScalar expansionFactor);
  NumericalScalar getExpansionFactor() const;

  /** Shrink factor accessor */
  void setShrinkFactor(NumericalScalar shrinkFactor);
  NumericalScalar getShrinkFactor() const;

  /** Calibration period */
  void setCalibrationStep(const UnsignedLong calibrationStep);
  UnsignedLong getCalibrationStep() const;

  /* Here is the interface that all derived class must implement */

  /** Virtual constructor */
  virtual CalibrationStrategyImplementation * clone() const;

  /** @copydoc CalibrationStrategy::computeUpdateFactor() const */
  virtual NumericalScalar computeUpdateFactor(const NumericalScalar rho) const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

private:
  Interval range_;
  NumericalScalar shrinkFactor_;
  NumericalScalar expansionFactor_;

  /// period between recalibrations
  UnsignedLong calibrationStep_;

}; /* class CalibrationStrategyImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_CALIBRATIONSTRATEGYIMPLEMENTATION_HXX */
