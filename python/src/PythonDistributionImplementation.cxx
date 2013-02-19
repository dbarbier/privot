//                                               -*- C++ -*-
/**
 * @file  PythonDistributionImplementation.cxx
 * @brief This class binds a Python object to an Open TURNS' Distribution
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

#include "PythonDistributionImplementation.hxx"
#include "OSS.hxx"
#include "Description.hxx"
#include "PythonWrappingFunctions.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS


CLASSNAMEINIT(PythonDistributionImplementation);

static Factory<PythonDistributionImplementation> RegisteredFactory("PythonDistributionImplementation");



/* Default constructor */
PythonDistributionImplementation::PythonDistributionImplementation()
  : DistributionImplementation(),
    pyObj_(0)
{
  // Nothing to do
}


/* Constructor from Python object*/
PythonDistributionImplementation::PythonDistributionImplementation(PyObject * pyObject)
  : DistributionImplementation(),
    pyObj_(pyObject)
{
  Py_XINCREF( pyObj_ );

  if ( !PyObject_HasAttrString( pyObj_, const_cast<char *>("computeCDF") ) ) throw InvalidArgumentException(HERE) << "Error: the given object does not have a computeCDF() method.";

  // Set the name of the object as its Python classname
  PyObject * cls = PyObject_GetAttrString( pyObj_,
                                           const_cast<char *>( "__class__" ) );
  PyObject * name = PyObject_GetAttrString( cls,
                                            const_cast<char *>( "__name__" ) );

  setName( checkAndConvert<_PyString_, String>(name) );

  Py_XDECREF( cls );
  Py_XDECREF( name );
}

/* Virtual constructor */
PythonDistributionImplementation * PythonDistributionImplementation::clone() const
{
  return new PythonDistributionImplementation(*this);
}

/* Copy constructor */
PythonDistributionImplementation::PythonDistributionImplementation(const PythonDistributionImplementation & other)
  : DistributionImplementation(other),
    pyObj_(other.pyObj_)
{
  Py_XINCREF( pyObj_ );
}

/* Destructor */
PythonDistributionImplementation::~PythonDistributionImplementation()
{
  Py_XDECREF( pyObj_ );
}

/* Comparison operator */
Bool PythonDistributionImplementation::operator ==(const PythonDistributionImplementation & other) const
{
  return true;
}

/* String converter */
String PythonDistributionImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << PythonDistributionImplementation::GetClassName()
      << " name=" << getName()
      << " description=" << getDescription();
  return oss;
}

/* String converter */
String PythonDistributionImplementation::__str__(const String & offset) const
{
  OSS oss;
  oss << "class=" << PythonDistributionImplementation::GetClassName()
      << " name=" << getName();
  return oss;
}


/* Here is the interface that all derived class must implement */

NumericalPoint PythonDistributionImplementation::getRealization() const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("getRealization") ) )
    {


      PyObject * callResult = PyObject_CallMethod( pyObj_,
                                                   const_cast<char *>( "getRealization" ),
                                                   const_cast<char *>( "()" ) );
      if ( callResult == NULL )
        {
          handleException();
        }
      NumericalPoint result(convert< _PySequence_, NumericalPoint >( callResult ));
      if (result.getDimension() != getDimension()) throw InvalidDimensionException(HERE) << "Realization returned by PythonDistribution has incorrect dimension. Got " << result.getDimension() << ". Expected" << getDimension();
      Py_XDECREF( callResult );
      return result;
    }
  else
    {
      return DistributionImplementation::getRealization();
    }
}

/* Numerical sample accessor */
NumericalSample PythonDistributionImplementation::getSample(const UnsignedLong size) const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("getSample") ) )
    {
      PyObject * methodName = convert< String, _PyString_ >("getSample" );
      PyObject * sizeArg = convert< UnsignedLong, _PyInt_ >( size );
      PyObject * callResult = PyObject_CallMethodObjArgs( pyObj_,
                                                          methodName,
                                                          sizeArg, NULL );
      Py_XDECREF( methodName );
      Py_XDECREF( sizeArg );
      if ( callResult == NULL )
        {
          handleException();
        }
      NumericalSample result(convert<_PySequence_, NumericalSample>( callResult ));
      Py_XDECREF( callResult );
      if (result.getDimension() != getDimension()) throw InvalidDimensionException(HERE) << "Sample returned by PythonDistribution has incorrect dimension. Got " << result.getDimension() << ". Expected" << getDimension();
      if (result.getSize() != size) throw InvalidDimensionException(HERE) << "Sample returned by PythonDistribution has incorrect size. Got " << result.getSize() << ". Expected" << size;
      return result;
    }
  else
    {
      return DistributionImplementation::getSample(size);
    }
}


/* Get the DDF of the distribution */
NumericalPoint PythonDistributionImplementation::computeDDF(const NumericalPoint & inP) const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("computeDDF") ) )
    {
      const UnsignedLong dimension = inP.getDimension();
      if ( dimension != getDimension() )
        throw InvalidDimensionException(HERE) << "Input point has incorrect dimension. Got " << dimension << ". Expected " << getDimension();
      PyObject * methodName = convert< String, _PyString_>( "computeDDF" );
      PyObject * point = convert<NumericalPoint, _PySequence_>(inP);
      PyObject * callResult = PyObject_CallMethodObjArgs( pyObj_,
                                                          methodName,
                                                          point, NULL );
      Py_XDECREF( methodName );
      Py_XDECREF( point );
      if ( callResult == NULL )
        {
          handleException();
        }
      NumericalPoint result(convert< _PySequence_, NumericalPoint >( callResult ));
      Py_XDECREF( callResult );
      if (result.getDimension() != dimension) throw InvalidDimensionException(HERE) << "DDF returned by PythonDistribution has incorrect dimension. Got " << result.getDimension() << ". Expected" << dimension;
      return result;
    }
  else
    {
      return DistributionImplementation::computeDDF( inP );
    }
}

/* Get the PDF of the distribution */
NumericalScalar PythonDistributionImplementation::computePDF(const NumericalPoint & inP) const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("computePDF") ) )
    {
      const UnsignedLong dimension = inP.getDimension();
      if ( dimension != getDimension() )
        throw InvalidDimensionException(HERE) << "Input point has incorrect dimension. Got " << dimension << ". Expected " << getDimension();
      PyObject * methodName = convert< String, _PyString_>( "computePDF" );
      PyObject * point = convert< NumericalPoint, _PySequence_ >( inP );
      PyObject * callResult = PyObject_CallMethodObjArgs( pyObj_,
                                                          methodName,
                                                          point, NULL );
      Py_XDECREF( methodName );
      Py_XDECREF( point );
      if ( callResult == NULL )
        {
          handleException();
        }
      NumericalScalar result = convert< _PyFloat_, NumericalScalar >( callResult );
      Py_XDECREF( callResult );
      return result;
    }
  else
    {
      return DistributionImplementation::computePDF(inP);
    }
}

/* Get the CDF of the distribution */
NumericalScalar PythonDistributionImplementation::computeCDF(const NumericalPoint & inP) const
{
  const UnsignedLong dimension = inP.getDimension();
  if ( dimension != getDimension() )
    throw InvalidDimensionException(HERE) << "Input point has incorrect dimension. Got " << dimension << ". Expected " << getDimension();
  PyObject * methodName = convert< String, _PyString_>( "computeCDF" );
  PyObject * point = convert< NumericalPoint, _PySequence_ >( inP );
  PyObject * callResult = PyObject_CallMethodObjArgs( pyObj_,
                                                      methodName,
                                                      point, NULL );
  Py_XDECREF( methodName );
  Py_XDECREF( point );
  if ( callResult == NULL )
    {
      handleException();
    }
  NumericalScalar result = convert< _PyFloat_, NumericalScalar >( callResult );
  Py_XDECREF( callResult );
  return result;
}


/* Get the complementary CDF of the distribution */
NumericalScalar PythonDistributionImplementation::computeComplementaryCDF(const NumericalPoint & inP) const
{
  const UnsignedLong dimension = inP.getDimension();
  if ( dimension != getDimension() )
    throw InvalidDimensionException(HERE) << "Input point has incorrect dimension. Got " << dimension << ". Expected " << getDimension();
  PyObject * methodName = convert< String, _PyString_>( "computeComplementaryCDF" );
  PyObject * point = convert< NumericalPoint, _PySequence_ >( inP );
  PyObject * callResult = PyObject_CallMethodObjArgs( pyObj_,
                                                      methodName,
                                                      point, NULL );
  Py_XDECREF( methodName );
  Py_XDECREF( point );
  if ( callResult == NULL )
    {
      handleException();
    }
  NumericalScalar result = convert< _PyFloat_, NumericalScalar >( callResult );
  Py_XDECREF( callResult );
  return result;
}


/* Get the PDFGradient of the distribution */
NumericalPoint PythonDistributionImplementation::computePDFGradient(const NumericalPoint & inP) const
{
  const UnsignedLong dimension = inP.getDimension();
  if ( dimension != getDimension() )
    throw InvalidDimensionException(HERE) << "Input point has incorrect dimension. Got " << dimension << ". Expected " << getDimension();
  PyObject * methodName = convert< String, _PyString_>( "computePDFGradient" );
  PyObject * point = convert< NumericalPoint, _PySequence_ >( inP );
  PyObject * callResult = PyObject_CallMethodObjArgs( pyObj_,
                                                      methodName,
                                                      point, NULL );
  Py_XDECREF( methodName );
  Py_XDECREF( point );
  if ( callResult == NULL )
    {
      handleException();
    }
  NumericalPoint result(convert< _PySequence_, NumericalPoint >( callResult ));
  Py_XDECREF( callResult );
  if (result.getDimension() != dimension) throw InvalidDimensionException(HERE) << "PDFGradient returned by PythonDistribution has incorrect dimension. Got " << result.getDimension() << ". Expected" << dimension;
  return result;
}

/* Get the CDFGradient of the distribution */
NumericalPoint PythonDistributionImplementation::computeCDFGradient(const NumericalPoint & inP) const
{
  const UnsignedLong dimension = inP.getDimension();
  if ( dimension != getDimension() )
    throw InvalidDimensionException(HERE) << "Input point has incorrect dimension. Got " << dimension << ". Expected " << getDimension();
  PyObject * methodName = convert< String, _PyString_>( "computeCDFGradient" );
  PyObject * point = convert< NumericalPoint, _PySequence_ >( inP );
  PyObject * callResult = PyObject_CallMethodObjArgs( pyObj_,
                                                      methodName,
                                                      point, NULL );
  Py_XDECREF( methodName );
  Py_XDECREF( point );
  if ( callResult == NULL )
    {
      handleException();
    }
  NumericalPoint result(convert< _PySequence_, NumericalPoint >( callResult ));
  Py_XDECREF( callResult );
  if (result.getDimension() != dimension) throw InvalidDimensionException(HERE) << "CDFGradient returned by PythonDistribution has incorrect dimension. Got " << result.getDimension() << ". Expected" << dimension;
  return result;
}

/* Get the quantile of the distribution */
NumericalScalar PythonDistributionImplementation::computeScalarQuantile(const NumericalScalar prob,
                                                                        const Bool tail,
                                                                        const NumericalScalar precision) const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("computeScalarQuantile") ) )
    {
      PyObject * methodName = convert< String, _PyString_>( "computeScalarQuantile" );
      PyObject * cProb = convert< NumericalScalar, _PyFloat_ >( tail ? 1 - prob : prob );
      PyObject * callResult = PyObject_CallMethodObjArgs( pyObj_,
                                                          methodName,
                                                          cProb, NULL );
      Py_XDECREF( methodName );
      Py_XDECREF( cProb );
      if ( callResult == NULL )
        {
          handleException();
        }
      NumericalScalar result = convert< _PyFloat_, NumericalScalar >( callResult );
      Py_XDECREF( callResult );
      return result;
    }
  else
    {
      return DistributionImplementation::computeScalarQuantile( prob, tail, precision );
    }
}


/* Get the roughness, i.e. the L2-norm of the PDF */
NumericalScalar PythonDistributionImplementation::getRoughness() const
{
  PyObject * callResult = PyObject_CallMethod( pyObj_,
                                               const_cast<char *>( "getRoughness" ),
                                               const_cast<char *>( "()" ) );
  if ( callResult == NULL )
    {
      handleException();
    }
  NumericalScalar result = convert< _PyFloat_, NumericalScalar >( callResult );
  Py_XDECREF( callResult );
  return result;
}

/* Mean accessor */
NumericalPoint PythonDistributionImplementation::getMean() const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("getMean") ) )
    {
      PyObject * callResult = PyObject_CallMethod( pyObj_,
                                                   const_cast<char *>( "getMean" ),
                                                   const_cast<char *>( "()" ) );
      if ( callResult == NULL )
        {
          handleException();
        }
      NumericalPoint result(convert< _PySequence_, NumericalPoint >( callResult ));
      Py_XDECREF( callResult );
      if (result.getDimension() != getDimension()) throw InvalidDimensionException(HERE) << "Mean returned by PythonDistribution has incorrect dimension. Got " << result.getDimension() << ". Expected" << getDimension();
      return result;
    }
  else
    {
      return DistributionImplementation::getMean();
    }
}

/* Standard deviation accessor */
NumericalPoint PythonDistributionImplementation::getStandardDeviation() const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("getStandardDeviation") ) )
    {
      PyObject * callResult = PyObject_CallMethod( pyObj_,
                                                   const_cast<char *>( "getStandardDeviation" ),
                                                   const_cast<char *>( "()" ) );
      if ( callResult == NULL )
        {
          handleException();
        }
      NumericalPoint result(convert< _PySequence_, NumericalPoint >( callResult ));
      Py_XDECREF( callResult );
      if (result.getDimension() != getDimension()) throw InvalidDimensionException(HERE) << "Standard deviation returned by PythonDistribution has incorrect dimension. Got " << result.getDimension() << ". Expected" << getDimension();
      return result;
    }
  else
    {
      return DistributionImplementation::getStandardDeviation();
    }
}

/* Skewness accessor */
NumericalPoint PythonDistributionImplementation::getSkewness() const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("getSkewness") ) )
    {
      PyObject * callResult = PyObject_CallMethod( pyObj_,
                                                   const_cast<char *>( "getSkewness" ),
                                                   const_cast<char *>( "()" ) );
      if ( callResult == NULL )
        {
          handleException();
        }
      NumericalPoint result(convert< _PySequence_, NumericalPoint >( callResult ));
      Py_XDECREF( callResult );
      if (result.getDimension() != getDimension()) throw InvalidDimensionException(HERE) << "Skewness returned by PythonDistribution has incorrect dimension. Got " << result.getDimension() << ". Expected" << getDimension();
      return result;
    }
  else
    {
      return DistributionImplementation::getSkewness();
    }
}

/* Kurtosis accessor */
NumericalPoint PythonDistributionImplementation::getKurtosis() const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("getKurtosis") ) )
    {
      PyObject * callResult = PyObject_CallMethod( pyObj_,
                                                   const_cast<char *>( "getKurtosis" ),
                                                   const_cast<char *>( "()" ) );
      if ( callResult == NULL )
        {
          handleException();
        }
      NumericalPoint result(convert< _PySequence_, NumericalPoint >( callResult ));
      Py_XDECREF( callResult );
      if (result.getDimension() != getDimension()) throw InvalidDimensionException(HERE) << "Kurtosis returned by PythonDistribution has incorrect dimension. Got " << result.getDimension() << ". Expected" << getDimension();
      return result;
    }
  else
    {
      return DistributionImplementation::getKurtosis();
    }
}

/* Get the raw moments of the distribution */
NumericalPoint PythonDistributionImplementation::getMoment(const UnsignedLong n) const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("getMoment") ) )
    {
      PyObject * methodName = convert< String, _PyString_>( "getMoment" );
      PyObject * nArg = convert< UnsignedLong, _PyInt_ >( n );
      PyObject * callResult = PyObject_CallMethodObjArgs( pyObj_,
                                                          methodName,
                                                          nArg, NULL );
      Py_XDECREF( methodName );
      Py_XDECREF( nArg );
      if ( callResult == NULL )
        {
          handleException();
        }
      NumericalPoint result(convert< _PySequence_, NumericalPoint >( callResult ));
      Py_XDECREF( callResult );
      if (result.getDimension() != getDimension()) throw InvalidDimensionException(HERE) << "Moment returned by PythonDistribution has incorrect dimension. Got " << result.getDimension() << ". Expected" << getDimension();
      return result;
    }
  else
    {
      return DistributionImplementation::getMoment( n );
    }
}

/* Get the centered moments of the distribution */
NumericalPoint PythonDistributionImplementation::getCenteredMoment(const UnsignedLong n) const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("getCenteredMoment") ) )
    {
      PyObject * methodName = convert< String, _PyString_>( "getCenteredMoment" );
      PyObject * nArg = convert< UnsignedLong, _PyInt_ >( n );
      PyObject * callResult = PyObject_CallMethodObjArgs( pyObj_,
                                                          methodName,
                                                          nArg, NULL );
      Py_XDECREF( methodName );
      Py_XDECREF( nArg );
      if ( callResult == NULL )
        {
          handleException();
        }
      NumericalPoint result(convert< _PySequence_, NumericalPoint >( callResult ));
      Py_XDECREF( callResult );
      if (result.getDimension() != getDimension()) throw InvalidDimensionException(HERE) << "Centered moment returned by PythonDistribution has incorrect dimension. Got " << result.getDimension() << ". Expected" << getDimension();
      return result;
    }
  else
    {
      return DistributionImplementation::getCenteredMoment( n );
    }
}


/* Check if the distribution is elliptical */
Bool PythonDistributionImplementation::isElliptical() const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("isElliptical") ) )
    {
      PyObject * callResult = PyObject_CallMethod( pyObj_,
                                                   const_cast<char *>( "isElliptical" ),
                                                   const_cast<char *>( "()" ) );
      if ( callResult == NULL )
        {
          handleException();
        }
      Bool result = convert< _PyBool_, Bool >( callResult );
      Py_XDECREF( callResult );
      return result;
    }
  else
    {
      return DistributionImplementation::isElliptical();
    }
}


/* Check if the distribution is continuous */
Bool PythonDistributionImplementation::isContinuous() const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("isContinuous") ) )
    {
      PyObject * callResult = PyObject_CallMethod( pyObj_,
                                                   const_cast<char *>( "isContinuous" ),
                                                   const_cast<char *>( "()" ) );
      if ( callResult == NULL )
        {
          handleException();
        }
      Bool result = convert< _PyBool_, Bool >( callResult );
      Py_XDECREF( callResult );
      return result;
    }
  else
    {
      return DistributionImplementation::isContinuous();
    }
}


/* Check if the distribution is integral */
Bool PythonDistributionImplementation::isIntegral() const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("isIntegral") ) )
    {
      PyObject * callResult = PyObject_CallMethod( pyObj_,
                                                   const_cast<char *>( "isIntegral" ),
                                                   const_cast<char *>( "()" ) );
      if ( callResult == NULL )
        {
          handleException();
        }
      Bool result = convert< _PyBool_, Bool >( callResult );
      Py_XDECREF( callResult );
      return result;
    }
  else
    {
      return DistributionImplementation::isIntegral();
    }
}


/* Tell if the distribution has elliptical copula */
Bool PythonDistributionImplementation::hasEllipticalCopula() const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("hasEllipticalCopula") ) )
    {
      PyObject * callResult = PyObject_CallMethod( pyObj_,
                                                   const_cast<char *>( "hasEllipticalCopula" ),
                                                   const_cast<char *>( "()" ) );
      if ( callResult == NULL )
        {
          handleException();
        }
      Bool result = convert< _PyBool_, Bool >( callResult );
      Py_XDECREF( callResult );
      return result;
    }
  else
    {
      return DistributionImplementation::hasEllipticalCopula();
    }
}


/* Tell if the distribution has independent copula */
Bool PythonDistributionImplementation::hasIndependentCopula() const
{
  if ( PyObject_HasAttrString( pyObj_, const_cast<char *>("hasIndependentCopula") ) )
    {
      PyObject * callResult = PyObject_CallMethod( pyObj_,
                                                   const_cast<char *>( "hasIndependentCopula" ),
                                                   const_cast<char *>( "()" ) );
      if ( callResult == NULL )
        {
          handleException();
        }
      Bool result = convert< _PyBool_, Bool >( callResult );
      Py_XDECREF( callResult );
      return result;
    }
  else
    {
      return DistributionImplementation::hasIndependentCopula();
    }
}


/* Method save() stores the object through the StorageManager */
void PythonDistributionImplementation::save(Advocate & adv) const
{
  DistributionImplementation::save( adv );

  pickleSave(adv, pyObj_);
}


/* Method save() reloads the object from the StorageManager */
void PythonDistributionImplementation::load(Advocate & adv)
{
  DistributionImplementation::load( adv );

  pickleLoad(adv, pyObj_);
}


END_NAMESPACE_OPENTURNS
