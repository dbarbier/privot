//                                               -*- C++ -*-
/**
 * @file  PythonRandomVectorImplementation.cxx
 * @brief This class binds a Python object to an Open TURNS' RandomVector
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
 * @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */

#include "PythonRandomVectorImplementation.hxx"
#include "OSS.hxx"
#include "Description.hxx"
#include "PythonWrappingFunctions.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS


CLASSNAMEINIT(PythonRandomVectorImplementation);

static Factory<PythonRandomVectorImplementation> RegisteredFactory("PythonRandomVectorImplementation");



/* Default constructor */
PythonRandomVectorImplementation::PythonRandomVectorImplementation()
  : RandomVectorImplementation(),
    pyObj_(0)
{
  // Nothing to do
}


/* Constructor from Python object*/
PythonRandomVectorImplementation::PythonRandomVectorImplementation(PyObject * pyObject)
  : RandomVectorImplementation(),
    pyObj_(pyObject)
{
  Py_XINCREF( pyObj_ );

  // Set the name of the object as its Python classname
  PyObject * cls = PyObject_GetAttrString( pyObj_,
                                           const_cast<char *>( "__class__" ) );
  PyObject * name = PyObject_GetAttrString( cls,
                                            const_cast<char *>( "__name__" ) );

  setName( checkAndConvert< _PyString_, String >(name) );

  Py_XDECREF( cls );
  Py_XDECREF( name );

  const UnsignedLong dimension  = getDimension();
  Description description(dimension);
  PyObject * desc = PyObject_CallMethod( pyObj_,
                                         const_cast<char *>( "getDescription" ),
                                         const_cast<char *>( "()" ) );
  if ( ( desc != NULL )
       && PySequence_Check( desc )
       && ( PySequence_Size( desc ) == dimension ) )
    {
      description = convert< _PySequence_, Description >( desc );
      Py_XDECREF( desc );
    }
  else for (UnsignedLong i = 0; i < dimension; ++i) description[i] = (OSS() << "x" << i);
  setDescription(description);
}

/* Virtual constructor */
PythonRandomVectorImplementation * PythonRandomVectorImplementation::clone() const
{
  return new PythonRandomVectorImplementation(*this);
}

/* Copy constructor */
PythonRandomVectorImplementation::PythonRandomVectorImplementation(const PythonRandomVectorImplementation & other)
  : RandomVectorImplementation(other),
    pyObj_(other.pyObj_)
{
  Py_XINCREF( pyObj_ );
}

/* Destructor */
PythonRandomVectorImplementation::~PythonRandomVectorImplementation()
{
  Py_XDECREF( pyObj_ );
}

/* Comparison operator */
Bool PythonRandomVectorImplementation::operator ==(const PythonRandomVectorImplementation & other) const
{
  return true;
}

/* String converter */
String PythonRandomVectorImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << PythonRandomVectorImplementation::GetClassName()
      << " name=" << getName()
      << " description=" << getDescription();
  return oss;
}

/* String converter */
String PythonRandomVectorImplementation::__str__(const String & offset) const
{
  OSS oss;
  oss << "class=" << PythonRandomVectorImplementation::GetClassName()
      << " name=" << getName();
  return oss;
}


/* Here is the interface that all derived class must implement */


/* Accessor for input point dimension */
UnsignedLong PythonRandomVectorImplementation::getDimension() const
{
  PyObject * result = PyObject_CallMethod( pyObj_,
                                           const_cast<char *>( "getDimension" ),
                                           const_cast<char *>( "()" ) );
  if ( result == NULL )
    {
      handleException();
    }

  UnsignedLong dim = convert< _PyInt_, UnsignedLong >( result );
  return dim;
}

NumericalPoint PythonRandomVectorImplementation::getRealization() const
{
  PyObject * result = PyObject_CallMethod( pyObj_,
                                           const_cast<char *>( "getRealization" ),
                                           const_cast<char *>( "()" ) );
  if ( result == NULL )
    {
      handleException();
    }
  NumericalPoint point(convert<_PySequence_, NumericalPoint>(result));
  Py_XDECREF( result );
  return point;
}


/* Numerical sample accessor */
NumericalSample PythonRandomVectorImplementation::getSample(const UnsignedLong size) const
{
  NumericalSample sample;

  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("getSample") ) )
    {
      PyObject * methodName = convert< String, _PyString_>( "getSample" );
      PyObject * sizeArg = convert< UnsignedLong, _PyInt_ >(size);
      PyObject * result = PyObject_CallMethodObjArgs( pyObj_,
                                                      methodName,
                                                      sizeArg, NULL );
      Py_XDECREF( methodName );
      Py_XDECREF( sizeArg );
      if ( result != NULL )
        {
          sample = convert<_PySequence_, NumericalSample>(result);
          Py_XDECREF( result );
          if (sample.getSize() != size) throw InvalidDimensionException(HERE) << "Sample returned by PythonRandomVector has incorrect size. Got " << sample.getSize() << ". Expected" << size;
        }
    }
  else
    {
      sample = RandomVectorImplementation::getSample(size);
    }
  return sample;
}


/* Mean accessor */
NumericalPoint PythonRandomVectorImplementation::getMean() const
{
  PyObject * result = PyObject_CallMethod( pyObj_,
                                           const_cast<char *>( "getMean" ),
                                           const_cast<char *>( "()" ) );
  if ( result == NULL )
    {
      handleException();
    }

  NumericalPoint mean(convert<_PySequence_, NumericalPoint>(result));
  Py_XDECREF( result );
  if (mean.getDimension() != getDimension()) throw InvalidDimensionException(HERE) << "Mean returned by PythonRandomVector has incorrect dimension. Got " << mean.getDimension() << ". Expected" << getDimension();
  return mean;
}

/* Covariance accessor */
CovarianceMatrix PythonRandomVectorImplementation::getCovariance() const
{
  PyObject * result = PyObject_CallMethod( pyObj_,
                                           const_cast<char *>( "getCovariance" ),
                                           const_cast<char *>( "()" ) );
  if ( result == NULL )
    {
      handleException();
    }

  CovarianceMatrix covariance(convert<_PySequence_, CovarianceMatrix>(result));
  Py_XDECREF( result );
  if (covariance.getDimension() != getDimension()) throw InvalidDimensionException(HERE) << "Covariance matrix returned by PythonRandomVector has incorrect dimension. Got " << covariance.getDimension() << ". Expected" << getDimension();

  return covariance;
}

/* Method save() stores the object through the StorageManager */
void PythonRandomVectorImplementation::save(Advocate & adv) const
{
  RandomVectorImplementation::save( adv );

  pickleSave(adv, pyObj_);
}


/* Method save() reloads the object from the StorageManager */
void PythonRandomVectorImplementation::load(Advocate & adv)
{
  RandomVectorImplementation::load( adv );

  pickleLoad(adv, pyObj_);
}


END_NAMESPACE_OPENTURNS
