//                                               -*- C++ -*-
/**
 *  @file  WeightedExperiment.cxx
 *  @brief Abstract top-level view of an weightedExperiment plane
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
#include <cstdlib>

#include "OTprivate.hxx"
#include "WeightedExperiment.hxx"
#include "Exception.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(WeightedExperiment);


/* Default constructor */
WeightedExperiment::WeightedExperiment(const String & name):
  ExperimentImplementation(name),
  distribution_(),
  size_(ResourceMap::GetAsUnsignedLong( "WeightedExperiment-DefaultSize" )),
  weights_(ResourceMap::GetAsUnsignedLong( "WeightedExperiment-DefaultSize" ), 1.0 / ResourceMap::GetAsUnsignedLong( "WeightedExperiment-DefaultSize" ))
{
  // Nothing to do
}

/* Constructor with parameters */
WeightedExperiment::WeightedExperiment(const UnsignedLong size,
                                       const String & name):
  ExperimentImplementation(name),
  distribution_(),
  size_(0),
  weights_(0)
{
  // Check if the size is valid
  setSize(size);
}

/* Constructor with parameters */
WeightedExperiment::WeightedExperiment(const Distribution & distribution,
                                       const UnsignedLong size,
                                       const String & name):
  ExperimentImplementation(name),
  distribution_(distribution),
  size_(0),
  weights_(0)
{
  // Check if the size is valid
  setSize(size);
}

/* Virtual constructor */
WeightedExperiment * WeightedExperiment::clone() const
{
  return new WeightedExperiment(*this);
}

/* String converter */
String WeightedExperiment::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName ()
      << " distribution=" << distribution_
      << " size=" << size_;
  return oss;
}

/* Distribution accessor */
void WeightedExperiment::setDistribution(const Distribution & distribution)
{
  distribution_ = distribution;
}

Distribution WeightedExperiment::getDistribution() const
{
  return distribution_;
}

/* Size accessor */
void WeightedExperiment::setSize(const UnsignedLong size)
{
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: the size must be > 0.";
  size_ = size;
  weights_ = NumericalPoint(size_, 1.0 / size_);
}

UnsignedLong WeightedExperiment::getSize() const
{
  return size_;
}

/* Sample generation */
NumericalSample WeightedExperiment::generate()
{
  throw NotYetImplementedException(HERE);
}

/* Sample generation with weights */
NumericalSample WeightedExperiment::generate(NumericalPoint & weights)
{
  const NumericalSample sample(generate());
  weights = weights_;
  return sample;
}

/* Weight accessor */
NumericalPoint WeightedExperiment::getWeight() const
{
  return weights_;
}

END_NAMESPACE_OPENTURNS
