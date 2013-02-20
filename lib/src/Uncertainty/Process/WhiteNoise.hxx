//                                               -*- C++ -*-
/**
 *  @file  WhiteNoise.hxx
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 *  Id      WhiteNoise.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_WHITENOISE_HXX
#define OPENTURNS_WHITENOISE_HXX

#include "ProcessImplementation.hxx"
#include "Distribution.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class WhiteNoise
 *
 * An interface class for composite White Noise
 */
class WhiteNoise
  : public ProcessImplementation
{
  CLASSNAME;

public:

  /** Some typedefs to ease reading */

  /** Default constructor */
  WhiteNoise(const String & name = DefaultName);

  /** Standard constructor */
  WhiteNoise(const Distribution & distribution,
             const String & name = DefaultName);

  /** Standard constructor */
  WhiteNoise(const Distribution & distribution,
             const RegularGrid & timeGrid,
             const String & name = DefaultName);

  /** Virtual constructor */
  WhiteNoise * clone() const;

  /** String converter */
  String __repr__() const;

  /** String converter  - pretty print */
  String __str__(const String & offset = "") const;

  /** Is the underlying stationary ? */
  Bool isStationary() const;

  /** Is the underlying a Normal process ? */
  Bool isNormal() const;

  /** Realization accessor */
  TimeSeries getRealization() const;

  /** Continuation of the last realization on a given number of steps */
  using ProcessImplementation::getFuture;
  TimeSeries getFuture(const UnsignedLong stepNumber) const;

  /** Get the marginal process corresponding to the i-th marginal component */
  Implementation getMarginalProcess(const UnsignedLong i) const;

  /** Get the marginal process corresponding to indices components */
  Implementation getMarginalProcess(const Indices & indices) const;

  /** Distribution accessor */
  Distribution getDistribution() const;

  /** Distribution accessor */
  void setDistribution(const Distribution & distribution);

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /** The distribution of the WhiteNoise */
  Distribution distribution_;

}; /* class WhiteNoise */
END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_WHITENOISE_HXX */
