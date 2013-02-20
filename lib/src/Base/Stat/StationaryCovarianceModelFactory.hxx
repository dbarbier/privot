//                                               -*- C++ -*-
/**
 *  @file  StationaryCovarianceModelFactory.hxx
 *  @brief Top-level class for all spectral model factories
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
 *  @author: $LastChangedBy: schueller $
 *  @date:   $LastChangedDate: 2012-02-17 19:35:43 +0100 (ven. 17 févr. 2012) $
 *  Id:      $Id: StationaryCovarianceModelFactory.hxx 2392 2012-02-17 18:35:43Z schueller $
 */
#ifndef OPENTURNS_STATIONARYCOVARIANCEMODELFACTORY_HXX
#define OPENTURNS_STATIONARYCOVARIANCEMODELFACTORY_HXX

#include "CovarianceModelFactoryImplementation.hxx"
#include "UserDefinedStationaryCovarianceModel.hxx"
#include "SpectralModelFactory.hxx"
#include "WelchFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class StationaryCovarianceModelFactory
 */
class StationaryCovarianceModelFactory
  : public CovarianceModelFactoryImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  StationaryCovarianceModelFactory(const SpectralModelFactory & factory = WelchFactory(),
                                   const String & name = OT::DefaultName);

  /** Virtual constructor */
  virtual StationaryCovarianceModelFactory * clone() const;

  /** SpectralModelFactory accessors */
  SpectralModelFactory getSpectralModelFactory() const;
  void setSpectralModelFactory(const SpectralModelFactory & factory);

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** Build a a spectral model based on a sample */
  UserDefinedStationaryCovarianceModel * build(const ProcessSample & sample) const;

  /** Build a a spectral model based on a TimeSeries */
  UserDefinedStationaryCovarianceModel * build(const TimeSeries & timeSerie) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  UserDefinedStationaryCovarianceModel build(const SpectralModel & mySpectralModel) const;

private:

  /** SpectralModelEstimate */
  SpectralModelFactory spectralFactory_;

}; /* class StationaryCovarianceModelFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_STATIONARYCOVARIANCEMODELFACTORY_HXX */
