//                                               -*- C++ -*-
/**
 *  @file  TemporalNormalProcess.hxx
 *  @brief A class which implements the TemporalNormalProcess process
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 *  Id      TemporalNormalProcess.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_TEMPORALNORMALPROCESS_HXX
#define OPENTURNS_TEMPORALNORMALPROCESS_HXX

#include "ProcessImplementation.hxx"
#include "SquareMatrix.hxx"
#include "SecondOrderModel.hxx"
#include "CovarianceModel.hxx"
#include "TrendTransform.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class TemporalNormalProcess
 *
 * The implementation of the Normal process class
 */
class TemporalNormalProcess
  : public ProcessImplementation
{
  CLASSNAME;

public:

  /** Some typedefs to ease reading */

  /** Default constructor */
  TemporalNormalProcess(const String & name = DefaultName);

  /** Standard constructor  */
  TemporalNormalProcess(const TrendTransform & trend,
			const SecondOrderModel  & model,
                        const RegularGrid & timeGrid,
                        const String & name = DefaultName);

  /** Standard constructor  */
  TemporalNormalProcess(const TrendTransform & trend,
			const CovarianceModel & covarianceModel,
                        const RegularGrid & timeGrid,
                        const String & name = DefaultName);

  /** Standard constructor  */
  TemporalNormalProcess(const SecondOrderModel  & model,
                        const RegularGrid & timeGrid,
                        const String & name = DefaultName);

  /** Standard constructor  */
  TemporalNormalProcess(const CovarianceModel & covarianceModel,
                        const RegularGrid & timeGrid,
                        const String & name = DefaultName);

  /** Virtual constructor */
  TemporalNormalProcess * clone() const;

  /** String converter */
  String __repr__() const;

  /** String converter  - pretty print */
  String __str__(const String & offset = "") const;

  /** TimeGrid accessor */
  void setTimeGrid(const RegularGrid & timeGrid);

  /** Realization accessor */
  TimeSeries getRealization() const;

  /** Covariance model accessor */
  CovarianceModel getCovarianceModel() const;

  /** Trend accessor */
  TrendTransform getTrend() const;

  /** Check if the process is stationary */
  Bool isStationary() const;

  /** Check if the process trend is stationary */
  Bool isTrendStationary() const;

  /** Check if the process is Normal */
  Bool isNormal() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  /** Initialization of the process */
  void initialize() const;

  /** Check if the trend function is stationary */
  void checkStationaryTrend() const;

private:

  /** Covariance model */
  CovarianceModel covarianceModel_;

  /** Cholesky factor  */
  mutable SquareMatrix choleskyFactorCovarianceMatrix_;

  /** Flag to manage process initialization */
  mutable Bool isInitialized_;

  /** Flag to tell if the process has a stationary trend */
  mutable Bool hasStationaryTrend_;
  mutable Bool checkedStationaryTrend_;
  mutable NumericalPoint stationaryTrendValue_;

  /** Trend function */
  TrendTransform trend_;

}; /* class TemporalNormalProcess */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NORMALPROCESS_HXX */
