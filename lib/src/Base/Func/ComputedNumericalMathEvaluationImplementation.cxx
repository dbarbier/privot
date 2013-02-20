//                                               -*- C++ -*-
/**
 * @file  ComputedNumericalMathEvaluationImplementation.cxx
 * @brief Abstract top-level class for all distributions
 *
 * Copyright (C) 2005-2013 EDF
 *
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 *
 * @author schueller
 * @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */

#include <set>

#include "ComputedNumericalMathEvaluationImplementation.hxx"
#include "PersistentObjectFactory.hxx"
#include "Log.hxx"
#include "WrapperData.hxx"
#include "WrapperObject.hxx"

BEGIN_NAMESPACE_OPENTURNS

typedef NumericalMathEvaluationImplementation::CacheKeyType             CacheKeyType;
typedef NumericalMathEvaluationImplementation::CacheValueType           CacheValueType;
typedef NumericalMathEvaluationImplementation::CacheType                CacheType;

CLASSNAMEINIT(ComputedNumericalMathEvaluationImplementation);

static Factory<ComputedNumericalMathEvaluationImplementation> RegisteredFactory("ComputedNumericalMathEvaluationImplementation");


/* Default constructor */
ComputedNumericalMathEvaluationImplementation::ComputedNumericalMathEvaluationImplementation(const String & name,
                                                                                             const WrapperFile & file)
  : NumericalMathEvaluationImplementation(),
    p_function_(0),
    p_state_(0)
{
  setName(name);
  const WrapperData data = file.getWrapperData();
  if (! data.isValid()) throw WrapperInternalException(HERE) << "The wrapper data are not valid";

  if (data.getFunctionDescription().provided_)
    {
      p_function_.reset(new WrapperObject( data.getLibraryPath(),
                                           data.getFunctionDescription().name_,
                                           data,
                                           WrapperObject::FUNCTION ));
    }

  if (p_function_.isNull()) throw WrapperInternalException(HERE) << "Unable to allocate wrapper";

  // Initialize the state into the wrapper
  p_state_ = p_function_->createNewState();
  NumericalMathEvaluationImplementation::setDescription(getDescription());

  // Activate the cache only if the external code is expensive: only the user knows it.
  NumericalMathEvaluationImplementation::disableCache();
}


/* Copy constructor */
ComputedNumericalMathEvaluationImplementation::ComputedNumericalMathEvaluationImplementation(const ComputedNumericalMathEvaluationImplementation & other)
  : NumericalMathEvaluationImplementation(other),
    p_function_(other.p_function_),
    p_state_(0)
{
  if (p_function_.isNull()) throw WrapperInternalException(HERE) << "Unable to allocate wrapper";

  // Initialize the state into the wrapper
  p_state_ = p_function_->createNewState();
}


/* Virtual constructor */
ComputedNumericalMathEvaluationImplementation * ComputedNumericalMathEvaluationImplementation::clone() const
{
  return new ComputedNumericalMathEvaluationImplementation(*this);
}

/* Destructor */
ComputedNumericalMathEvaluationImplementation::~ComputedNumericalMathEvaluationImplementation()
{
  // Call the finalization function before destruction
  p_function_->finalize( p_state_ );

  // Delete the state into the wrapper
  p_function_->deleteState( p_state_ );
}


/* Comparison operator */
Bool ComputedNumericalMathEvaluationImplementation::operator ==(const ComputedNumericalMathEvaluationImplementation & other) const
{
  return true;
}


/* String converter */
String ComputedNumericalMathEvaluationImplementation::__repr__() const
{
  return OSS() << "class=" << ComputedNumericalMathEvaluationImplementation::GetClassName()
               << " name=" << getName();
}

/* String converter */
String ComputedNumericalMathEvaluationImplementation::__str__(const String & offset) const
{
  return OSS(false) << offset << "ComputedNumericalMathEvaluationImplementation bound to wrapper '"
                    << p_function_->__str__() << "'";
}





/* State accessor */
void * ComputedNumericalMathEvaluationImplementation::getState() const
{
  return p_state_;
}






/* Here is the interface that all derived class must implement */

/* Operator () */
NumericalPoint
ComputedNumericalMathEvaluationImplementation::operator() (const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inP.getDimension();
  NumericalPoint result;
  try
    {
      // First, initialize the external code on first invocation
      if (callsNumber_ == 0) p_function_->initialize( p_state_ );
      // Specific code if cache is enabled
      if (p_cache_->isEnabled())
        {
          CacheKeyType inKey = inP.getCollection();
          // Execute the wrapper
          if ( p_cache_->hasKey( inKey ) )
            {
              result = NumericalPoint::ImplementationType( p_cache_->find( inKey ) );
            }
          else
            {
              ++callsNumber_;
              result = p_function_->execute( p_state_, inP );
              CacheValueType outValue(result.getCollection());
              p_cache_->add( inKey, outValue );
            }
        } // If cache is enabled
      else
        {
          ++callsNumber_;
          result = p_function_->execute( p_state_, inP );
        } // Cache disabled
      if (isHistoryEnabled_)
        {
          inputStrategy_.store(inP);
          outputStrategy_.store(result);
        }
      return result;
    } // try
  catch (InvalidArgumentException & ex)
    {
      throw;
    }
  catch (WrapperInternalException & ex)
    {
      throw InternalException(HERE) << ex;
    }
}

/* Operator () */
NumericalSample
ComputedNumericalMathEvaluationImplementation::operator() (const NumericalSample & inS) const
{
  try
    {
      const UnsignedLong size(inS.getSize());
      // The sample out will store all the results as if there was no specific action for multiple input points or already computed values
      NumericalSample result( size, getOutputDimension() );
      // First, initialize the external code on first invocation
      if (callsNumber_ == 0) p_function_->initialize( p_state_ );
      // If the cache is enabled, it means that each evaluation is costly, so avoid any evaluation as much as possible
      if (p_cache_->isEnabled())
        {
          // First, remove all the points that have already been computed
          // We build a new sample without the already computed points
          NumericalSample newIn( 0, inS.getDimension() );
          for(UnsignedLong i = 0; i < size; ++i) if ( ! p_cache_->hasKey( inS[i].getCollection() ) ) newIn.add( inS[i] );
          const UnsignedLong newSize(newIn.getSize());
          // Execute the wrapper if needed
          NumericalSample newOut( 0, getOutputDimension() );
          NumericalSample values( 0, inS.getDimension() );

          // If there is still something to do
          if (newSize > 0)
            {
              std::set<NumericalPoint> sortedValues;
              for(UnsignedLong i = 0; i < newSize; ++i) sortedValues.insert( newIn[i] );
              for(std::set<NumericalPoint>::const_iterator it = sortedValues.begin(); it != sortedValues.end(); ++it) values.add( *it );

              callsNumber_ += newSize;
              newOut = p_function_->execute( p_state_, values );
            } // If there is something to do

          // We use a secondary cache to hold the computed values
          CacheType tempCache( values.getSize() );
          tempCache.enable();
          for(UnsignedLong i = 0; i < values.getSize(); ++i) tempCache.add( values[i].getCollection(), newOut[i].getCollection() );

          // We gather the computed values into a new output sample
          for(UnsignedLong i = 0; i < size; ++i)
            {
              // The output values were either in the cache before the call to the evaluation operator
              if ( p_cache_->hasKey( inS[i].getCollection() ) )
                {
                  result[i] = NumericalPoint::ImplementationType( p_cache_->find( inS[i].getCollection() ) );
                }
              // or they are in the new temporary cache that store the unique points that have been evaluated
              else if ( tempCache.hasKey( inS[i].getCollection() ) )
                {
                  result[i] = NumericalPoint::ImplementationType( tempCache.find( inS[i].getCollection() ) );
                }
              // or something strange occured
              else throw InternalException(HERE) << "Error in computing sample. Some values can not be retrieved from cache.";
            } // Loop over the output sample
          // We add the computed values into the cache AFTER having read the cache because
          // older values may be flushed
          //for(UnsignedLong i=0; i<newIn.getSize(); ++i) p_cache_->add( newIn[i], newOut[i] );
          p_cache_->merge( tempCache );
        } // If the cache is enabled
      else
        {
          callsNumber_ += size;
          result = p_function_->execute( p_state_, inS );
        }
      // Store the computations in the history if asked for
      if (isHistoryEnabled_)
        {
          inputStrategy_.store(inS);
          outputStrategy_.store(result);
        }
      result.setDescription(getOutputDescription());
      // return the gathered sample
      return result;
    } // try
  catch (InvalidArgumentException & ex)
    {
      throw;
    }
  catch (WrapperInternalException & ex)
    {
      throw InternalException(HERE) << ex;
    }
}



/* Accessor for input point dimension */
UnsignedLong ComputedNumericalMathEvaluationImplementation::getInputDimension() const
{
  UnsignedLong inputDimension = 0;

  try
    {
      inputDimension = p_function_->getInNumericalPointDimension( p_state_ );
    }
  catch (WrapperInternalException & ex)
    {
      throw InternalException(HERE) << ex;
    }

  return inputDimension;
}



/* Accessor for output point dimension */
UnsignedLong ComputedNumericalMathEvaluationImplementation::getOutputDimension() const
{
  UnsignedLong outputDimension = 0;

  try
    {
      outputDimension = p_function_->getOutNumericalPointDimension( p_state_ );
    }
  catch (WrapperInternalException & ex)
    {
      throw InternalException(HERE) << ex;
    }

  return outputDimension;
}

/* Accessor for output point dimension */
Description ComputedNumericalMathEvaluationImplementation::getDescription() const
{
  Description description;

  try
    {
      // Here, we get only the description of the input variable
      description = p_function_->getDescription();
      if (description.getSize() == getInputDimension())
        {
          // Put generic names for the output description if something they are missing in the wrapper
          for (UnsignedLong i = 0; i < getOutputDimension(); ++i) description.add(OSS() << "y" << i);
        }
      // If the description does not match the dimensions, error
      if (description.getSize() != getInputDimension() + getOutputDimension()) throw InternalException(HERE) << "Error: the description " << description << " does not match the dimensions of the function. Here, input dimension=" << getInputDimension() << " and output dimension=" << getOutputDimension() << ". Check the wrapper description.";
    }
  catch (WrapperInternalException & ex)
    {
      throw InternalException(HERE) << ex;
    }

  return description;
}

/* Method save() stores the object through the StorageManager */
void ComputedNumericalMathEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void ComputedNumericalMathEvaluationImplementation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load(adv);
  ComputedNumericalMathEvaluationImplementation other( getName(), WrapperFile::FindWrapperByName( getName() ) );
  p_function_ = other.p_function_;
  // Initialize the state into the wrapper
  p_state_ = p_function_->createNewState();
}

END_NAMESPACE_OPENTURNS
