//                                               -*- C++ -*-
/**
 *  @file  RandomWalk.cxx
 *  @brief A class which implements the RandomWalk process
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
 *  @author lebrun
 *  @date   2011-10-26 13:54:47 +0200 (mer. 26 oct. 2011)
 */

#include "RandomWalk.hxx"
#include "PersistentObjectFactory.hxx"
#include "Normal.hxx"
#include "ResourceMap.hxx"
#include "NumericalPoint.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(RandomWalk);

static Factory<RandomWalk> RegisteredFactory("RandomWalk");

/* Standard constructor */
RandomWalk::RandomWalk(const String & name)
  : ProcessImplementation(name),
    origin_(1),
    distribution_(Normal()),
    currentPosition_(1)
{
  // Nothing to do
}

/* Standard constructor */
RandomWalk::RandomWalk(const NumericalPoint & origin,
                       const Distribution & distribution,
                       const String & name)
  : ProcessImplementation(name),
    origin_(origin),
    distribution_(),
    currentPosition_(origin)
{
  // Set the dimension of the process
  setDimension(origin.getDimension());
  setDistribution(distribution);
}

/* Standard constructor */
RandomWalk::RandomWalk(const NumericalPoint & origin,
                       const Distribution & distribution,
                       const RegularGrid & timeGrid,
                       const String & name)
  : ProcessImplementation(name),
    origin_(origin),
    distribution_(),
    currentPosition_(origin)
{
  // Set the dimension of the process
  setDimension(origin.getDimension());
  setDistribution(distribution);
  setTimeGrid(timeGrid);
}

/* Virtual constructor */
RandomWalk * RandomWalk::clone() const
{
  return new RandomWalk(*this);
}

/* String converter */
String RandomWalk::__repr__() const
{
  OSS oss;
  oss << "class=" << RandomWalk::GetClassName()
      << ", origin=" << origin_
      << ", distribution=" << distribution_;
  return oss;
}

/* Is the underlying stationary ? */
Bool RandomWalk::isStationary() const
{
  return false;
}

/* Is the underlying gaussian ? */
Bool RandomWalk::isNormal() const
{
  return (distribution_.getImplementation()->getClassName() == "Normal");
}

/* Realization accessor */
TimeSeries RandomWalk::getRealization() const
{
  const UnsignedLong size(timeGrid_.getN());
  NumericalSample data(size, origin_);
  for (UnsignedLong i = 1; i < size; ++i) data[i] = data[i - 1] + distribution_.getRealization();
  currentPosition_ = data[data.getSize() - 1];
  return TimeSeries(timeGrid_, data);
}

/* Compute the next steps of a random walk */
TimeSeries RandomWalk::getFuture(const UnsignedLong stepNumber) const
{
  if (stepNumber == 0) return TimeSeries(0, dimension_);
  /* TimeGrid associated with the possible future */
  const NumericalScalar timeStep(timeGrid_.getStep());
  const RegularGrid futurTimeGrid(timeGrid_.getEnd(), timeStep, stepNumber);
  NumericalSample data(stepNumber, dimension_);
  NumericalPoint previous(currentPosition_);
  for (UnsignedLong i = 0; i < stepNumber; ++i)
    {
      data[i] = previous + distribution_.getRealization();
      previous = data[i];
    }
  return TimeSeries(futurTimeGrid, data);
}

/* Get the random vector corresponding to the i-th marginal component */
RandomWalk::Implementation RandomWalk::getMarginalProcess(const UnsignedLong i) const
{
  if (i >= dimension_) throw InvalidArgumentException(HERE) << "The index of a marginal process must be in the range [0, dim-1]";
  return new RandomWalk(NumericalPoint(1, origin_[i]), distribution_.getMarginal(i), timeGrid_);
}

/* Get the marginal process corresponding to indices components */
RandomWalk::Implementation RandomWalk::getMarginalProcess(const Indices & indices) const
{
  if (!indices.check(getDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal process must be in the range [0, dim-1] and  must be different";
  const UnsignedLong size(indices.getSize());
  NumericalPoint marginalOrigin(size);
  for (UnsignedLong i = 0; i < size; ++i) marginalOrigin[i] = origin_[indices[i]];
  return new RandomWalk(marginalOrigin, distribution_.getMarginal(indices), timeGrid_);
}

/* Distribution accessor */
Distribution RandomWalk::getDistribution() const
{
  return distribution_;
}

/* Distribution accessor */
void RandomWalk::setDistribution(const Distribution & distribution)
{
  if (distribution.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given distribution has a dimension=" << distribution.getDimension() << " incompatible with the process dimension=" << dimension_;
  distribution_ = distribution;
}

/* Origin accessor */
NumericalPoint RandomWalk::getOrigin() const
{
  return origin_;
}

void RandomWalk::setOrigin(const NumericalPoint & origin)
{
  if (origin.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given origin has a dimension=" << origin.getDimension() << " incompatible with the process dimension=" << dimension_;
  origin_ = origin;
}

/* Method save() stores the object through the StorageManager */
void RandomWalk::save(Advocate & adv) const
{
  ProcessImplementation::save(adv);
  adv.saveAttribute( "distribution_", distribution_ );
  adv.saveAttribute( "origin_", origin_ );
  adv.saveAttribute( "currentPosition_", currentPosition_ );
}

/* Method load() reloads the object from the StorageManager */
void RandomWalk::load(Advocate & adv)
{
  ProcessImplementation::load(adv);
  adv.loadAttribute( "distribution_", distribution_ );
  adv.loadAttribute( "origin_", origin_ );
  adv.loadAttribute( "currentPosition_", currentPosition_ );
}

END_NAMESPACE_OPENTURNS
