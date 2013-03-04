//                                               -*- C++ -*-
/**
 * @file  PythonWrappingFunctions.hxx
 * @brief This file provides functions to ease Python wrapping
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
 * @date   2009-09-14 14:39:35 +0200 (Mon, 14 Sep 2009)
 */

#ifndef OPENTURNS_PYTHONWRAPPINGFUNCTIONS_HXX
#define OPENTURNS_PYTHONWRAPPINGFUNCTIONS_HXX

#include "Python.h"
#include "OT.hxx"

BEGIN_NAMESPACE_OPENTURNS


/** These templates are just declared, not defined. Only specializations are. */
template <class CPP_Type>                    struct traitsPythonType;
template <class PYTHON_Type>                 static inline int          isAPython(PyObject * pyObj);
template <class PYTHON_Type>                 static inline const char * namePython();
template <class PYTHON_Type, class CPP_Type> static inline CPP_Type     convert(PyObject * pyObj);
template <class CPP_Type, class PYTHON_Type> static inline PyObject *   convert(CPP_Type);
template <class PYTHON_Type, class CPP_Type> static inline bool         canConvert(PyObject * pyObj);
template <class PYTHON_Type>                 static inline void         check(PyObject * pyObj);
template <class PYTHON_Type, class CPP_Type> static inline CPP_Type     checkAndConvert(PyObject * pyObj);
template <class T>                           static inline T *          buildObjectFromPySequence(PyObject * pyObj);



/** Specializations */


/* PyObject */
struct _PyObject_ {};

template <>
inline
int
isAPython<_PyObject_>(PyObject * pyObj)
{
  return 1;
}

template <>
inline
const char *
namePython<_PyObject_>()
{
  return "object";
}




/* PyBool */
struct _PyBool_ {};

template <>
inline
int
isAPython<_PyBool_>(PyObject * pyObj)
{
  return PyBool_Check( pyObj );
}

template <>
inline
const char *
namePython<_PyBool_>()
{
  return "bool";
}

template <>
struct traitsPythonType<Bool>
{
  typedef _PyBool_ Type;
};

template <>
inline
Bool
convert< _PyBool_, Bool >(PyObject * pyObj)
{
  return pyObj == Py_True;
}





/* PyInt */
struct _PyInt_ {};

template <>
inline
int
isAPython< _PyInt_ >(PyObject * pyObj)
{
  // PyInt type is deprecated
#if PY_MAJOR_VERSION >= 3
  return PyLong_Check( pyObj );
#else
  return PyInt_Check( pyObj ) || PyLong_Check( pyObj );
#endif
}

template <>
inline
const char *
namePython< _PyInt_ >()
{
  return "integer";
}

template <>
struct traitsPythonType< UnsignedLong >
{
  typedef _PyInt_ Type;
};

template <>
inline
UnsignedLong
convert< _PyInt_, UnsignedLong >(PyObject * pyObj)
{
  return PyLong_AsUnsignedLong( pyObj );
}

template <>
inline
PyObject *
convert< UnsignedLong, _PyInt_ >(UnsignedLong n)
{
  return PyLong_FromUnsignedLong( n );
}




/* PyFloat */
struct _PyFloat_ {};

template <>
inline
int
isAPython< _PyFloat_ >(PyObject * pyObj)
{
  // check also against sequence protocol as numpy array complies to several protocols
  return PyNumber_Check( pyObj ) && !PyComplex_Check(pyObj) && !PySequence_Check( pyObj );
}

template <>
inline
const char *
namePython<_PyFloat_>()
{
  return "double";
}

template <>
struct traitsPythonType< NumericalScalar >
{
  typedef _PyFloat_ Type;
};

template <>
inline
NumericalScalar
convert< _PyFloat_, NumericalScalar >(PyObject * pyObj)
{
  return PyFloat_AsDouble( pyObj );
}

template <>
inline
PyObject *
convert< NumericalScalar, _PyFloat_ >(NumericalScalar x)
{
  return PyFloat_FromDouble( x );
}


/* PyComplex */
struct _PyComplex_ {};

template <>
inline
int
isAPython<_PyComplex_>(PyObject * pyObj)
{
  return PyNumber_Check( pyObj );
}

template <>
inline
const char *
namePython<_PyComplex_>()
{
  return "complex";
}

template <>
struct traitsPythonType< NumericalComplex >
{
  typedef _PyComplex_ Type;
};

template <>
inline
NumericalComplex
convert< _PyComplex_, NumericalComplex >(PyObject * pyObj)
{
  return NumericalComplex( PyComplex_RealAsDouble( pyObj ), PyComplex_ImagAsDouble( pyObj ) );
}

template <>
inline
PyObject *
convert< NumericalComplex, _PyComplex_ >(NumericalComplex x)
{
  return PyComplex_FromDoubles( x.real(), x.imag() );
}


/* PyBytes */
struct _PyBytes_ {};

template <>
inline
int
isAPython< _PyBytes_ >(PyObject * pyObj)
{
#if PY_MAJOR_VERSION >= 3
  return PyBytes_Check( pyObj );
#else
  return PyString_Check( pyObj );
#endif
}

template <>
inline
const char *
namePython< _PyBytes_ >()
{
  return "bytes";
}

template <>
inline
String
convert< _PyBytes_, String >(PyObject * pyObj)
{
#if PY_MAJOR_VERSION >= 3
  return PyBytes_AsString( pyObj );
#else
  return PyString_AsString( pyObj );
#endif
}

template <>
inline
PyObject *
convert< String, _PyBytes_ >( String s )
{
#if PY_MAJOR_VERSION >= 3
  return PyBytes_FromString( s.data() );
#else
  return PyString_FromString( s.data() );
#endif
}


/* PyString */
struct _PyString_ {};

template <>
inline
int
isAPython< _PyString_ >(PyObject * pyObj)
{
#if PY_MAJOR_VERSION >= 3
  return PyUnicode_Check( pyObj );
#else
  return PyString_Check( pyObj );
#endif
}

template <>
inline
const char *
namePython<_PyString_>()
{
  return "string";
}

template <>
struct traitsPythonType< String >
{
  typedef _PyString_ Type;
};

template <>
inline
String
convert< _PyString_, String >(PyObject * pyObj)
{
#if PY_MAJOR_VERSION >= 3
  PyObject * utf8Bytes = PyUnicode_AsUTF8String(pyObj);
  assert( utf8Bytes );
  String result(convert< _PyBytes_, String >(utf8Bytes));
  Py_XDECREF( utf8Bytes );
  return result;
#else
  return PyString_AsString( pyObj );
#endif
}

template <>
inline
PyObject *
convert< String, _PyString_ >( String s )
{
#if PY_MAJOR_VERSION >= 3
  return PyUnicode_FromString( s.data() );
#else
  return PyString_FromString( s.data() );
#endif
}


/* PySequence */
struct _PySequence_ {};

template <>
inline
int
isAPython< _PySequence_ >(PyObject * pyObj)
{
  return PySequence_Check( pyObj );
}

template <>
inline
const char *
namePython<_PySequence_>()
{
  return "sequence object";
}




template <class PYTHON_Type>
static inline
int
isAPythonSequenceOf(PyObject * pyObj)
{
  int ok = isAPython<_PySequence_>( pyObj ) && ( ! isAPython< _PyString_ >( pyObj ) );

  if ( ok )
    {
      const UnsignedLong size = PySequence_Size( pyObj );
      for( UnsignedLong i = 0; ok && (i < size); ++i )
        {
          PyObject * elt = PySequence_ITEM( pyObj, i );
          int elt_ok = isAPython<PYTHON_Type>( elt );
          ok *= elt_ok;
          Py_XDECREF( elt );
        }
    }

  return ok;
}


template <class PYTHON_Type>
static inline
void
check(PyObject * pyObj)
{
  if (! isAPython<PYTHON_Type>( pyObj ))
    {
      throw InvalidArgumentException(HERE) << "Object passed as argument is not a " << namePython<PYTHON_Type>();
    }
}


template <class PYTHON_Type, class CPP_Type>
static inline
CPP_Type
checkAndConvert(PyObject * pyObj)
{
  check<PYTHON_Type>( pyObj );
  return convert< PYTHON_Type, CPP_Type >( pyObj );
}





template <class T>
static inline
bool
canConvertCollectionObjectFromPySequence(PyObject * pyObj)
{
  try
    {
      check<_PySequence_>( pyObj );
    }
  catch( InvalidArgumentException & ex )
    {
      return false;
    }

  PyObject * newPyObj = PySequence_Fast( pyObj, "" );

  const UnsignedLong size = PySequence_Fast_GET_SIZE( newPyObj );
  for(UnsignedLong i = 0; i < size; ++i)
    {
      PyObject * elt = PySequence_Fast_GET_ITEM( newPyObj, i );
      if (!canConvert< typename traitsPythonType< T >::Type, T >( elt ))
        {
          Py_XDECREF( newPyObj );
          return false;
        }
    }

  Py_XDECREF( newPyObj );
  return true;
}




template <class T>
static inline
Collection<T> *
buildCollectionFromPySequence(PyObject * pyObj, int sz = 0)
{
  check<_PySequence_>( pyObj );
  PyObject * newPyObj = PySequence_Fast( pyObj, "" );

  const UnsignedLong size = PySequence_Fast_GET_SIZE( newPyObj );
  if ((sz != 0) && (sz != (int)size))
    {
      Py_XDECREF( newPyObj );
      throw InvalidArgumentException(HERE) << "Sequence object has incorrect size " << size << ". Must be " << sz << ".";
    }
  Collection<T> * p_coll = new Collection< T >( size );

  for(UnsignedLong i = 0; i < size; ++i)
    {
      PyObject * elt = PySequence_Fast_GET_ITEM( newPyObj, i );
      try
        {
          check<typename traitsPythonType< T >::Type>( elt );
        }
      catch( InvalidArgumentException & ex )
        {
          Py_XDECREF( newPyObj );
          delete p_coll;
          throw;
        }
      (*p_coll)[i] = convert< typename traitsPythonType< T >::Type, T >( elt );
    }

  Py_XDECREF( newPyObj );
  return p_coll;
}






template <>
struct traitsPythonType< NumericalPoint >
{
  typedef _PySequence_ Type;
};

template <>
inline
NumericalPoint
convert< _PySequence_, NumericalPoint >(PyObject * pyObj)
{
  Pointer<Collection<NumericalScalar> > ptr = buildCollectionFromPySequence<NumericalScalar>( pyObj );
  return NumericalPoint( *ptr );
}

template <>
inline
PyObject *
convert< NumericalPoint, _PySequence_ >(NumericalPoint inP)
{
  UnsignedLong dimension = inP.getDimension();
  PyObject * point = PyTuple_New( dimension );
  for ( UnsignedLong i = 0; i < dimension; ++ i )
    {
      PyTuple_SetItem( point, i, convert< NumericalScalar, _PyFloat_ >( inP[i] ) );
    }
  return point;
}


template <>
struct traitsPythonType< Collection < NumericalComplex> >
{
  typedef _PySequence_ Type;
};


template <>
inline
Collection<NumericalComplex>
convert<_PySequence_, Collection<NumericalComplex> >(PyObject * pyObj)
{
  Pointer<Collection<NumericalComplex> > ptr = buildCollectionFromPySequence<NumericalComplex>( pyObj );
  return Collection<NumericalComplex>( *ptr );
}


template <>
struct traitsPythonType< NumericalSample >
{
  typedef _PySequence_ Type;
};

template <>
inline
NumericalSample
convert< _PySequence_, NumericalSample >(PyObject * pyObj)
{
  Pointer<Collection<NumericalPoint> > ptr = buildCollectionFromPySequence<NumericalPoint>( pyObj );
  return NumericalSample( *ptr );
}



template <>
struct traitsPythonType< TimeSeries >
{
  typedef _PySequence_ Type;
};

template <>
inline
TimeSeries
convert< _PySequence_, TimeSeries >(PyObject * pyObj)
{
  Pointer<Collection<NumericalPoint> > ptr = buildCollectionFromPySequence<NumericalPoint>( pyObj );
  return TimeSeries( *ptr );
}


template <>
struct traitsPythonType< Collection< UnsignedLong > >
{
  typedef _PySequence_ Type;
};

template <>
inline
Collection< UnsignedLong >
convert< _PySequence_, Collection<UnsignedLong> >(PyObject * pyObj)
{
  Pointer<Collection<UnsignedLong> > ptr = buildCollectionFromPySequence<UnsignedLong>( pyObj );
  return Collection<UnsignedLong>( ptr->begin(), ptr->end() );
}


template <>
struct traitsPythonType< Indices >
{
  typedef _PySequence_ Type;
};

template <>
inline
Indices
convert< _PySequence_, Indices >(PyObject * pyObj)
{
  Pointer<Collection<UnsignedLong> > ptr = buildCollectionFromPySequence<UnsignedLong>( pyObj );
  return Indices( ptr->begin(), ptr->end() );
}



template <>
struct traitsPythonType< Description >
{
  typedef _PySequence_ Type;
};

template <>
inline
Description
convert<_PySequence_, Description>(PyObject * pyObj)
{
  Pointer<Collection<String> > ptr = buildCollectionFromPySequence<String>( pyObj );
  return Description( *ptr );
}



template <>
struct traitsPythonType< Collection< NumericalScalar > >
{
  typedef _PySequence_ Type;
};

template <>
inline
Collection<NumericalScalar>
convert< _PySequence_, Collection<NumericalScalar> >(PyObject * pyObj)
{
  Pointer<Collection<NumericalScalar> > ptr = buildCollectionFromPySequence<NumericalScalar>( pyObj );
  return Collection<NumericalScalar>( *ptr );
}



template <>
struct traitsPythonType< MatrixImplementation >
{
  typedef _PySequence_ Type;
};



template <>
inline
MatrixImplementation*
convert< _PySequence_, MatrixImplementation* >(PyObject * pyObj)
{
  // use the same conversion function for numpy array/matrix, knowing numpy matrix is not a sequence
  if ( PyObject_HasAttrString(pyObj, const_cast<char *>("shape")) )
    {
      PyObject * shapeObj = PyObject_GetAttrString( pyObj, "shape" );
      if ( shapeObj )
        {
          Indices shape( checkAndConvert< _PySequence_, Indices >( shapeObj ) );
          Py_XDECREF( shapeObj );
          if ( shape.getSize() == 2 )
            {
              UnsignedLong nbRows = shape[0];
              UnsignedLong nbColumns = shape[1];
              PyObject * askObj = PyTuple_New(2);
              PyObject * methodObj = convert< String, _PyString_ >("__getitem__");
              MatrixImplementation *p_implementation = new MatrixImplementation( nbRows, nbColumns );
              for ( UnsignedLong i = 0; i < nbRows; ++ i )
                {
                  PyTuple_SetItem( askObj, 0, convert< UnsignedLong, _PyInt_ >(i) );
                  for ( UnsignedLong j = 0; j < nbColumns; ++ j )
                    {
                      PyTuple_SetItem( askObj, 1, convert< UnsignedLong, _PyInt_ >(j) );
                      PyObject * elt = PyObject_CallMethodObjArgs( pyObj, methodObj, askObj, NULL );
                      if ( elt )
                        {
                          try
                            {
                              p_implementation->operator()( i, j ) = checkAndConvert<_PyFloat_, NumericalScalar>( elt );
                            }
                          catch( InvalidArgumentException & ex )
                            {
                              Py_XDECREF( askObj );
                              Py_XDECREF( methodObj );
                              delete p_implementation;
                              Py_XDECREF( elt );
                              throw;
                            }
                          Py_XDECREF( elt );
                        }
                    }
                }
              Py_XDECREF( askObj );
              Py_XDECREF( methodObj );
              return p_implementation;
            }
          else
            throw InvalidArgumentException(HERE) << "Invalid array dimension: " << shape.getSize();
        }
    }

  // else try to convert from a sequence of sequences
  Pointer< Collection< NumericalPoint > > ptr = buildCollectionFromPySequence< NumericalPoint >( pyObj );
  NumericalSample sample( *ptr );
  UnsignedLong nbRows = sample.getSize();
  UnsignedLong nbColumns = sample.getDimension();
  MatrixImplementation *p_implementation = new MatrixImplementation( nbRows, nbColumns );
  for ( UnsignedLong i = 0; i < nbRows; ++ i )
    for ( UnsignedLong j = 0; j < nbColumns; ++ j )
      p_implementation->operator()( i, j ) = sample[i][j];
  return p_implementation;
}



template <>
inline
Matrix
convert< _PySequence_, Matrix >(PyObject * pyObj)
{
  MatrixImplementation *p_implementation = convert< _PySequence_, MatrixImplementation* >(pyObj);
  return Matrix( p_implementation );
}



template <>
inline
SquareMatrix
convert< _PySequence_, SquareMatrix >(PyObject * pyObj)
{
  MatrixImplementation *p_implementation = convert< _PySequence_, MatrixImplementation* >(pyObj);
  if ( p_implementation->getNbRows() != p_implementation->getNbColumns() )
    throw InvalidArgumentException(HERE) << "The matrix is not square";
  return SquareMatrix( p_implementation );
}



template <>
inline
SymmetricMatrix
convert< _PySequence_, SymmetricMatrix >(PyObject * pyObj)
{
  MatrixImplementation *p_implementation = convert< _PySequence_, MatrixImplementation* >(pyObj);
  if (!p_implementation->isSymmetric())
    throw InvalidArgumentException(HERE) << "The matrix is not symmetric";
  return SymmetricMatrix( p_implementation );
}



template <>
inline
CovarianceMatrix
convert< _PySequence_, CovarianceMatrix >(PyObject * pyObj)
{
  MatrixImplementation *p_implementation = convert< _PySequence_, MatrixImplementation* >(pyObj);
  if (!p_implementation->isSymmetric())
    throw InvalidArgumentException(HERE) << "The matrix is not symmetric";
  // SPD check is too expensive
  return CovarianceMatrix( p_implementation );
}



template <>
inline
CorrelationMatrix
convert< _PySequence_, CorrelationMatrix >(PyObject * pyObj)
{
  MatrixImplementation *p_implementation = convert< _PySequence_, MatrixImplementation* >(pyObj);
  if (!p_implementation->isSymmetric())
    throw InvalidArgumentException(HERE) << "The matrix is not symmetric";
  // SPD check is too expensive
  if (!p_implementation->hasUnitRange())
    throw InvalidArgumentException(HERE) << "The matrix range is not (-1;1)";
  return CorrelationMatrix( p_implementation );
}



template <>
struct traitsPythonType< TensorImplementation >
{
  typedef _PySequence_ Type;
};



template <>
inline
TensorImplementation*
convert< _PySequence_, TensorImplementation* >(PyObject * pyObj)
{
  Pointer< Collection< NumericalSample > > ptr = buildCollectionFromPySequence< NumericalSample >( pyObj );
  UnsignedLong nbRows = ptr->getSize();
  UnsignedLong nbColumns = ptr->getSize() > 0 ? (*ptr)[0].getSize() : 0;
  UnsignedLong nbSheets = ptr->getSize() > 0 ? (*ptr)[0].getDimension() : 0;
  TensorImplementation *p_implementation = new TensorImplementation( nbRows, nbColumns, nbSheets );
  for ( UnsignedLong i = 0; i < nbRows; ++ i )
    for ( UnsignedLong j = 0; j < nbColumns; ++ j )
      for ( UnsignedLong k = 0; k < nbSheets; ++ k )
        p_implementation->operator()( i, j, k ) = (*ptr)[i][j][k];
  return p_implementation;
}



template <>
inline
Tensor
convert< _PySequence_, Tensor >(PyObject * pyObj)
{
  TensorImplementation *p_implementation = convert<_PySequence_, TensorImplementation*>(pyObj);
  return Tensor( p_implementation );
}



template <>
inline
SymmetricTensor
convert< _PySequence_, SymmetricTensor >(PyObject * pyObj)
{
  TensorImplementation *p_implementation = convert< _PySequence_, TensorImplementation* >(pyObj);
  if (!p_implementation->isSymmetric())
    throw InvalidArgumentException(HERE) << "The tensor is not symmetric";
  return SymmetricTensor( p_implementation );
}



template <>
struct traitsPythonType< ComplexMatrixImplementation* >
{
  typedef _PySequence_ Type;
};



template <>
inline
ComplexMatrixImplementation*
convert< _PySequence_, ComplexMatrixImplementation* >(PyObject * pyObj)
{
  // use the same conversion function for numpy array/matrix, knowing numpy matrix is not a sequence
  if ( PyObject_HasAttrString(pyObj, const_cast<char *>("shape")) )
    {
      PyObject * shapeObj = PyObject_GetAttrString( pyObj, "shape" );
      if (shapeObj)
        {
          Indices shape( checkAndConvert< _PySequence_, Indices >( shapeObj ) );
          Py_XDECREF( shapeObj );
          if ( shape.getSize() == 2 )
            {
              UnsignedLong nbRows = shape[0];
              UnsignedLong nbColumns = shape[1];
              PyObject * askObj = PyTuple_New(2);
              PyObject * methodObj = convert< String, _PyString_ >("__getitem__");
              ComplexMatrixImplementation *p_implementation = new ComplexMatrixImplementation( nbRows, nbColumns );
              for ( UnsignedLong i = 0; i < nbRows; ++ i )
                {
                  PyTuple_SetItem( askObj, 0, convert< UnsignedLong, _PyInt_ >(i) );
                  for ( UnsignedLong j = 0; j < nbColumns; ++ j )
                    {
                      PyTuple_SetItem( askObj, 1, convert< UnsignedLong, _PyInt_ >(j) );
                      PyObject * elt = PyObject_CallMethodObjArgs( pyObj, methodObj, askObj, NULL );
                      if ( elt )
                        {
                          try
                            {
                              p_implementation->operator()( i, j ) = checkAndConvert<_PyComplex_, NumericalComplex>( elt );
                            }
                          catch( InvalidArgumentException & ex )
                            {
                              Py_XDECREF( askObj );
                              Py_XDECREF( methodObj );
                              delete p_implementation;
                              Py_XDECREF( elt );
                              throw;
                            }
                          Py_XDECREF( elt );
                        }
                    }
                }
              Py_XDECREF( askObj );
              Py_XDECREF( methodObj );
              return p_implementation;
            }
          else
            throw InvalidArgumentException(HERE) << "Invalid array dimension: " << shape.getSize();
        }
    }

  // else try to convert from a sequence of sequences
  Pointer< Collection< Collection< NumericalComplex > > > ptr = buildCollectionFromPySequence< Collection< NumericalComplex > >( pyObj );
  UnsignedLong nbRows = ptr->getSize();
  UnsignedLong nbColumns = ptr->getSize() > 0 ? (*ptr)[0].getSize() : 0;
  ComplexMatrixImplementation *p_implementation = new ComplexMatrixImplementation( nbRows, nbColumns );
  for ( UnsignedLong i = 0; i < nbRows; ++ i )
    for ( UnsignedLong j = 0; j < nbColumns; ++ j )
      p_implementation->operator()( i, j ) = (*ptr)[i][j];
  return p_implementation;
}



template <>
inline
ComplexMatrix
convert< _PySequence_, ComplexMatrix >(PyObject * pyObj)
{
  ComplexMatrixImplementation *p_implementation = convert< _PySequence_, ComplexMatrixImplementation* >(pyObj);
  return ComplexMatrix( p_implementation );
}



template <>
inline
SquareComplexMatrix
convert< _PySequence_, SquareComplexMatrix >(PyObject * pyObj)
{
  ComplexMatrixImplementation *p_implementation = convert< _PySequence_, ComplexMatrixImplementation* >(pyObj);
  if ( p_implementation->getNbRows() != p_implementation->getNbColumns() )
    throw InvalidArgumentException(HERE) << "The matrix is not square";
  return SquareComplexMatrix( p_implementation );
}



template <>
inline
TriangularComplexMatrix
convert< _PySequence_, TriangularComplexMatrix >(PyObject * pyObj)
{
  ComplexMatrixImplementation *p_implementation = convert< _PySequence_, ComplexMatrixImplementation* >(pyObj);
  Bool lower = p_implementation->isTriangular(true);
  if ( !lower && !p_implementation->isTriangular(false) )
    throw InvalidArgumentException(HERE) << "The matrix is not triangular";
  return TriangularComplexMatrix( p_implementation, lower );
}


template <>
struct traitsPythonType< HermitianMatrix >
{
  typedef _PySequence_ Type;
};

template <>
inline
HermitianMatrix
convert< _PySequence_, HermitianMatrix >(PyObject * pyObj)
{
  ComplexMatrixImplementation *p_implementation = convert< _PySequence_, ComplexMatrixImplementation* >(pyObj);
  if ( !p_implementation->isHermitian() )
    throw InvalidArgumentException(HERE) << "The matrix is not hermitian";
  return HermitianMatrix( p_implementation );
}



template <>
struct traitsPythonType< WrapperDataFile >
{
  typedef _PySequence_ Type;
};

template <>
inline
WrapperDataFile
convert< _PySequence_, WrapperDataFile >(PyObject * pyObj)
{
  return WrapperDataFile();
}



template <>
struct traitsPythonType< WrapperDataVariable >
{
  typedef _PySequence_ Type;
};

template <>
inline
WrapperDataVariable
convert< _PySequence_, WrapperDataVariable >(PyObject * pyObj)
{
  return WrapperDataVariable();
}

template <>
struct traitsPythonType< WhittleFactoryState>
{
  typedef _PySequence_ Type;
};

template <>
inline
WhittleFactoryState
convert< _PySequence_, WhittleFactoryState >(PyObject * pyObj)
{
  return WhittleFactoryState();
}


inline
void handleException()
{
  PyObject * exception = PyErr_Occurred();

  if ( exception )
    {

      PyObject *type = NULL, *value = NULL, *traceback = NULL;
      PyErr_Fetch( &type, &value, &traceback );

      String exceptionMessage("Python exception caught.");

      // try to get error msg, value and traceback can be NULL
      try {
        String typeString = checkAndConvert< _PyString_, String >(type);
        exceptionMessage += " Type=" + typeString;

        String valueString = checkAndConvert< _PyString_, String >(value);
        exceptionMessage += " Value=" + valueString;

      } catch(InvalidArgumentException & ex) {
        // could not get msg from strings
      }

      PyErr_Restore( type, value, traceback );

      if ( PyErr_ExceptionMatches(PyExc_RuntimeError) )
        throw InternalException(HERE) << "Call to Python method failed. " + exceptionMessage;
      else if ( PyErr_ExceptionMatches( PyExc_TypeError ) )
        throw InvalidArgumentException(HERE) << "Invalid argument passed to Python method. " + exceptionMessage;
      else
        {
          throw InternalException(HERE) << exceptionMessage;
        }
    }
}


inline
void pickleSave(Advocate & adv, PyObject * pyObj)
{
  PyObject * pickleModule = PyImport_ImportModule( "pickle" ); // new reference
  assert( pickleModule );

  PyObject * pickleDict = PyModule_GetDict( pickleModule );
  assert(pickleDict);

  PyObject * dumpsMethod = PyDict_GetItemString( pickleDict, "dumps" );
  assert( dumpsMethod );
  if ( ! PyCallable_Check( dumpsMethod ) )
    throw InternalException(HERE) << "Python 'pickle' module has no 'dumps' method";

  assert( pyObj );
  PyObject * rawDump = PyObject_CallFunctionObjArgs( dumpsMethod, pyObj, NULL ); // new reference
  Py_XDECREF( pickleModule );

  if (PyErr_Occurred()) PyErr_Print();
  assert( rawDump );

  PyObject * base64Module = PyImport_ImportModule( "base64" ); // new reference
  assert( base64Module );

  PyObject * base64Dict = PyModule_GetDict( base64Module );
  assert( base64Dict );

  PyObject * b64encodeMethod = PyDict_GetItemString( base64Dict, "standard_b64encode" );
  assert( b64encodeMethod );
  if ( ! PyCallable_Check( b64encodeMethod ) )
    throw InternalException(HERE) << "Python 'base64' module has no 'standard_b64encode' method";

  PyObject * base64Dump = PyObject_CallFunctionObjArgs( b64encodeMethod, rawDump, NULL ); // new reference
  Py_XDECREF( base64Module );
  Py_XDECREF( rawDump );
  if (PyErr_Occurred()) PyErr_Print();
  assert( base64Dump );

  String pyInstanceSt(convert< _PyBytes_, String >( base64Dump ));
  adv.saveAttribute( "pyInstance_", pyInstanceSt );

  Py_XDECREF( base64Dump );
}


inline
void pickleLoad(Advocate & adv, PyObject * & pyObj)
{
  String pyInstanceSt;
  adv.loadAttribute( "pyInstance_", pyInstanceSt );

  PyObject * base64Dump = convert< String, _PyBytes_ >( pyInstanceSt ); // new reference
  assert( base64Dump );

  PyObject * base64Module = PyImport_ImportModule( "base64" ); // new reference
  assert( base64Module );

  PyObject * base64Dict = PyModule_GetDict( base64Module );
  assert( base64Dict );

  PyObject * b64decodeMethod = PyDict_GetItemString( base64Dict, "standard_b64decode" );
  assert( b64decodeMethod );
  if ( ! PyCallable_Check( b64decodeMethod ) )
    throw InternalException(HERE) << "Python 'base64' module has no 'standard_b64decode' method";

  PyObject * rawDump = PyObject_CallFunctionObjArgs( b64decodeMethod, base64Dump, NULL ); // new reference
  Py_XDECREF( base64Module );
  Py_XDECREF( base64Dump );
  if (PyErr_Occurred()) PyErr_Print();
  assert( rawDump );

  PyObject * pickleModule = PyImport_ImportModule( "pickle" ); // new reference
  assert( pickleModule );

  PyObject * pickleDict = PyModule_GetDict( pickleModule );
  assert( pickleDict );

  PyObject * loadsMethod = PyDict_GetItemString( pickleDict, "loads" );
  assert( loadsMethod );
  if ( ! PyCallable_Check( loadsMethod ) )
    throw InternalException(HERE) << "Python 'pickle' module has no 'loads' method";

  Py_XDECREF( pyObj );
  pyObj = PyObject_CallFunctionObjArgs( loadsMethod, rawDump, NULL ); // new reference
  Py_XDECREF( pickleModule );
  Py_XDECREF( rawDump );
  if (PyErr_Occurred()) PyErr_Print();
  assert( pyObj );
}


// PySliceObject type is deprecated
#if PY_VERSION_HEX >= 0x03020000
inline PyObject* SliceCast(PyObject* pyObj)
{
  return pyObj;
}
#else
inline PySliceObject* SliceCast(PyObject* pyObj)
{
  return (PySliceObject*)pyObj;
}
#endif


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PYTHONWRAPPINGFUNCTIONS_HXX */
