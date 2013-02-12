//                                               -*- C++ -*-
/**
 *  @file  Box.cxx
 *  @brief Implementation of the box experiment plane
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
#include <cmath>
#include "OTprivate.hxx"
#include "Box.hxx"
#include "Indices.hxx"
#include "Tuples.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Box);

/* Default constructor */
Box::Box(const String & name)
  : StratifiedExperiment(name)
{
  // Nothing to do
}

/* Constructor with parameters */
Box::Box(const NumericalPoint & levels,
         const String & name)
  : StratifiedExperiment(NumericalPoint(levels.getDimension(), 0.0), levels, name)
{
  // Check if there is the same number of levels than the dimension of the experiment plane
  if (levels.getDimension() == 0) throw InvalidArgumentException(HERE) << "Error: the levels dimension must be > 0";
  setLevels(levels);
}

/* Constructor with parameters */
Box::Box(const Indices & levels,
         const String & name)
  : StratifiedExperiment(NumericalPoint(levels.getSize(), 0.0), NumericalPoint(levels.getSize(), 0.0), name)
{
  // Check if there is the same number of levels than the dimension of the experiment plane
  const UnsignedLong size(levels.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: the levels dimension must be > 0";
  setLevels(levels);
}

/* Virtual constructor */
Box * Box::clone() const
{
  return new Box(*this);
}

/* Experiment plane generation
   The box [0, 1]^n is uniformly sampled in each dimension
   levels counts the number of interior points in each dimension */
NumericalSample Box::generate()
{
  const UnsignedLong dimension(levels_.getDimension());
  Indices bounds(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i) bounds[i] = static_cast< UnsignedLong > (round(levels_[i] + 2.0));
  Tuples::IndicesCollection tuples(Tuples(bounds).generate());
  const UnsignedLong size(tuples.getSize());
  NumericalSample boxPlane(size, dimension);
  for (UnsignedLong i = 0; i < size; ++i)
    for (UnsignedLong j = 0; j < dimension; ++j)
      boxPlane[i][j] = tuples[i][j] / (levels_[j] + 1.0);
  return boxPlane;
} // generate()

/* String converter */
String Box::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName()
      << " levels=" << levels_;
  return oss;
}

/** Specific levels accessor */
void Box::setLevels(const NumericalPoint & levels)
{
  UnsignedLong dimension(center_.getDimension());
  UnsignedLong size(levels.getDimension());
  if (size != dimension) throw InvalidArgumentException(HERE) << "Error: levels dimension must equal center dimension for the Box design of experiment, here levels dimension=" << size << " and center dimension=" << dimension;
  for (UnsignedLong i = 0; i < dimension; ++i) if (levels[i] < 0.0) throw InvalidArgumentException(HERE) << "Error: levels values must be greater or equal to 0 for the Box design of experiment";
  StratifiedExperiment::setLevels(levels);
}

END_NAMESPACE_OPENTURNS
