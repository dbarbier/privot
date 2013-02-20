//                                               -*- C++ -*-
/**
 *  @file  SamplingStrategyImplementation.cxx
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include "SamplingStrategyImplementation.hxx"
#include "Exception.hxx"
#include "DistFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class SamplingStrategyImplementation
 */

CLASSNAMEINIT(SamplingStrategyImplementation);

/* Constructor with parameters */
SamplingStrategyImplementation::SamplingStrategyImplementation(const UnsignedLong dimension)
  : PersistentObject(),
    dimension_(dimension)
{
  // Nothing to do
}

/* Virtual constructor */
SamplingStrategyImplementation * SamplingStrategyImplementation::clone() const
{
  return new SamplingStrategyImplementation(*this);
}

/* Generate a set of directions */
NumericalSample SamplingStrategyImplementation::generate() const
{
  throw NotYetImplementedException(HERE);
}

/* Generate a uniform random unit vector */
NumericalPoint SamplingStrategyImplementation::getUniformUnitVectorRealization(const UnsignedLong dimension) const
{
  NumericalPoint direction(dimension);
  NumericalScalar norm(0.0);
  do
    {
      for (UnsignedLong i = 0; i < dimension; ++i) direction[i] = DistFunc::rNormal();
      norm = direction.norm();
    }
  while (norm == 0.0);
  return (1.0 / norm) * direction;
}

/* Generate a uniform random unit vector */
NumericalPoint SamplingStrategyImplementation::getUniformUnitVectorRealization() const
{
  return getUniformUnitVectorRealization(dimension_);
}

/* Dimension accessor */
void SamplingStrategyImplementation::setDimension(const UnsignedLong dimension)
{
  dimension_ = dimension;
}

UnsignedLong SamplingStrategyImplementation::getDimension() const
{
  return dimension_;
}

/* String converter */
String SamplingStrategyImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << SamplingStrategyImplementation::GetClassName()
      << " dimension=" << dimension_;
  return oss;
}

END_NAMESPACE_OPENTURNS
