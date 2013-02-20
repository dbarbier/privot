//                                               -*- C++ -*-
/**
 * @file  PythonNumericalMathEvaluationImplementation.cxx
 * @brief This class binds a Python function to an Open TURNS' NumericalMathFunction
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
 * @author schueller
 * @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)
 */

#include "PythonNumericalMathEvaluationImplementation.hxx"
#include "OSS.hxx"
#include "Description.hxx"
#include "PythonWrappingFunctions.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

typedef NumericalMathEvaluationImplementation::CacheKeyType             CacheKeyType;
typedef NumericalMathEvaluationImplementation::CacheValueType           CacheValueType;
typedef NumericalMathEvaluationImplementation::CacheType                CacheType;


CLASSNAMEINIT(PythonNumericalMathEvaluationImplementation);

static Factory<PythonNumericalMathEvaluationImplementation> RegisteredFactory("PythonNumericalMathEvaluationImplementation");



/* Default constructor */
PythonNumericalMathEvaluationImplementation::PythonNumericalMathEvaluationImplementation()
  : NumericalMathEvaluationImplementation()
  , pyObj_(0)
{
  // Nothing to do
}


/* Constructor from Python object*/
PythonNumericalMathEvaluationImplementation::PythonNumericalMathEvaluationImplementation(PyObject * pyCallable)
  : NumericalMathEvaluationImplementation()
  , pyObj_(pyCallable)
{
  Py_XINCREF( pyCallable );

  // Set the name of the object as its Python classname
  PyObject * cls = PyObject_GetAttrString( pyObj_,
                                           const_cast<char *>( "__class__" ) );
  PyObject * name = PyObject_GetAttrString( cls,
                                            const_cast<char *>( "__name__" ) );
  setName( convert< _PyString_, String >( name ) );
  Py_XDECREF( name );
  Py_XDECREF( cls );


  const UnsignedLong inputDimension  = getInputDimension();
  const UnsignedLong outputDimension = getOutputDimension();
  Description description(inputDimension + outputDimension);

  PyObject * descIn = PyObject_CallMethod( pyObj_,
                                           const_cast<char *>( "getInputDescription" ),
                                           const_cast<char *>( "()" ) );
  if ( ( descIn != NULL )
       && PySequence_Check( descIn )
       && ( PySequence_Size( descIn ) == (SignedInteger)inputDimension ) )
    {
      Description inputDescription(convert< _PySequence_, Description >( descIn ));
      for (UnsignedLong i = 0; i < inputDimension; ++i)
        {
          description[i] = inputDescription[i];
        }
      Py_XDECREF( descIn );
    }
  else for (UnsignedLong i = 0; i < inputDimension; ++i) description[i] = (OSS() << "x" << i);


  PyObject * descOut = PyObject_CallMethod( pyObj_,
                                            const_cast<char *>( "getOutputDescription" ),
                                            const_cast<char *>( "()" ) );
  if ( ( descOut != NULL )
       && PySequence_Check( descOut )
       && ( PySequence_Size( descOut ) == (SignedInteger)outputDimension ) )
    {
      Description outputDescription(convert< _PySequence_, Description >( descOut ));
      for (UnsignedLong i = 0; i < outputDimension; ++i)
        {
          description[inputDimension + i] = outputDescription[i];
        }
      Py_XDECREF( descOut );
    }
  else for (UnsignedLong i = 0; i < outputDimension; ++i) description[inputDimension + i] = (OSS() << "y" << i);

  setDescription(description);
}

/* Virtual constructor */
PythonNumericalMathEvaluationImplementation * PythonNumericalMathEvaluationImplementation::clone() const
{
  return new PythonNumericalMathEvaluationImplementation(*this);
}

/* Copy constructor */
PythonNumericalMathEvaluationImplementation::PythonNumericalMathEvaluationImplementation(const PythonNumericalMathEvaluationImplementation & other)
  : NumericalMathEvaluationImplementation(other)
  , pyObj_(other.pyObj_)
{
  Py_XINCREF( pyObj_ );
}

/* Destructor */
PythonNumericalMathEvaluationImplementation::~PythonNumericalMathEvaluationImplementation()
{
  Py_XDECREF( pyObj_ );
}

/* Comparison operator */
Bool PythonNumericalMathEvaluationImplementation::operator ==(const PythonNumericalMathEvaluationImplementation & other) const
{
  return true;
}

/* String converter */
String PythonNumericalMathEvaluationImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << PythonNumericalMathEvaluationImplementation::GetClassName()
      << " name=" << getName()
      << " description=" << getDescription()
      << " parameters=" << getParameters();
  return oss;
}

/* String converter */
String PythonNumericalMathEvaluationImplementation::__str__(const String & offset) const
{
  OSS oss;
  oss << "class=" << PythonNumericalMathEvaluationImplementation::GetClassName()
      << " name=" << getName();
  return oss;
}

/* Test for actual implementation */
Bool PythonNumericalMathEvaluationImplementation::isActualImplementation() const
{
  return true;
}



/* Here is the interface that all derived class must implement */

/* Operator () */
NumericalPoint PythonNumericalMathEvaluationImplementation::operator() (const NumericalPoint & inP) const
/*        throw(InvalidArgumentException,InternalException)*/
{
  const UnsignedLong dimension( inP.getDimension() );

  if ( dimension != getInputDimension() )
    throw InvalidDimensionException(HERE) << "Input point has incorrect dimension. Got " << dimension << ". Expected " << getInputDimension();

  NumericalPoint outP;
  CacheKeyType inKey( inP.getCollection() );
  if ( p_cache_->isEnabled() && p_cache_->hasKey( inKey ) )
    {
      outP = NumericalPoint::ImplementationType( p_cache_->find( inKey ) );
    }
  else
    {
      ++ callsNumber_;

      PyObject * point = convert< NumericalPoint, _PySequence_ >(inP);
      PyObject * result = PyObject_CallFunctionObjArgs( pyObj_, point, NULL );

      if ( result == NULL )
        {
          handleException();
        }

      try
        {
          outP = convert< _PySequence_, NumericalPoint >( result );
        }
      catch (const InvalidArgumentException & ex)
        {
          throw InvalidArgumentException(HERE) << "Output value for " << getName() << "._exec() method is not a sequence object (list, tuple, NumericalPoint, etc.)";
        }

      if (outP.getDimension() != getOutputDimension())
        {
          throw InvalidDimensionException(HERE) << "Output point has incorrect dimension. Got " << outP.getDimension() << ". Expected " << getOutputDimension();
        }

      if ( p_cache_->isEnabled() )
        {
          CacheValueType outValue( outP.getCollection() );
          p_cache_->add( inKey, outValue );
        }

      Py_XDECREF( point  );
      Py_XDECREF( result );

    }
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inP);
      outputStrategy_.store(outP);
    }
  return outP;
}


/* Operator () */
NumericalSample PythonNumericalMathEvaluationImplementation::operator() (const NumericalSample & inS) const
/*        throw(InvalidArgumentException,InternalException)*/
{
  const UnsignedLong size( inS.getSize() );
  const UnsignedLong inDim( inS.getDimension() );
  const UnsignedLong outDim( getOutputDimension() );

  if ( inDim != getInputDimension() )
    throw InvalidDimensionException(HERE) << "Input point has incorrect dimension. Got " << inDim << ". Expected " << getInputDimension();

  Indices toDo;
  NumericalSample outS( size, outDim );
  if ( p_cache_->isEnabled() )
    {
      for (UnsignedLong i = 0; i < size; ++ i )
        {
          CacheKeyType inKey( inS[i].getCollection() );
          if ( p_cache_->hasKey( inKey ) )
            {
              outS[i] = NumericalPoint::ImplementationType( p_cache_->find( inKey ) );
            }
          else
            {
              toDo.add( i );
            }
        }
    }
  else
    {
      toDo = Indices( size );
      toDo.fill();
    }
  UnsignedLong toDoSize( toDo.getSize() );

  if ( toDoSize > 0 )
    {
      callsNumber_ += toDoSize;

      PyObject * inTuple = PyTuple_New( toDoSize );

      for ( UnsignedLong i = 0; i < toDoSize; ++ i )
        {
          PyObject * eltTuple = PyTuple_New( inDim );
          for ( UnsignedLong j = 0; j < inDim; ++ j ) PyTuple_SetItem( eltTuple, j, convert< NumericalScalar, _PyFloat_ > ( inS[toDo[i]][j] ) );
          PyTuple_SetItem( inTuple, i, eltTuple );
        }

      PyObject * result = PyObject_CallFunctionObjArgs( pyObj_, inTuple, NULL );

      if ( result == NULL )
        {
          handleException();
        }

      if ( PySequence_Check( result ) )
        {
          const UnsignedLong lengthResult = PySequence_Size( result );
          if ( lengthResult == toDoSize )
            {
              for (UnsignedLong i = 0; i < toDoSize; ++i)
                {
                  PyObject * elt = PySequence_GetItem( result, i );
                  if ( PySequence_Check( elt ) )
                    {
                      const UnsignedLong lengthElt = PySequence_Size( elt );
                      if ( lengthElt == outDim )
                        {
                          for (UnsignedLong j = 0; j < outDim; ++j)
                            {
                              PyObject * val = PySequence_GetItem( elt, j );
                              outS[toDo[i]][j] = convert< _PyFloat_, NumericalScalar >( val );
                              Py_XDECREF( val );
                            }
                        }
                      else
                        {
                          throw InvalidArgumentException(HERE) << "Python NumericalMathFunction returned an sequence object with incorrect dimension (at position "
                                                               << i << ")";
                        }
                    }
                  else
                    {
                      throw InvalidArgumentException(HERE) << "Python NumericalMathFunction returned an object which is NOT a sequence (at position "
                                                           << i << ")";
                    }
                  Py_XDECREF( elt );
                }
            }
          else
            {
              throw InvalidArgumentException(HERE) << "Python NumericalMathFunction returned an sequence object with incorrect size (got "
                                                   << lengthResult << ", expected " << toDoSize << ")";
            }
        }

      Py_XDECREF( inTuple );
      Py_XDECREF( result  );
    }

  if (outS.getDimension() != getOutputDimension())
    {
      throw InvalidDimensionException(HERE) << "Output sample has incorrect dimension. Got " << outS.getDimension() << ". Expected " << getOutputDimension();
    }

  if ( p_cache_->isEnabled() )
    {
      for (UnsignedLong i = 0; i < toDoSize; ++i)
        {
          CacheKeyType inKey( inS[toDo[i]].getCollection() );
          CacheValueType outValue( outS[toDo[i]].getCollection() );
          p_cache_->add( inKey, outValue );
        }
    }
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inS);
      outputStrategy_.store(outS);
    }
  outS.setDescription(getOutputDescription());
  return outS;
}


/* Accessor for input point dimension */
UnsignedLong PythonNumericalMathEvaluationImplementation::getInputDimension() const
/*        throw(InternalException)*/
{
  PyObject * result = PyObject_CallMethod( pyObj_,
                                           const_cast<char *>( "getInputDimension" ),
                                           const_cast<char *>( "()" ) );
  UnsignedLong dim = convert< _PyInt_, UnsignedLong >( result );
  Py_XDECREF( result );
  return dim;
}


/* Accessor for output point dimension */
UnsignedLong PythonNumericalMathEvaluationImplementation::getOutputDimension() const
/*        throw(InternalException)*/
{
  PyObject * result = PyObject_CallMethod( pyObj_,
                                           const_cast<char *>( "getOutputDimension" ),
                                           const_cast<char *>( "()" ) );
  UnsignedLong dim = convert< _PyInt_, UnsignedLong >( result );
  Py_XDECREF( result );
  return dim;
}


/* Method save() stores the object through the StorageManager */
void PythonNumericalMathEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save( adv );

  pickleSave(adv, pyObj_);
}


/* Method save() reloads the object from the StorageManager */
void PythonNumericalMathEvaluationImplementation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load( adv );

  pickleLoad(adv, pyObj_);
}


END_NAMESPACE_OPENTURNS
