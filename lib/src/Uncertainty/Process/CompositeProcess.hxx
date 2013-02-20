//                                               -*- C++ -*-
/**
 *  @file  CompositeProcess.hxx
 *  @brief A class which implements the CompositeProcess process
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
 *  Id      CompositeProcess.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_COMPOSITEPROCESS_HXX
#define OPENTURNS_COMPOSITEPROCESS_HXX

#include "ProcessImplementation.hxx"
#include "Process.hxx"
#include "DynamicalFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class CompositeProcess
 *
 * The implementation of the CompositeProcess class
 */
class CompositeProcess
  : public ProcessImplementation
{
  CLASSNAME;

public:


  /** Default constructor */
  CompositeProcess(const String & name = DefaultName);

  /** Standard constructor */
  CompositeProcess(const DynamicalFunction & function,
                   const Antecedent & p_antecedent,
                   const String & name = DefaultName);

  /** Standard constructor */
  CompositeProcess(const DynamicalFunction & function,
                   const Process & antecedent,
                   const String & name = DefaultName);

  /** Virtual constructor */
  CompositeProcess * clone() const;

  /** String converter */
  virtual String __repr__() const;

  /** String converter  - pretty print */
  virtual String __str__(const String & offset = "") const;

  /** Is the underlying a composite process ? */
  Bool isComposite() const;

  /** Realization accessor */
  virtual TimeSeries getRealization() const;

  /** Antecedent accessor */
  Antecedent getAntecedent() const;

  /** Function accessor */
  DynamicalFunction getFunction() const;

  /** Get the random vector corresponding to the i-th marginal component */
  Implementation getMarginalProcess(const UnsignedLong i) const;

  /** Get the marginal random vector corresponding to indices components */
  Implementation getMarginalProcess(const Indices & indices) const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

  /** The function used for realization  */
  DynamicalFunction function_;

  /** The antecedent of the process through the function */
  Antecedent p_antecedent_;

}; /* class CompositeProcess */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COMPOSITEPROCESS_HXX */
