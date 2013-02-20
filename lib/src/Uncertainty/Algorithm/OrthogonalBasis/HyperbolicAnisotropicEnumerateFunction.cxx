//                                               -*- C++ -*-
/**
 *  @file  HyperbolicAnisotropicEnumerateFunction.cxx
 *  @brief The bijective function to select polynomials in the orthogonal basis
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
 *  @date   2008-05-21 11:21:38 +0200 (Wed, 21 May 2008)
 */
#include "HyperbolicAnisotropicEnumerateFunction.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "SpecFunc.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(HyperbolicAnisotropicEnumerateFunction);

static Factory<HyperbolicAnisotropicEnumerateFunction> RegisteredFactory("HyperbolicAnisotropicEnumerateFunction");

/* Default constructor */
HyperbolicAnisotropicEnumerateFunction::HyperbolicAnisotropicEnumerateFunction()
  : EnumerateFunctionImplementation(),
    weight_(),
    q_(0.0)
{
  // Nothing to do
}

/* Parameter constructor */
HyperbolicAnisotropicEnumerateFunction::HyperbolicAnisotropicEnumerateFunction(const UnsignedLong dimension,
                                                                               const NumericalScalar q)
  : EnumerateFunctionImplementation(dimension),
    weight_(dimension, 1.0),
    q_(q)
{
  initialize();
}


/* Parameter constructor */
HyperbolicAnisotropicEnumerateFunction::HyperbolicAnisotropicEnumerateFunction(const NumericalPoint & weight,
                                                                               const NumericalScalar q)
  : EnumerateFunctionImplementation(weight.getDimension()),
    weight_(weight),
    q_(q)
{
  initialize();
}


/* Virtual constrcutor */
HyperbolicAnisotropicEnumerateFunction * HyperbolicAnisotropicEnumerateFunction::clone() const
{
  return new HyperbolicAnisotropicEnumerateFunction(*this);
}


/* String converter */
String HyperbolicAnisotropicEnumerateFunction::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " derived from " << EnumerateFunctionImplementation::__repr__()
               << " q=" << q_
               << " weights=" << weight_;
}


void HyperbolicAnisotropicEnumerateFunction::initialize()
{
  if ( (q_ <= 0.0) || (q_ > 1.0) )
    throw InvalidRangeException( HERE ) << "q parameter should be in range ]0, 1]";

  for ( UnsignedLong i = 0; i < getDimension(); ++ i )
    {
      if ( weight_[i] < 0.0 )
        {
          throw InvalidRangeException( HERE ) << "Anisotropic weights should not be negative.";
        }
    }

  cache_.clear();
  candidates_.clear();
  // insert indice 0, with q-norm 0.0 in the candidate list
  ValueType zero( Indices( getDimension(), 0 ), 0.0 );
  candidates_.insert( candidates_.begin(), zero );
  strataCumulatedCardinal_.add( 1 );
}


/* Returns the q-norm of the indice set */
NumericalScalar HyperbolicAnisotropicEnumerateFunction::computeQNorm(const Indices & indices) const
{
  NumericalScalar result(0.0);
  UnsignedLong dimension(indices.getSize());
  for( UnsignedLong j = 0; j < dimension; ++ j )
    result += pow( indices[j] * weight_[j], q_ );
  return pow( result, 1.0 / q_ );
}

/* Tells if indice1 is a neighbor of indice2 */
Bool HyperbolicAnisotropicEnumerateFunction::isNeighbor(const Indices & indice1,
                                                        const Indices & indice2) const
{
  UnsignedLong diff( 0 );
  for ( UnsignedLong i = 0; i < getDimension(); ++ i )
    {
      diff += (indice1[i] > indice2[i]) ? ( indice1[i] - indice2[i] ) : 0;
    }
  return ( diff < 2 );
}

/*
 * We start from the zero-filled indice set,
 * and for every indice set asked, compute its immediate neighbors and add them to the list of candidates
 * this list contains all the potential next indices, sorted according to their q-norm
 * so the next indice set is the first in the list, i.e. the one closest to the origin
 * then we put it in a vector, allowing for fast retrieval according to the index
 */
Indices HyperbolicAnisotropicEnumerateFunction::operator() (const UnsignedLong index) const
{
  // if we haven't generated enough indices, generate them
  for ( UnsignedLong i = cache_.getSize(); i <= index; ++ i )
    {
      // the current indice is the first candidate in the list as we maintain q-norm sorting
      ValueType current(candidates_.front());

      // move it to cache
      candidates_.pop_front();
      cache_.add( current.first );

      // generate all the neighbors indices
      for( UnsignedLong j = 0; j < getDimension(); ++ j )
        {
          Indices nextIndices( current.first );
          ++ nextIndices[j];
          NumericalScalar nextNorm(computeQNorm( nextIndices ));
          ValueType next( nextIndices, nextNorm );
          IndiceCache::iterator it = candidates_.begin();

          // we'll try to insert the indice in the list according to its q-norm
          while (( it->second < nextNorm ) && ( it != candidates_.end() ))
            {
              ++ it;
            }

          // check if the same indice was already added
          bool duplicate( false );
          const NumericalScalar epsilon(ResourceMap::GetAsNumericalScalar("SpecFunc-Precision"));
          while ( ( fabs( nextNorm - it->second ) < epsilon ) && ( it != candidates_.end() ))
            {
              if ( it->first == nextIndices )
                {
                  duplicate = true;
                }
              ++ it;
            }

          // insert it in the list if not a duplicate
          if ( !duplicate )
            {
              candidates_.insert( it, next );
            }
        }

      // check if the intersection between the current front and the last front is empty
      UnsignedLong lastStrataStart( ( strataCumulatedCardinal_.getSize() > 1 ) ? strataCumulatedCardinal_[ strataCumulatedCardinal_.getSize() - 2 ] : 0 );
      UnsignedLong lastStrataCardinal( getStrataCardinal( strataCumulatedCardinal_.getSize() - 1 ) );
      Bool disJointFront ( true );
      UnsignedLong j( 0 );
      while ( disJointFront && ( j < lastStrataCardinal ) )
        {
          IndiceCache::iterator it( candidates_.begin() );
          while ( disJointFront && ( it != candidates_.end()) )
            {
              disJointFront = ( ! isNeighbor( it->first, cache_[ lastStrataStart + j ] ));
              ++ it;
            }
          ++ j;
        }

      // if empty then we just finished a new strata
      if ( disJointFront )
        {
          strataCumulatedCardinal_.add( cache_.getSize() );
        }

    }
  return cache_[index];
}


/* The cardinal of the given strata */
UnsignedLong HyperbolicAnisotropicEnumerateFunction::inverse(const Indices & indices) const
{
  UnsignedLong inverse( 0 );
  while (( cache_[ inverse ] != indices ) && ( inverse < cache_.getSize() ) )
    {
      ++ inverse;
    }
  if ( inverse == cache_.getSize() )
    {
      do
        {
          operator()( inverse );
          ++ inverse;
        }
      while (cache_[ inverse ] != indices);
    }
  return inverse;
}


/* The cardinal of the given strata */
UnsignedLong HyperbolicAnisotropicEnumerateFunction::getStrataCardinal(const UnsignedLong strataIndex) const
{
  UnsignedLong result(getStrataCumulatedCardinal(strataIndex));
  if ( strataIndex > 0 )
    {
      result -= getStrataCumulatedCardinal( strataIndex - 1 );
    }
  return result;
}


/* The cardinal of the cumulated strata above or equal to the given strata */
UnsignedLong HyperbolicAnisotropicEnumerateFunction::getStrataCumulatedCardinal(const UnsignedLong strataIndex) const
{
  while ( strataCumulatedCardinal_.getSize() <= strataIndex )
    {
      operator()(cache_.getSize());
    }
  return strataCumulatedCardinal_[strataIndex];
}


/* Q accessor */
NumericalScalar HyperbolicAnisotropicEnumerateFunction::getQ() const
{
  return q_;
}


/* Weight accessor */
NumericalPoint HyperbolicAnisotropicEnumerateFunction::getWeight() const
{
  return weight_;
}


/* Method save() stores the object through the StorageManager */
void HyperbolicAnisotropicEnumerateFunction::save(Advocate & adv) const
{
  EnumerateFunctionImplementation::save(adv);
  adv.saveAttribute( "q_", q_ );
  adv.saveAttribute( "weight_", weight_ );
}


/* Method load() reloads the object from the StorageManager */
void HyperbolicAnisotropicEnumerateFunction::load(Advocate & adv)
{
  EnumerateFunctionImplementation::load(adv);
  adv.loadAttribute( "q_", q_ );
  adv.loadAttribute( "weight_", weight_ );
  initialize();
}


END_NAMESPACE_OPENTURNS
