//                                               -*- C++ -*-
/**
 *  @file  Axial.cxx
 *  @brief Implementation of the axial experiment plane
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
#include "OTprivate.hxx"
#include "Axial.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Axial);

/* Default constructor */
Axial::Axial(const String & name):
  StratifiedExperiment(name)
{
  // Nothing to do
}

/* Constructor with parameters */
Axial::Axial(const NumericalPoint & center,
             const NumericalPoint & levels,
             const String & name):
  StratifiedExperiment(center, levels, name)
{
  // Nothing to do
}

/* Constructor with parameters */
Axial::Axial(const UnsignedLong dimension,
             const NumericalPoint & levels,
             const String & name):
  StratifiedExperiment(NumericalPoint(dimension, 0.0), levels, name)
{
  // Nothing to do
}

/* Virtual constructor */
Axial * Axial::clone() const
{
  return new Axial(*this);
}

/* Experiment plane generation */
NumericalSample Axial::generate()
{
  /* Dimension of the realizations */
  UnsignedLong dimension(center_.getDimension());
  /* Number of level to be generated */
  UnsignedLong levelNumber(levels_.getDimension());
  /* Size of the sample to be generated: 1 + 2 x number of levels x dimension */
  UnsignedLong size(1 + 2 * levelNumber * dimension);
  NumericalSample axialPlane(size, center_);
  axialPlane.setName("Axial plane");
  UnsignedLong index(1);
  /* For each level of the axial plane */
  for(UnsignedLong levelIndex = 0; levelIndex < levelNumber; ++levelIndex)
    {
      NumericalScalar levelValue(levels_[levelIndex]);
      /* For each vertex of one half of the current level */
      for(UnsignedLong vertex = 0; vertex < dimension; ++vertex)
        {
          /* Compute the coordinates of the positive vertex */
          axialPlane[index][vertex] += levelValue;
          ++index;
          /* Compute the coordinates of the negative vertex */
          axialPlane[index][vertex] -= levelValue;
          ++index;
        } // vertex
    } // levelIndex
  return axialPlane;
} // generate()

/* String converter */
String Axial::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName()
      << " center=" << center_
      << " levels=" << levels_;
  return oss;
}

END_NAMESPACE_OPENTURNS
