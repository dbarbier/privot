//                                               -*- C++ -*-
/**
 *  @file  DynamicalFunctionImplementation.cxx
 *  @brief Abstract top-level class for all distributions
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
 *  @date   2011-05-24 19:30:41 +0200 (mar. 24 mai 2011)
 */
#include "DynamicalFunctionImplementation.hxx"
#include "Exception.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(DynamicalFunctionImplementation);


static Factory<DynamicalFunctionImplementation> RegisteredFactory("DynamicalFunctionImplementation");

/* Default constructor */
DynamicalFunctionImplementation::DynamicalFunctionImplementation()
  : PersistentObject(),
    inputDescription_(),
    outputDescription_(),
    callsNumber_(0)
{
  // Nothing to do
}

/* Virtual constructor */
DynamicalFunctionImplementation * DynamicalFunctionImplementation::clone() const
{
  return new DynamicalFunctionImplementation(*this);
}

/* Comparison operator */
Bool DynamicalFunctionImplementation::operator ==(const DynamicalFunctionImplementation & other) const
{
  return true;
}

/* String converter */
String DynamicalFunctionImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << DynamicalFunctionImplementation::GetClassName()
      << " name=" << getName()
      << " input description=" << inputDescription_
      << " output description=" << outputDescription_
      << " number of calls=" << callsNumber_;
  return oss;
}

/* String converter */
String DynamicalFunctionImplementation::__str__(const String & offset) const
{
  return OSS(false) << offset << __repr__();
}

/* Get the i-th marginal function */
DynamicalFunctionImplementation::Implementation DynamicalFunctionImplementation::getMarginal(const UnsignedLong i) const
{
  if (i >= getOutputDimension()) throw InvalidArgumentException(HERE) << "Error: the index of a marginal function must be in the range [0, outputDimension-1]";
  return getMarginal(Indices(1, i));
}

/* Get the function corresponding to indices components */
DynamicalFunctionImplementation::Implementation DynamicalFunctionImplementation::getMarginal(const Indices & indices) const
{
  if (!indices.check(getOutputDimension() - 1)) throw InvalidArgumentException(HERE) << "Error: the indices of a marginal function must be in the range [0, outputDimension-1] and  must be different";
  throw NotYetImplementedException(HERE);
}

/* Input description Accessor */
void DynamicalFunctionImplementation::setInputDescription(const Description & inputDescription)
{
  inputDescription_ = inputDescription;
}

Description DynamicalFunctionImplementation::getInputDescription() const
{
  return inputDescription_;
}

/* Output description Accessor */
void DynamicalFunctionImplementation::setOutputDescription(const Description & outputDescription)
{
  outputDescription_ = outputDescription;
}

Description DynamicalFunctionImplementation::getOutputDescription() const
{
  return outputDescription_;
}

/* Operator () */
TimeSeries DynamicalFunctionImplementation::operator() (const TimeSeries & inTS) const
{
  return TimeSeries();
}

/* Operator () */
ProcessSample DynamicalFunctionImplementation::operator() (const ProcessSample & inPS) const
{
  if (inPS.getDimension() != getInputDimension()) throw InvalidArgumentException(HERE) << "Error: the given process sample has an invalid dimension. Expect a dimension " << getInputDimension() << ", got " << inPS.getDimension();
  const UnsignedLong size(inPS.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: the given process sample has a size of 0.";
  ProcessSample outSample(computeOutputTimeGrid(inPS[0].getTimeGrid()), size, getOutputDimension());
  // Simple loop over the evaluation operator based on time series
  // The calls number is updated by these calls
  for (UnsignedLong i = 0; i < size; ++i)
    outSample[i] = operator()(inPS[i]);
  return outSample;
}

/* Accessor for input point dimension */
UnsignedLong DynamicalFunctionImplementation::getInputDimension() const
{
  return inputDescription_.getSize();
}

/* Accessor for output point dimension */
UnsignedLong DynamicalFunctionImplementation::getOutputDimension() const
{
  return outputDescription_.getSize();
}

/* Number of calls to the dynamical function */
UnsignedLong DynamicalFunctionImplementation::getCallsNumber() const
{
  return callsNumber_;
}

/* Compute the output time grid based on a given input time grid */
RegularGrid DynamicalFunctionImplementation::computeOutputTimeGrid(const RegularGrid & inputTimeGrid) const
{
  return inputTimeGrid;
}

/* Method save() stores the object through the StorageManager */
void DynamicalFunctionImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "inputDescription_", inputDescription_ );
  adv.saveAttribute( "outputDescription_", outputDescription_ );
  adv.saveAttribute( "callsNumber_", callsNumber_ );
}

/* Method load() reloads the object from the StorageManager */
void DynamicalFunctionImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "inputDescription_", inputDescription_ );
  adv.loadAttribute( "outputDescription_", outputDescription_ );
  adv.loadAttribute( "callsNumber_", callsNumber_ );
}




END_NAMESPACE_OPENTURNS
