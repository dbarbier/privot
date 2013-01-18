//                                               -*- C++ -*-
/**
 *  @file  PiecewiseEvaluationImplementation.cxx
 *  @brief The evaluation part of a linear piecewise scalar function
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
 *  @author dutka
 *  @date   2008-05-21 11:21:38 +0200 (Wed, 21 May 2008)
 */
#include <algorithm>
#include "PiecewiseEvaluationImplementation.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "Description.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(PiecewiseEvaluationImplementation);

static Factory<PiecewiseEvaluationImplementation> RegisteredFactory("PiecewiseEvaluationImplementation");


/* Default constructor */
PiecewiseEvaluationImplementation::PiecewiseEvaluationImplementation()
  : NumericalMathEvaluationImplementation()
  , locations_(0)
  , values_(0)
{
  // Nothing to do
}


/* Parameters constructor */
PiecewiseEvaluationImplementation::PiecewiseEvaluationImplementation(const NumericalPoint & locations,
                                                                     const NumericalPoint & values)
  : NumericalMathEvaluationImplementation()
  , locations_(0)
  , values_(0)
{
  // Check the input
  setLocationsAndValues(locations, values);
}


/* Virtual constructor */
PiecewiseEvaluationImplementation * PiecewiseEvaluationImplementation::clone() const
{
  return new PiecewiseEvaluationImplementation(*this);
}


/* String converter */
String PiecewiseEvaluationImplementation::__repr__() const
{
  return OSS() << "class=" << GetClassName()
               << " locations=" << locations_
	       << " values=" << values_;
}

String PiecewiseEvaluationImplementation::__str__(const String & offset) const
{
  return OSS() << offset << __repr__();
}


/* Evaluation operator */
NumericalPoint PiecewiseEvaluationImplementation::operator () (const NumericalPoint & inP) const
{
  if (inP.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: expected an input point of dimension 1, got dimension=" << inP.getDimension();
  const NumericalScalar x(inP[0]);
  UnsignedLong iLeft(0);
  if (x <= locations_[iLeft]) return NumericalPoint(1, values_[iLeft]);
  UnsignedLong iRight(locations_.getSize() - 1);
  if (x >= locations_[iRight]) return NumericalPoint(1, values_[iRight]);
  // Find the segment containing x by bisection
  while (iRight - iLeft> 1)
    {
      const UnsignedLong iMiddle((iRight + iLeft) / 2);
      if (x < locations_[iMiddle]) iRight = iMiddle;
      else iLeft = iMiddle;
    }
  const NumericalScalar value((values_[iRight] * (locations_[iLeft] - x) + values_[iLeft] * (x - locations_[iRight])) / (locations_[iLeft] - locations_[iRight]));
  return NumericalPoint(1, value);
}

/* Locations accessor */
NumericalPoint PiecewiseEvaluationImplementation::getLocations() const
{
  return locations_;
}

void PiecewiseEvaluationImplementation::setLocations(const NumericalPoint & locations)
{
  if (locations.getDimension() != values_.getDimension()) throw InvalidArgumentException(HERE) << "Error: the number of locations=" << locations.getDimension() << " must match the number of previously set values="<< values_.getDimension();
  locations_ = locations;
  std::stable_sort(locations_.begin(), locations_.end());
}

/* Values accessor */
NumericalPoint PiecewiseEvaluationImplementation::getValues() const
{
  return values_;
}

void PiecewiseEvaluationImplementation::setValues(const NumericalPoint & values)
{
  if (values.getDimension() != locations_.getDimension()) throw InvalidArgumentException(HERE) << "Error: the number of values=" << values.getDimension() << " must match the number of previously set locations="<< locations_.getDimension();
  values_ = values;
}


void PiecewiseEvaluationImplementation::setLocationsAndValues(const NumericalPoint & locations,
                                                              const NumericalPoint & values)
{
  const UnsignedLong size(locations.getDimension());
  if (size != values.getDimension()) throw InvalidArgumentException(HERE) << "Error: the number of values=" << values.getDimension() << " must match the number of locations="<< size;
  // Sort the data in increasing order according to the locations
  NumericalSample data(size, 2);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      data[i][0] = locations[i];
      data[i][1] = values[i];
    }
  data = data.sortAccordingToAComponent(0);
  locations_ = NumericalPoint(size);
  values_ = NumericalPoint(size);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      locations_[i] = data[i][0];
      values_[i] = data[i][1];
    }
}

/* Input dimension accessor */
UnsignedLong PiecewiseEvaluationImplementation::getInputDimension() const
{
  return 1;
}

/* Output dimension accessor */
UnsignedLong PiecewiseEvaluationImplementation::getOutputDimension() const
{
  return 1;
}


/* Method save() stores the object through the StorageManager */
void PiecewiseEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute( "locations_", locations_ );
  adv.saveAttribute( "values_", values_ );
}


/* Method load() reloads the object from the StorageManager */
void PiecewiseEvaluationImplementation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute( "locations_", locations_ );
  adv.loadAttribute( "values_", values_ );
}


END_NAMESPACE_OPENTURNS
