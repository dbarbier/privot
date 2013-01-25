//                                               -*- C++ -*-
/**
 *  @file  UserDefinedStationaryCovarianceModel.cxx
 *  @brief
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
 *  @author: $LastChangedBy$
 *  @date:   $LastChangedDate$
 *  Id:      $Id$
 */
#include "UserDefinedStationaryCovarianceModel.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

// TEMPLATE_CLASSNAMEINIT(PersistentCollection< CovarianceMatrix >);
// static Factory< PersistentCollection< CovarianceMatrix > > RegisteredFactory1("PersistentCollection< CovarianceMatrix >");


CLASSNAMEINIT(UserDefinedStationaryCovarianceModel);
static Factory<UserDefinedStationaryCovarianceModel> RegisteredFactory("UserDefinedStationaryCovarianceModel");
/* Constructor with parameters */
UserDefinedStationaryCovarianceModel::UserDefinedStationaryCovarianceModel(const String & name)
  : StationaryCovarianceModel(name),
    covarianceCollection_(0)
{
  dimension_ = 0;
}

// Classical constructor
// For a non stationary model, we need N x N covariance functions with N the number of instants in the time grid
UserDefinedStationaryCovarianceModel::UserDefinedStationaryCovarianceModel(const RegularGrid & timeGrid,
                                                                           const CovarianceMatrixCollection & covarianceFunction,
                                                                           const String & name)
  : StationaryCovarianceModel(name),
    covarianceCollection_(0)
{
  const UnsignedLong size(timeGrid.getN());
  if (size != covarianceFunction.getSize())
    throw InvalidArgumentException(HERE) << "Error: for a non stationary covariance model, sizes are incoherents"
                                         << " timeGrid size = " << size << "covariance function size = " << covarianceFunction.getSize();
  timeGrid_ = timeGrid;

  covarianceCollection_ = CovarianceMatrixCollection(size);
  // put the first element
  covarianceCollection_[0] = covarianceFunction[0];
  dimension_ = covarianceCollection_[0].getDimension();
  // put the next elements if dimension is ok
  for (UnsignedLong k = 1; k < size; ++k)
    {
      if (covarianceFunction[k].getDimension() != dimension_)
        throw InvalidArgumentException(HERE) << " Error with dimension; the covariance matrices should be of same dimension";
      covarianceCollection_[k] = covarianceFunction[k];
    }
}

/* Virtual constructor */
UserDefinedStationaryCovarianceModel * UserDefinedStationaryCovarianceModel::clone() const
{
  return new UserDefinedStationaryCovarianceModel(*this);
}


/* Computation of the covariance function */
CovarianceMatrix UserDefinedStationaryCovarianceModel::computeCovariance(const NumericalScalar t,
                                                                         const NumericalScalar s) const
{
  return computeCovariance(std::abs(t - s));
}

/* Computation of the covariance function */
CovarianceMatrix UserDefinedStationaryCovarianceModel::computeCovariance(const NumericalScalar t) const
{

  // If the grid size is one , return the covariance function
  // else find in the grid the nearest instant values
  if (timeGrid_.getN() == 1) return covarianceCollection_[0];

  // We consider that the time step is positive
  const NumericalScalar initialInstant(timeGrid_.getStart());
  const NumericalScalar N(timeGrid_.getN());
  const NumericalScalar step(timeGrid_.getStep());
  const SignedInteger index(static_cast<SignedInteger>(std::min(N - 1.0, std::max(0.0,  nearbyint( ( t - initialInstant ) / step) ))));

  return covarianceCollection_[index];
}

/* Time grid accessor */
RegularGrid UserDefinedStationaryCovarianceModel::getTimeGrid() const
{
  return timeGrid_;
}

/* String converter */
String UserDefinedStationaryCovarianceModel::__repr__() const
{
  OSS oss;
  oss << "class =" << UserDefinedStationaryCovarianceModel::GetClassName()
      << " timeGrid =" << timeGrid_
      << " dimension = " << dimension_
      << " covarianceCollection =" << covarianceCollection_;
  return oss;

}

/* String converter */
String UserDefinedStationaryCovarianceModel::__str__(const String & offset) const
{
  return __repr__();
}

/* Method save() stores the object through the StorageManager */
void UserDefinedStationaryCovarianceModel::save(Advocate & adv) const
{
  StationaryCovarianceModel::save(adv);
  adv.saveAttribute( "timeGrid_", timeGrid_);
  adv.saveAttribute( "covarianceCollection_", covarianceCollection_);
}

/* Method load() reloads the object from the StorageManager */
void UserDefinedStationaryCovarianceModel::load(Advocate & adv)
{
  StationaryCovarianceModel::load(adv);
  adv.loadAttribute( "timeGrid_", timeGrid_);
  adv.loadAttribute( "covarianceCollection_", covarianceCollection_);
}

END_NAMESPACE_OPENTURNS
