//                                               -*- C++ -*-
/**
 *  @file  UserDefinedSpectralModel.cxx
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
 *  @author schueller
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include "UserDefinedSpectralModel.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

TEMPLATE_CLASSNAMEINIT(PersistentCollection< HermitianMatrix >);
static Factory< PersistentCollection< HermitianMatrix > > RegisteredFactory1("PersistentCollection< HermitianMatrix >");


CLASSNAMEINIT(UserDefinedSpectralModel);
static Factory<UserDefinedSpectralModel> RegisteredFactory("UserDefinedSpectralModel");
/* Constructor with parameters */
UserDefinedSpectralModel::UserDefinedSpectralModel(const String & name)
  : SpectralModelImplementation(name)
  , DSPCollection_(0)
{
  dimension_ = 0;
}

UserDefinedSpectralModel::UserDefinedSpectralModel(const RegularGrid & frequencyGrid,
                                                   const HermitianMatrixCollection & spectralFunction,
                                                   const String & name)
  : SpectralModelImplementation(name),
    DSPCollection_(0)
{
  const UnsignedLong N(frequencyGrid.getN());
  if (N != spectralFunction.getSize())
    throw InvalidArgumentException(HERE) << "Error: Frequency grid and spectral functions have different sizes";
  if (frequencyGrid.getStart() < 0.0)
    throw InvalidArgumentException(HERE) << "Error: The frequency grid must contains only nonnegative values";
  setFrequencyGrid(frequencyGrid);
  DSPCollection_ = HermitianMatrixCollection(N);
  // put the first element
  DSPCollection_[0] = spectralFunction[0];
  setDimension(DSPCollection_[0].getDimension());
  // put the next elements if dimension is ok
  for (UnsignedLong k = 1; k < N; ++k)
    {
      if (spectralFunction[k].getDimension() != dimension_)
        throw InvalidArgumentException(HERE) << " Error with dimension; the spectral matrices should be of same dimension";
      DSPCollection_[k] = spectralFunction[k];
    }
}

/* Virtual constructor */
UserDefinedSpectralModel * UserDefinedSpectralModel::clone() const
{
  return new UserDefinedSpectralModel(*this);
}


/* Computation of the spectral density function */
HermitianMatrix UserDefinedSpectralModel::computeSpectralDensity(const NumericalScalar frequency) const
{
  Bool nonNegative(frequency >= 0.0);
  // If the grid size is one , return the spectral function
  // else find in the grid the nearest frequency value
  if (getFrequencyGrid().getN() == 1) return DSPCollection_[0];
  const NumericalScalar frequencyStep(getFrequencyGrid().getStep());
  const UnsignedLong nFrequency(getFrequencyGrid().getN());
  const UnsignedLong index(std::min<UnsignedLong>( nFrequency - 1, static_cast<UnsignedLong>( std::max<NumericalScalar>( 0.0, nearbyint( ( fabs(frequency) - frequencyGrid_.getStart() ) / frequencyStep) ) ) ));
  // Use the relation S(-f) = conjugate(S(f))
  return (nonNegative ? DSPCollection_[index] : DSPCollection_[index].conjugate());
}


/* String converter */
String UserDefinedSpectralModel::__repr__() const
{
  OSS oss;
  oss << "class=" << UserDefinedSpectralModel::GetClassName()
      << " DSPCollection=" << DSPCollection_
      << " frequencyGrid=" << getFrequencyGrid();
  return oss;
}

/* String converter */
String UserDefinedSpectralModel::__str__(const String & offset) const
{
  OSS oss;
  oss << "frequency =" << frequencyGrid_.__str__(offset)
      << " dimension = " << dimension_;
  return oss;
}

/* Method save() stores the object through the StorageManager */
void UserDefinedSpectralModel::save(Advocate & adv) const
{
  SpectralModelImplementation::save(adv);
  adv.saveAttribute( "DSPCollection_", DSPCollection_);
}

/* Method load() reloads the object from the StorageManager */
void UserDefinedSpectralModel::load(Advocate & adv)
{
  SpectralModelImplementation::load(adv);
  adv.loadAttribute( "DSPCollection_", DSPCollection_);
}

END_NAMESPACE_OPENTURNS
