//                                               -*- C++ -*-
/**
 *  @file  StationaryCovarianceModel.hxx
 *  @brief This class enables to build a covariance model
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
 *  Id      StationaryCovarianceModel.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_STATIONARYCOVARIANCEMODEL_HXX
#define OPENTURNS_STATIONARYCOVARIANCEMODEL_HXX

#include "CovarianceModel.hxx"
#include "Pointer.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class StationaryCovarianceModel
 */

class StationaryCovarianceModel
  : public CovarianceModelImplementation
{

  CLASSNAME;

public:


  /** Default constructor without parameters */
  StationaryCovarianceModel(const String & name = DefaultName);

  /** Virtual copy constructor */
  virtual StationaryCovarianceModel * clone() const;

  /** Computation of the covariance matrix */
  virtual CovarianceMatrix computeCovariance(const NumericalScalar s,
                                             const NumericalScalar t) const;


  /** Computation of the covariance matrix for stationnary models */
  virtual CovarianceMatrix computeCovariance(const NumericalScalar tau) const;

  /** Discretize the covariance function on a given TimeGrid */
  CovarianceMatrix discretizeCovariance(const RegularGrid & timeGrid) const;

  /** Is the underlying a stationary covariance model ? */
  Bool isStationary() const;

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

} ; /* class StationaryCovarianceModel */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_STATIONARYCOVARIANCEMODEL_HXX */
