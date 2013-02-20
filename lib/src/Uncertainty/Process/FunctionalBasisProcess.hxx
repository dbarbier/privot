//                                               -*- C++ -*-
/**
 *  @file  FunctionalBasisProcess.hxx
 *  @brief A class which implements the FunctionalBasisProcess process
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
 *  @date   2012-04-18 17:56:46 +0200 (mer. 18 avril 2012)
 *  Id      FunctionalBasisProcess.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_FUNCTIONALBASISPROCESS_HXX
#define OPENTURNS_FUNCTIONALBASISPROCESS_HXX

#include "ProcessImplementation.hxx"
#include "Basis.hxx"
#include "Distribution.hxx"
#include "NumericalPoint.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class FunctionalBasisProcess
 *
 * An interface class for composite White Noise
 */
class FunctionalBasisProcess
  : public ProcessImplementation
{
  CLASSNAME;

public:

  /** Some typedefs to ease reading */

  /** Default constructor */
  FunctionalBasisProcess(const String & name = DefaultName);

  /** Standard constructor */
  FunctionalBasisProcess(const Distribution & distribution,
                         const Basis & basis,
                         const String & name = DefaultName);

  /** Standard constructor */
  FunctionalBasisProcess(const Distribution & distribution,
                         const Basis & basis,
                         const RegularGrid & timeGrid,
                         const String & name = DefaultName);

  /** Virtual constructor */
  FunctionalBasisProcess * clone() const;

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

  /** Get the marginal process corresponding to the i-th marginal component */
  Implementation getMarginalProcess(const UnsignedLong i) const;

  /** Get the marginal process corresponding to indices components */
  Implementation getMarginalProcess(const Indices & indices) const;

  /** Distribution accessor */
  Distribution getDistribution() const;

  /** Distribution accessor */
  void setDistribution(const Distribution & distribution);

  /** Basis accessor */
  Basis getBasis() const;

  /** Basis accessor */
  void setBasis(const Basis & basis);

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /** The distribution of the coefficients */
  Distribution distribution_;

  /** The functional basis */
  Basis basis_;

  /** The last state */
  mutable NumericalPoint state_;

}; /* class FunctionalBasisProcess */
END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_FUNCTIONALBASISPROCESS_HXX */
