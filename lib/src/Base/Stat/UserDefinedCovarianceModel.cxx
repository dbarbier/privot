//                                               -*- C++ -*-
/**
 *  @file  UserDefinedCovarianceModel.cxx
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
#include "UserDefinedCovarianceModel.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

TEMPLATE_CLASSNAMEINIT(PersistentCollection< CovarianceMatrix >);
static Factory< PersistentCollection< CovarianceMatrix > > RegisteredFactory1("PersistentCollection< CovarianceMatrix >");


CLASSNAMEINIT(UserDefinedCovarianceModel);
static Factory<UserDefinedCovarianceModel> RegisteredFactory("UserDefinedCovarianceModel");
/* Constructor with parameters */
UserDefinedCovarianceModel::UserDefinedCovarianceModel(const String & name)
  : CovarianceModelImplementation(name)
  , covarianceCollection_(0)
{
  dimension_ = 0;
}

// Classical constructor
// For a non stationary model, we need N x N covariance functions with N the number of instants in the time grid
UserDefinedCovarianceModel::UserDefinedCovarianceModel(const RegularGrid & timeGrid,
                                                       const CovarianceMatrixCollection & covarianceFunction,
                                                       const String & name)
  : CovarianceModelImplementation(name)
  , covarianceCollection_(0)
{
  const UnsignedLong N(timeGrid.getN());
  const UnsignedLong size(static_cast<UnsignedLong>((N * (N + 1)) / 2));
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: the time grid is empty.";
  if (size != covarianceFunction.getSize())
    throw InvalidArgumentException(HERE) << "Error: for a non stationary covariance model, sizes are incoherents:"
                                         << " timeGrid size=" << N << " and covariance function size=" << covarianceFunction.getSize() << " instead of " << size;
  timeGrid_ = timeGrid;

  covarianceCollection_ = CovarianceMatrixCollection(size);
  // put the first element
  covarianceCollection_[0] = covarianceFunction[0];
  dimension_ = covarianceCollection_[0].getDimension();
  // put the next elements if dimension is ok
  for (UnsignedLong k = 1; k < size; ++k)
    {
      if (covarianceFunction[k].getDimension() != dimension_)
        throw InvalidArgumentException(HERE) << " Error with dimension; all the covariance matrices must have the same dimension";
      covarianceCollection_[k] = covarianceFunction[k];
    }
}

/* Virtual constructor */
UserDefinedCovarianceModel * UserDefinedCovarianceModel::clone() const
{
  return new UserDefinedCovarianceModel(*this);
}


/* Computation of the covariance density function */
CovarianceMatrix UserDefinedCovarianceModel::computeCovariance(const NumericalScalar s,
                                                               const NumericalScalar t) const
{
  if (s > t) return computeCovariance(t, s);
  // If the grid size is one , return the covariance function
  // else find in the grid the nearest instant values
  if (timeGrid_.getN() == 1) return covarianceCollection_[0];

  // We consider that the two instants are in the time grid ==> look for their respective indexes
  // We consider that the time step is positive
  const NumericalScalar initialInstant(timeGrid_.getStart());
  const NumericalScalar step(timeGrid_.getStep());
  const SignedInteger N(timeGrid_.getN());
  const SignedInteger indexS(std::min(static_cast<SignedInteger>(N - 1), std::max(static_cast<SignedInteger>(0), static_cast<SignedInteger>( nearbyint( ( s - initialInstant ) / step) ))));
  const SignedInteger indexT(std::min(static_cast<SignedInteger>(N - 1), std::max(static_cast<SignedInteger>(indexS), static_cast<SignedInteger>( nearbyint( ( t - initialInstant ) / step) ))));
  // We use the information about the ordering of the collection
  // N first elements => s = initialInstant and t = (initialInstant...endInstant)
  // then s = initialInstant + step and t = (initialInstant + step...endInstant)
  // size is N * (N + 1) / 2 with N the time grid size
  const SignedInteger index(indexT + (indexS * (2 * N - indexS - 1)) /2);
  return covarianceCollection_[index];
}

/* Time grid accessor */
RegularGrid UserDefinedCovarianceModel::getTimeGrid() const
{
  return timeGrid_;
}

/* String converter */
String UserDefinedCovarianceModel::__repr__() const
{
  OSS oss;
  oss << "class =" << UserDefinedCovarianceModel::GetClassName()
      << " timeGrid =" << timeGrid_
      << " dimension = " << dimension_
      << " covarianceCollection =" << covarianceCollection_;
  return oss;

}

/* String converter */
String UserDefinedCovarianceModel::__str__(const String & offset) const
{
  return __repr__();
}

/* Method save() stores the object through the StorageManager */
void UserDefinedCovarianceModel::save(Advocate & adv) const
{
  CovarianceModelImplementation::save(adv);
  adv.saveAttribute( "timeGrid_", timeGrid_);
  adv.saveAttribute( "covarianceCollection_", covarianceCollection_);
}

/* Method load() reloads the object from the StorageManager */
void UserDefinedCovarianceModel::load(Advocate & adv)
{
  CovarianceModelImplementation::load(adv);
  adv.loadAttribute( "timeGrid_", timeGrid_);
  adv.loadAttribute( "covarianceCollection_", covarianceCollection_);
}

END_NAMESPACE_OPENTURNS
