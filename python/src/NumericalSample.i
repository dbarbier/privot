// SWIG file NumericalSample.i
// @author schueller
// @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)

%{
#include "NumericalSampleImplementation.hxx"
#include "NumericalSample.hxx"
%}

%template(NumericalSampleImplementationTypedInterfaceObject) OT::TypedInterfaceObject<OT::NumericalSampleImplementation>;

#define OT_TYPECHECK_NUMERICALSAMPLE 5

%typemap(in) const NumericalSample & ($1_basetype temp) {
  if (! SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0))) {
    temp = OT::convert<OT::_PySequence_,OT::NumericalSample>( $input );
    $1 = &temp;
  }
}

%typemap(typecheck,precedence=OT_TYPECHECK_NUMERICALSAMPLE) const NumericalSample & {
  $1 = SWIG_IsOK(SWIG_ConvertPtr($input, NULL, $1_descriptor, 0)) || OT::isAPythonSequenceOf<OT::_PySequence_>( $input );
}

%apply const NumericalSample & { const OT::NumericalSample & };

%include NumericalSampleImplementation.hxx
%include NumericalSample.hxx
//%copyctor NumericalSample;

%pythoncode %{
# This code has been added to conform to Numpy ndarray interface
# that tries to reuse the data stored in the NumericalSample (zero copy)
# see http://docs.scipy.org/doc/numpy/reference/arrays.interface.html#arrays-interface
# for details.
# See python doc http://docs.python.org/reference/datamodel.html?highlight=getattribute#object.__getattribute__
# for details on how to write such a method.
def NumericalSample___getattribute__(self, name):
  """__getattribute__(self, name) -> value"""
  if (name == '__array_interface__'):
    self.__dict__['__array_interface__'] = { 'shape'   : ( self.getSize(), self.getDimension() ),
                                            'typestr' : "|f" + str(self.__elementsize__()),
                                            'data'    : ( int(self.__baseaddress__()), True),
                                            'version' : 3, 
                                            }
  return object.__getattribute__(self,name)
NumericalSample.__getattribute__ = NumericalSample___getattribute__
%}

namespace OT {  
%extend NumericalSample {

PyObject * __getitem__(PyObject * args) const {

  Py_ssize_t start1;
  Py_ssize_t stop1;
  Py_ssize_t step1;
  Py_ssize_t slicelength1;

  // case #0: [slice] => NumericalSample
  if ( PySlice_Check( args ) )
  { 
    PySlice_GetIndicesEx( OT::SliceCast( args ), self->getSize(), &start1, &stop1, &step1, &slicelength1 );
    OT::NumericalSample result( slicelength1, self->getDimension() );
    for ( Py_ssize_t i = 0; i < slicelength1; ++ i )
    {
      result.at(i) = self->at( start1 + i*step1 );
    }
    return SWIG_NewPointerObj((new OT::NumericalSample(static_cast< const OT::NumericalSample& >(result))), SWIGTYPE_p_OT__NumericalSample, SWIG_POINTER_OWN |  0 );
  }

  PyObject * obj1 = 0 ;
  PyObject * obj2 = 0 ;
  
  // argument values
  OT::UnsignedLong arg2 = 0;
  OT::UnsignedLong arg3 = 0;
  
  if (!PyArg_ParseTuple(args,(char *)"OO:NumericalSample___getitem__",&obj1,&obj2)) SWIG_fail;

  Py_ssize_t start2;
  Py_ssize_t stop2;
  Py_ssize_t step2;
  Py_ssize_t slicelength2;

  // convert first list argument 
  if ( PySlice_Check( obj1 ) )
  { 
    PySlice_GetIndicesEx( OT::SliceCast( obj1 ), self->getSize(), &start1, &stop1, &step1, &slicelength1 );
  }
  else
  {
    unsigned long val2 ;
    int ecode2 = 0 ;
    ecode2 = SWIG_AsVal_unsigned_SS_long(obj1, &val2);
    if (!SWIG_IsOK(ecode2)) {
      SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "NumericalSample___getitem__" "', argument " "2"" of type '" "OT::UnsignedLong""'");
    }
    arg2 = static_cast< OT::UnsignedLong >(val2);
  }

  // convert second list argument
  if ( PySlice_Check( obj2 ) )
  {
    PySlice_GetIndicesEx( OT::SliceCast( obj2 ), self->getDimension(), &start2, &stop2, &step2, &slicelength2 );
  }
  else
  {
    unsigned long val3 ;
    int ecode3 = 0 ;
    ecode3 = SWIG_AsVal_unsigned_SS_long(obj2, &val3);
    if (!SWIG_IsOK(ecode3)) {
      SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "NumericalSample___getitem__" "', argument " "3"" of type '" "OT::UnsignedLong""'");
    }
    arg3 = static_cast< OT::UnsignedLong >(val3);
  }

  // handle arguments
  if ( PySlice_Check( obj1 ) )
  {

    if ( PySlice_Check( obj2 ) )
    {
      // case #1: [slice/slice] => NumericalSample
      OT::NumericalSample result( slicelength1, slicelength2 );
      for ( Py_ssize_t i = 0; i < slicelength1; ++ i )
      {
        for ( Py_ssize_t j = 0; j < slicelength2; ++ j )
        {
          result.at(i, j) = self->at( start1 + i*step1, start2 + j*step2 );
        }
      }
      return SWIG_NewPointerObj((new OT::NumericalSample(static_cast< const OT::NumericalSample& >(result))), SWIGTYPE_p_OT__NumericalSample, SWIG_POINTER_OWN |  0 );
    }
    else
    {
      // case #2: [slice/index] => NumericalSample
      OT::NumericalSample result( slicelength1, 1 );
      for ( Py_ssize_t i = 0; i < slicelength1; ++ i )
      {
        result.at(i, 0) = self->at( start1 + i*step1, arg3 );
      }
      return SWIG_NewPointerObj((new OT::NumericalSample(static_cast< const OT::NumericalSample& >(result))), SWIGTYPE_p_OT__NumericalSample, SWIG_POINTER_OWN |  0 );
    }

  }
  else
  {
    if ( PySlice_Check( obj2 ) )
    {
      // case #3: [index/slice] => NumericalPoint
      OT::NumericalPoint result( slicelength2 );
      for ( Py_ssize_t j = 0; j < slicelength2; ++ j )
      {
        result.at(j) = self->at( arg2, start2 + j*step2 );
      }
      return SWIG_NewPointerObj((new OT::NumericalPoint(static_cast< const OT::NumericalPoint& >(result))), SWIGTYPE_p_OT__NumericalPoint, SWIG_POINTER_OWN |  0 );
    }
    else
    {  
      // case #4: [index/index] => float
      return OT::convert< OT::NumericalScalar, OT::_PyFloat_>( self->at(arg2, arg3) );
    }
  }
fail:
  return NULL;
}



void __setitem__(PyObject * args, PyObject * valObj) {

  Py_ssize_t start1;
  Py_ssize_t stop1;
  Py_ssize_t step1;
  Py_ssize_t slicelength1;

  // case #0: [slice] <= NumericalSample
  if ( PySlice_Check( args ) )
  {
    PySlice_GetIndicesEx( OT::SliceCast( args ), self->getSize(), &start1, &stop1, &step1, &slicelength1 );
    OT::NumericalSample temp2 ;
    OT::NumericalSample *val2 = 0 ;
    if (! SWIG_IsOK(SWIG_ConvertPtr(valObj, (void **) &val2, SWIGTYPE_p_OT__NumericalSample, 0))) {
      temp2 = OT::convert< OT::_PySequence_, OT::NumericalSample >( valObj );
      val2 = &temp2;
    }
    assert( val2 );
    for ( Py_ssize_t i = 0; i < slicelength1; ++ i )
    {
      self->at( start1 + i*step1 ) = val2->at(i);
    }
    return;
  }

  PyObject * obj1 = 0 ;
  PyObject * obj2 = 0 ;

  // argument values
  OT::UnsignedLong arg2 = 0;
  OT::UnsignedLong arg3 = 0;
  
  if (!PyArg_ParseTuple(args,(char *)"OO:NumericalSample___getitem__",&obj1,&obj2)) SWIG_fail;

  Py_ssize_t start2;
  Py_ssize_t stop2;
  Py_ssize_t step2;
  Py_ssize_t slicelength2;

  // convert first list argument 
  if ( PySlice_Check( obj1 ) )
  { 
    PySlice_GetIndicesEx( OT::SliceCast( obj1 ), self->getSize(), &start1, &stop1, &step1, &slicelength1 );
  }
  else
  {
    unsigned long val2 ;
    int ecode2 = 0 ;
    ecode2 = SWIG_AsVal_unsigned_SS_long(obj1, &val2);
    if (!SWIG_IsOK(ecode2)) {
      SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "NumericalSample___setitem__" "', argument " "2"" of type '" "OT::UnsignedLong""'");
    }
    arg2 = static_cast< OT::UnsignedLong >(val2);
  }

  // convert second list argument
  if ( PySlice_Check( obj2 ) )
  {
    PySlice_GetIndicesEx( OT::SliceCast( obj2 ), self->getDimension(), &start2, &stop2, &step2, &slicelength2 );
  }
  else
  {
    unsigned long val3 ;
    int ecode3 = 0 ;
    ecode3 = SWIG_AsVal_unsigned_SS_long(obj2, &val3);
    if (!SWIG_IsOK(ecode3)) {
      SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "NumericalSample___setitem__" "', argument " "3"" of type '" "OT::UnsignedLong""'");
    }
    arg3 = static_cast< OT::UnsignedLong >(val3);
  }

  // handle arguments
  if ( PySlice_Check( obj1 ) )
  {

    if ( PySlice_Check( obj2 ) )
    {
      // case #1: [slice/slice] <= NumericalSample
      OT::NumericalSample temp2 ;
      OT::NumericalSample *val2 = 0 ;
      if (! SWIG_IsOK(SWIG_ConvertPtr(valObj, (void **) &val2, SWIGTYPE_p_OT__NumericalSample, 0))) {
        temp2 = OT::convert<OT::_PySequence_,OT::NumericalSample>( valObj );
        val2 = &temp2;
      }
      for ( Py_ssize_t i = 0; i < slicelength1; ++ i )
      {
        for ( Py_ssize_t j = 0; j < slicelength2; ++ j )
        {
          self->at( start1 + i*step1, start2 + j*step2 ) = val2->at(i, j);
        }
      }
    }
    else
    {
      // case #2: [slice/index] <= NumericalSample
      OT::NumericalSample temp2 ;
      OT::NumericalSample *val2 = 0 ;
      if (! SWIG_IsOK(SWIG_ConvertPtr(valObj, (void **) &val2, SWIGTYPE_p_OT__NumericalSample, 0))) {
        temp2 = OT::convert<OT::_PySequence_,OT::NumericalSample>( valObj );
        val2 = &temp2;
      }
      for ( Py_ssize_t i = 0; i < slicelength1; ++ i )
      {
        self->at( start1 + i*step1, arg3 ) = val2->at(i, 0);
      }
    }

  }
  else
  {
    if ( PySlice_Check( obj2 ) )
    {
      // case #3: [index/slice] <= NumericalPoint
      OT::NumericalPoint temp2 ;
      OT::NumericalPoint *val2 = 0 ;
      if (! SWIG_IsOK(SWIG_ConvertPtr(valObj, (void **) &val2, SWIGTYPE_p_OT__NumericalPoint, 0))) {
        temp2 = OT::convert<OT::_PySequence_,OT::NumericalPoint>( valObj );
        val2 = &temp2;
      }
      for ( Py_ssize_t j = 0; j < slicelength2; ++ j )
      {
        self->at( arg2, start2 + j*step2 ) = val2->at(j);
      }
    }
    else
    {  
      // case #4: [index/index] <= float
      double val = 0.;
      int ecode2 = SWIG_AsVal_double(valObj, &val );
      if (!SWIG_IsOK(ecode2)) {
        SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "NumericalSample___setitem__" "', argument " "2"" of type '" "OT::NumericalScalar""'");
      }
      self->at(arg2, arg3) = val;
    }
  }
fail:
  return;
}

NumericalSample(const NumericalSample & other)
{
  return new OT::NumericalSample( other );
}

NumericalSample(PyObject * pyObj)
{
  return new OT::NumericalSample( OT::convert< OT::_PySequence_, OT::NumericalSample>(pyObj) );  
}

NumericalSample(PyObject * pyObj, UnsignedLong dimension)
{
  OT::NumericalPoint point( OT::convert< OT::_PySequence_, OT::NumericalPoint >(pyObj) );
  OT::UnsignedLong pointSize = point.getDimension();
  OT::UnsignedLong size = (pointSize + dimension - 1) / dimension;
  OT::NumericalSample sample(size, dimension);
  OT::UnsignedLong k = 0;
  for ( OT::UnsignedLong i = 0; i < size; ++ i ) {
    for ( OT::UnsignedLong j = 0; j < dimension; ++ j ) {
      if ( k < pointSize ) {
        sample[i][j] = point[k];
        ++ k;
      }
    }
  }
  return new OT::NumericalSample( sample );
}

Bool __eq__(const NumericalSample & other) { return (*self) == other; }

} // %extend
} // namespace OT
