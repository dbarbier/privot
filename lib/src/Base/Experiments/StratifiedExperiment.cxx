//                                               -*- C++ -*-
/**
 *  @file  StratifiedExperiment.cxx
 *  @brief Abstract top-level view of a stratified experiment plane
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
 *  @date   2008-06-26 13:50:17 +0200 (jeu, 26 jun 2008)
 */
#include "StratifiedExperiment.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(StratifiedExperiment);

/* Default constructor */
StratifiedExperiment::StratifiedExperiment(const String & name):
  ExperimentImplementation(name),
  center_(NumericalPoint(0)),
  levels_(NumericalPoint(0))
{
  // Nothing to do
}

/* Constructor with parameters */
StratifiedExperiment::StratifiedExperiment(const NumericalPoint & center,
                                           const NumericalPoint & levels,
                                           const String & name):
  ExperimentImplementation(name),
  center_(center),
  levels_(levels)
{
  // Nothing to do
}

/* String converter */
String StratifiedExperiment::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName ()
      << " center=" << center_
      << " levels=" << levels_;
  return oss;
}

/* Center accessor */
void StratifiedExperiment::setCenter(const NumericalPoint & center)
{
  center_ = center;
}

/* Center accessor */
NumericalPoint StratifiedExperiment::getCenter() const
{
  return center_;
}

/* Levels accessor */
void StratifiedExperiment::setLevels(const NumericalPoint & levels)
{
  levels_ = levels;
}

/* Levels accessor */
NumericalPoint StratifiedExperiment::getLevels() const
{
  return levels_;
}

END_NAMESPACE_OPENTURNS
