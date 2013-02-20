//                                               -*- C++ -*-
/**
 *  @file  Process.hxx
 *  @brief The interface class that implements all process
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
 *  Id      Process.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_PROCESS_HXX
#define OPENTURNS_PROCESS_HXX

#include "TypedInterfaceObject.hxx"
#include "Pointer.hxx"
#include "ProcessImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class Process
 *
 * The class that implements all processs
 */
class Process
  : public TypedInterfaceObject<ProcessImplementation>
{
  CLASSNAME;

public:

  /* Some typedefs for easy reading */
  typedef ProcessImplementation::Implementation Implementation;

  /** Default constructor */
  Process();

  /** Copy constructors */
  Process(const ProcessImplementation & implementation,
          const String & name = DefaultName);


  /** Constructor from implementation */
  Process(const Implementation & p_implementation,
          const String & name = DefaultName);

#ifndef SWIG
  /** Constructor from implementation pointer */
  Process(ProcessImplementation * p_implementation,
          const String & name = DefaultName);
#endif

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** Description Accessor */
  void setDescription(const Description & description);
  Description getDescription() const;

  /** Is the underlying a normal process ? */
  Bool isNormal() const;

  /** Is the underlying a stationary process ? */
  Bool isStationary() const;

  /** Is the underlying a composite process ? */
  Bool isComposite() const;

  /** Dimension accessor */
  UnsignedLong getDimension() const;

  /** Realization accessor */
  TimeSeries getRealization() const;

  /** TimeGrid accessor */
  RegularGrid getTimeGrid() const;
  void setTimeGrid (const RegularGrid & timeGrid);

  /** Process sample accessors */
  ProcessSample getSample(const UnsignedLong size) const;

  /** Continuation of the last realization on a given number of steps */
  TimeSeries getFuture(const UnsignedLong stepNumber) const;
  ProcessSample getFuture(const UnsignedLong stepNumber,
                          const UnsignedLong size) const;

  /** Get the process corresponding to the i-th marginal component */
  Process getMarginalProcess(const UnsignedLong i) const;

  /** Get the marginal process corresponding to indices components */
  Process getMarginalProcess(const Indices & indices) const;


}; /* class Process */
END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_RANDOMVECTOR_HXX */
