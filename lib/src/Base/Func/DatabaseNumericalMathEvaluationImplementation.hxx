//                                               -*- C++ -*-
/**
 *  @file  DatabaseNumericalMathEvaluationImplementation.hxx
 *  @brief An implementation from samples
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
 *  @date   2008-05-21 17:44:02 +0200 (Wed, 21 May 2008)
 *  Id      Object.hxx 818 2008-05-21 15:44:02Z dutka
 */
#ifndef OPENTURNS_DATABASENUMERICALMATHFUNCTIONEVALUATIONIMPLEMENTATION_HXX
#define OPENTURNS_DATABASENUMERICALMATHFUNCTIONEVALUATIONIMPLEMENTATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"
#include "Collection.hxx"
#include "PersistentCollection.hxx"
#include "NumericalMathFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS


/**
 * @class DatabaseNumericalMathEvaluationImplementation
 * Provided two samples X (input) and Y (output), returns Yi when evaluated on Xi.
 */
class DatabaseNumericalMathEvaluationImplementation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
  friend class NumericalMathFunctionImplementation;

public:

  /** Default constructor */
  DatabaseNumericalMathEvaluationImplementation();

  /** Default constructor */
  DatabaseNumericalMathEvaluationImplementation(const NumericalSample & inputSample,
                                                const NumericalSample & outputSample,
                                                const Bool activateCache = true);

  /** Virtual constructor */
  virtual DatabaseNumericalMathEvaluationImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const DatabaseNumericalMathEvaluationImplementation & other) const;

  /** String converter */
  String __repr__() const;
  String __str__( const String & offset = "" ) const;

  /** Input sample accessor */
  void setInputSample(const NumericalSample & inputSample);
  NumericalSample getInputSample() const;

  /** Output sample accessor */
  void setOutputSample(const NumericalSample & outputSample);
  NumericalSample getOutputSample() const;

  /* Here is the interface that all derived class must implement */

  /** Operator () */
  virtual NumericalPoint operator()(const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  virtual UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  virtual UnsignedLong getOutputDimension() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


protected:
  /** Set cached sample */
  void setSample(const NumericalSample & inputSample,
                 const NumericalSample & outputSample,
                 const Bool activateCache = true);

  NumericalSample inputSample_;
  NumericalSample outputSample_;

private:


}; /* class DatabaseNumericalMathEvaluationImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DATABASENUMERICALMATHFUNCTIONEVALUATIONIMPLEMENTATION_HXX */