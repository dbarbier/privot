//                                               -*- C++ -*-
/**
 *  @file  WhiteNoise.cxx
 *  @brief A class which implements the WhiteNoise process
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

#include "WhiteNoise.hxx"
#include "PersistentObjectFactory.hxx"
#include "Normal.hxx"
#include "ResourceMap.hxx"
#include "NumericalPoint.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(WhiteNoise);

static Factory<WhiteNoise> RegisteredFactory("WhiteNoise");

/* Standard constructor */
WhiteNoise::WhiteNoise(const String & name)
  : ProcessImplementation(name),
    distribution_(Normal())
{
  // Nothing to do
}

/* Standard constructor */
WhiteNoise::WhiteNoise(const Distribution & distribution,
                       const String & name)
  /* throw (InvalidArgumentException) */
  : ProcessImplementation(name),
    distribution_()
{
  // Check if the given distribution has a null mean
  setDistribution(distribution);
}

/* Standard constructor */
WhiteNoise::WhiteNoise(const Distribution & distribution,
                       const RegularGrid & timeGrid,
                       const String & name)
  : ProcessImplementation(name),
    distribution_()
{
  // Set the dimension of the process
  // Check if the given distribution has a null mean
  setDistribution(distribution);
  setTimeGrid(timeGrid);
}

/* Virtual constructor */
WhiteNoise * WhiteNoise::clone() const
{
  return new WhiteNoise(*this);
}

/* String converter */
String WhiteNoise::__repr__() const
{
  OSS oss;
  oss << "class=" << WhiteNoise::GetClassName()
      << " distribution =" << distribution_ ;
  return oss;
}

String WhiteNoise::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(" << distribution_.__str__() << ")";
  return oss;
}

/* Is the underlying stationary ? */
Bool WhiteNoise::isStationary() const
{
  return true;
}

/* Is the underlying gaussian ? */
Bool WhiteNoise::isNormal() const
{
  return (distribution_.getImplementation()->getClassName() == "Normal");
}

/* Realization accessor */
TimeSeries WhiteNoise::getRealization() const
{
  return TimeSeries(timeGrid_, distribution_.getSample(timeGrid_.getN()));
}

/* Compute the next steps of a random walk */
TimeSeries WhiteNoise::getFuture(const UnsignedLong stepNumber) const
{
  if (stepNumber == 0) return TimeSeries(0, dimension_);
  /* TimeGrid associated with the possible future */
  const NumericalScalar timeStep(timeGrid_.getStep());
  const RegularGrid futurTimeGrid(timeGrid_.getEnd(), timeStep, stepNumber);
  return TimeSeries(futurTimeGrid, distribution_.getSample(stepNumber));
}

/* Get the marginal process corresponding to the i-th marginal component */
WhiteNoise::Implementation WhiteNoise::getMarginalProcess(const UnsignedLong i) const
{
  if (i >= getDimension()) throw InvalidArgumentException(HERE) << "The index of a marginal process must be in the range [0, dim-1]";
  return WhiteNoise(distribution_.getMarginal(i), timeGrid_).clone();
}

/* Get the marginal random vector corresponding to indices components */
WhiteNoise::Implementation WhiteNoise::getMarginalProcess(const Indices & indices) const
{
  if (!indices.check(getDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal process must be in the range [0, dim-1] and  must be different";
  return new WhiteNoise(distribution_.getMarginal(indices), timeGrid_);
}

/* Distribution accessor */
Distribution WhiteNoise::getDistribution() const
{
  return distribution_;
}

/* Distribution accessor */
void WhiteNoise::setDistribution(const Distribution & distribution)
{
  // Check if the given distribution has a null mean
  const NumericalPoint mean(distribution.getMean());
  if (mean.norm() > ResourceMap::GetAsNumericalScalar( "WhiteNoise-MeanEpsilon" )) throw InvalidArgumentException(HERE) << "Error: the given distribution has a mean=" << mean.__str__() << " which is not null.";
  distribution_ = distribution;
  setDimension(distribution.getDimension());
}

/* Method save() stores the object through the StorageManager */
void WhiteNoise::save(Advocate & adv) const
{
  ProcessImplementation::save(adv);
  adv.saveAttribute( "distribution_", distribution_ );
}

/* Method load() reloads the object from the StorageManager */
void WhiteNoise::load(Advocate & adv)
{
  ProcessImplementation::load(adv);
  adv.loadAttribute( "distribution_", distribution_ );
}
END_NAMESPACE_OPENTURNS
