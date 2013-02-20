//                                               -*- C++ -*-
/**
 *  @file  ProcessImplementation.hxx
 *  @brief An interface for all implementation class of process
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 *  Id      ProcessImplementation.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_PROCESSIMPLEMENTATION_HXX
#define OPENTURNS_PROCESSIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"
#include "Indices.hxx"
#include "TimeSeries.hxx"
#include "ProcessSample.hxx"
#include "Pointer.hxx"

BEGIN_NAMESPACE_OPENTURNS


/**
 * @class ProcessImplementation
 *
 * An interface for all implementation class of process
 */
class ProcessImplementation
  : public PersistentObject
{
  CLASSNAME;

public:

  /** Some typedefs to ease reading */
  typedef Pointer<ProcessImplementation>     Antecedent;
  typedef Antecedent                         Implementation;

  /** Default constructor */
  ProcessImplementation(const String & name = DefaultName);

  /** Virtual constructor */
  virtual ProcessImplementation * clone() const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /* Here is the interface that all derived class must implement */

  /** Is the underlying gaussian process ? */
  virtual Bool isNormal() const;

  /** Is the underlying a stationary process ? */
  virtual Bool isStationary() const;

  /** Is the underlying a composite process ? */
  virtual Bool isComposite() const;

  /** Dimension accessor */
  virtual UnsignedLong getDimension() const;

  /** TimeGrid accessor */
  virtual RegularGrid getTimeGrid() const;

  /** TimeGrid accessor */
  virtual void setTimeGrid(const RegularGrid & timeGrid);

  /** Realization accessor */
  virtual TimeSeries getRealization() const;

  /** Process sample accessors */
  virtual ProcessSample getSample(const UnsignedLong size) const;

  /** Continuation of the last realization on a given number of steps */
  virtual TimeSeries getFuture(const UnsignedLong stepNumber) const;
  virtual ProcessSample getFuture(const UnsignedLong stepNumber,
                                  const UnsignedLong size) const;

  /** Get the marginal process corresponding to the i-th marginal component */
  virtual Implementation getMarginalProcess(const UnsignedLong i) const;

  /** Get the marginal process corresponding to indices components */
  virtual Implementation getMarginalProcess(const Indices & indices) const;

  /** Description accessor */
  virtual void setDescription(const Description & description);
  virtual Description getDescription() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

  /** Dimension accessor */
  void setDimension(const UnsignedLong dimension);

  /** Description of each component */
  Description description_;

  /** Dimension of the process */
  UnsignedLong dimension_;

  /** The timeGrid over which the process is defined or discretized */
  RegularGrid timeGrid_;

}; /* class ProcessImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PROCESSIMPLEMENTATION_HXX */
