//                                               -*- C++ -*-
/**
 *  @file  RandomWalk.hxx
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
 *  @date   2011-10-19 22:47:28 +0200 (mer. 19 oct. 2011)
 *  Id      RandomWalk.hxx 2157 2011-10-19 20:47:28Z lebrun
 */
#ifndef OPENTURNS_RANDOMWALK_HXX
#define OPENTURNS_RANDOMWALK_HXX

#include "ProcessImplementation.hxx"
#include "Process.hxx"
#include "Pointer.hxx"
#include "Distribution.hxx"
#include "NumericalPoint.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class RandomWalk
 *
 * An interface class for composite White Noise
 */
class RandomWalk
  : public ProcessImplementation
{
  CLASSNAME;

public:

  /** Default constructor */
  RandomWalk(const String & name = DefaultName);

  /** Standard constructor */
  RandomWalk(const NumericalPoint & origin,
             const Distribution & distribution,
             const String & name = DefaultName);

  /** Standard constructor */
  RandomWalk(const NumericalPoint & origin,
             const Distribution & distribution,
             const RegularGrid & timeGrid,
             const String & name = DefaultName);

  /** Virtual constructor */
  RandomWalk * clone() const;

  /** String converter */
  String __repr__() const;

  /** Is the underlying stationary ? */
  Bool isStationary() const;

  /** Is the underlying a Normal process ? */
  Bool isNormal() const;

  /** Realization accessor */
  TimeSeries getRealization() const;

  /** Continuation of the last realization on a given number of steps */
  using ProcessImplementation::getFuture;
  TimeSeries getFuture(const UnsignedLong stepNumber) const;

  /** Get the process corresponding to the i-th marginal component */
  Implementation getMarginalProcess(const UnsignedLong i) const;

  /** Get the marginal process corresponding to indices components */
  Implementation getMarginalProcess(const Indices & indices) const;

  /** Distribution accessor */
  Distribution getDistribution() const;

  /** Distribution accessor */
  void setDistribution(const Distribution & distribution);

  /** Origin accessor */
  NumericalPoint getOrigin() const;

  /** Origin accessor */
  void setOrigin(const NumericalPoint & origin);

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /** The origin of the walk */
  NumericalPoint origin_;

  /** The distribution of the walk steps */
  Distribution distribution_;

  /** The current position of the walk */
  mutable NumericalPoint currentPosition_;

}; /* class RandomWalk */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_RANDOMWALK_HXX */
