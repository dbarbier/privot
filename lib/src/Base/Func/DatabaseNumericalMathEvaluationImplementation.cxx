//                                               -*- C++ -*-
/**
 *  @file  DatabaseNumericalMathEvaluationImplementation.cxx
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
 */
#include "DatabaseNumericalMathEvaluationImplementation.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "Description.hxx"
#include "Exception.hxx"
#include "Os.hxx"

BEGIN_NAMESPACE_OPENTURNS

typedef NumericalMathEvaluationImplementation::CacheKeyType             CacheKeyType;
typedef NumericalMathEvaluationImplementation::CacheValueType           CacheValueType;
typedef NumericalMathEvaluationImplementation::CacheType                CacheType;

CLASSNAMEINIT(DatabaseNumericalMathEvaluationImplementation);

static Factory<DatabaseNumericalMathEvaluationImplementation> RegisteredFactory("DatabaseNumericalMathEvaluationImplementation");


/* Default constructor */
DatabaseNumericalMathEvaluationImplementation::DatabaseNumericalMathEvaluationImplementation()
  : NumericalMathEvaluationImplementation()
{
  // Nothing to do
}

/* Default constructor */
DatabaseNumericalMathEvaluationImplementation::DatabaseNumericalMathEvaluationImplementation(const NumericalSample & inputSample,
                                                                                             const NumericalSample & outputSample,
                                                                                             const Bool activateCache)
  : NumericalMathEvaluationImplementation()
{
  setSample(inputSample, outputSample, activateCache);
}


/* Virtual constructor */
DatabaseNumericalMathEvaluationImplementation * DatabaseNumericalMathEvaluationImplementation::clone() const
{
  return new DatabaseNumericalMathEvaluationImplementation(*this);
}


/* Comparison operator */
Bool DatabaseNumericalMathEvaluationImplementation::operator ==(const DatabaseNumericalMathEvaluationImplementation & other) const
{
  return (inputSample_ == other.inputSample_) && (outputSample_ == other.outputSample_);
}


/* String converter */
String DatabaseNumericalMathEvaluationImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << DatabaseNumericalMathEvaluationImplementation::GetClassName()
      << " name=" << getName()
      << " inputSample=" << inputSample_
      << " outputSample" << outputSample_;
  return oss;
}

String DatabaseNumericalMathEvaluationImplementation::__str__( const String & offset ) const
{
  OSS oss;
  oss << DatabaseNumericalMathEvaluationImplementation::GetClassName() << Os::GetEndOfLine()
      << offset << "  input sample :" << Os::GetEndOfLine() << inputSample_.__str__(offset) << Os::GetEndOfLine()
      << offset << "  output sample :" << Os::GetEndOfLine() << outputSample_.__str__(offset);
  return oss;
}

/* Input sample accessor */
void DatabaseNumericalMathEvaluationImplementation::setInputSample(const NumericalSample &inputSample)
{
  setSample( inputSample, outputSample_ );
}

NumericalSample DatabaseNumericalMathEvaluationImplementation::getInputSample() const
{
  return inputSample_;
}


/* Output sample accessor */
void DatabaseNumericalMathEvaluationImplementation::setOutputSample(const NumericalSample &outputSample)
{
  setSample( inputSample_, outputSample );
}

NumericalSample DatabaseNumericalMathEvaluationImplementation::getOutputSample() const
{
  return outputSample_;
}


void DatabaseNumericalMathEvaluationImplementation::setSample(const NumericalSample & inputSample,
                                                              const NumericalSample & outputSample,
                                                              const Bool activateCache)
{
  const UnsignedLong size = inputSample.getSize();

  if(size == 0)
    throw InvalidDimensionException(HERE) << "Empty input sample";

  if (outputSample.getSize() == 0)
    throw InvalidDimensionException(HERE) << "Empty output sample";

  if (inputSample.getSize() != outputSample.getSize())
    throw InvalidDimensionException(HERE) << "Input and output samples have different sizes (in=" << inputSample.getSize() << " out=" << outputSample.getSize() << ")";

  inputSample_ = inputSample;
  outputSample_ = outputSample;

  // Don't activate the cache systematically as it can take a significant amount of time for large samples
  if (activateCache)
    {
      addCacheContent( inputSample, outputSample );
      enableCache();
    }
  else disableCache();
}


/* Here is the interface that all derived class must implement */

/* Operator () */
NumericalPoint DatabaseNumericalMathEvaluationImplementation::operator()( const NumericalPoint & inP ) const
{
  const UnsignedLong inputDimension = getInputDimension();
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inP.getDimension();
  NumericalPoint result;
  CacheKeyType inKey(inP.getCollection());

  if ( isCacheEnabled() && p_cache_->hasKey(inKey) )
    {
      result = NumericalPoint::ImplementationType( p_cache_->find(inKey) );
    }
  else
    {
      // find the nearest
      const UnsignedLong size(inputSample_.getSize());
      UnsignedLong nearestPointIndex(0);
      NumericalScalar nearestPointDistance( (inputSample_[0] - inP).norm() );
      for ( UnsignedLong i = 1; i < size; ++ i )
        {
          NumericalScalar pointDistance( (inputSample_[i] - inP).norm() );
          if (pointDistance < nearestPointDistance)
            {
              nearestPointDistance = pointDistance;
              nearestPointIndex = i;
            }
        }

      result = outputSample_[nearestPointIndex];
    }
  ++ callsNumber_;
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inP);
      outputStrategy_.store(result);
    }
  return result;
}

/* Accessor for input point dimension */
UnsignedLong DatabaseNumericalMathEvaluationImplementation::getInputDimension() const
{
  return inputSample_.getDimension();
}


/* Accessor for output point dimension */
UnsignedLong DatabaseNumericalMathEvaluationImplementation::getOutputDimension() const
{
  return outputSample_.getDimension();
}


/* Method save() stores the object through the StorageManager */
void DatabaseNumericalMathEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute("inputSample_", inputSample_);
  adv.saveAttribute("outputSample_", outputSample_);
}

/* Method load() reloads the object from the StorageManager */
void DatabaseNumericalMathEvaluationImplementation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute("inputSample_", inputSample_);
  adv.loadAttribute("outputSample_", outputSample_);
}

END_NAMESPACE_OPENTURNS
