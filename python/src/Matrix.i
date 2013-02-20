// SWIG file Matrix.i
// @author schueller
// @date   2012-02-15 18:09:50 +0100 (Wed, 15 Feb 2012)

%{
#include "Matrix.hxx"
%}

%template(MatrixImplementationTypedInterfaceObject) OT::TypedInterfaceObject<OT::MatrixImplementation>;
%apply const NumericalScalarCollection & { const OT::Matrix::NumericalScalarCollection & };

%define OTMatrixGetAccessor(matrixType, elementType)
elementType __getitem__(PyObject * args) const {
  OT::UnsignedLong arg2 ;
  OT::UnsignedLong arg3 ;
  unsigned long val2 ;
  int ecode2 = 0 ;
  unsigned long val3 ;
  int ecode3 = 0 ;
  PyObject * obj1 = 0 ;
  PyObject * obj2 = 0 ;

  if (!PyArg_ParseTuple(args,(char *)"OO:" #matrixType "___getitem__",&obj1,&obj2)) SWIG_fail;

  ecode2 = SWIG_AsVal_unsigned_SS_long(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" #matrixType "___getitem__" "', argument " "2"" of type '" "OT::UnsignedLong""'");
  }
  arg2 = static_cast< OT::UnsignedLong >(val2);


  ecode3 = SWIG_AsVal_unsigned_SS_long(obj2, &val3);
  if (!SWIG_IsOK(ecode3)) {
    SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" #matrixType "___getitem__" "', argument " "3"" of type '" "OT::UnsignedLong""'");
  }
  arg3 = static_cast< OT::UnsignedLong >(val3);

  return (*self)(arg2,arg3);
fail:
  return 0.;
}
%enddef

%define OTMatrixSetAccessor(matrixType, elementType)
void __setitem__(PyObject * args, elementType val) {
  OT::UnsignedLong arg2 ;
  OT::UnsignedLong arg3 ;
  unsigned long val2 ;
  int ecode2 = 0 ;
  unsigned long val3 ;
  int ecode3 = 0 ;
  PyObject * obj1 = 0 ;
  PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:" #matrixType "___setitem__",&obj1,&obj2)) SWIG_fail;

  ecode2 = SWIG_AsVal_unsigned_SS_long(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" #matrixType "___setitem__" "', argument " "2"" of type '" "OT::UnsignedLong""'");
  }
  arg2 = static_cast< OT::UnsignedLong >(val2);

  ecode3 = SWIG_AsVal_unsigned_SS_long(obj2, &val3);
  if (!SWIG_IsOK(ecode3)) {
    SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" #matrixType "___setitem__" "', argument " "3"" of type '" "OT::UnsignedLong""'");
  }
  arg3 = static_cast< OT::UnsignedLong >(val3);

  (*self)(arg2,arg3) = val;
fail:
  return;
}
%enddef

%define OTMatrixAccessors(matrixType, elementType)
OTMatrixGetAccessor(matrixType, elementType)
OTMatrixSetAccessor(matrixType, elementType)
%enddef

%include Matrix.hxx

%pythoncode %{
# This code has been added to conform to Numpy ndarray interface
# that tries to reuse the data stored in the Matrix (zero copy)
# see http://docs.scipy.org/doc/numpy/reference/arrays.interface.html#arrays-interface
# for details.
# See python doc http://docs.python.org/reference/datamodel.html?highlight=getattribute#object.__getattribute__
# for details on how to write such a method.
def Matrix___getattribute__(self, name):
  """__getattribute__(self, name) -> value"""
  if (name == '__array_interface__'):
    self.__dict__['__array_interface__'] = { 'shape'   : ( self.getNbRows(), self.getNbColumns() ),
               'typestr' : "|f" + str(self.__elementsize__()),
               'data'    : ( int(self.__baseaddress__()), True),
               'strides' : ( self.__stride__(0), self.__stride__(1) ),
               'version' : 3,
    }
  return object.__getattribute__(self,name)
Matrix.__getattribute__ = Matrix___getattribute__
%}

namespace OT {  

%extend Matrix {

  Matrix(const Matrix & other) { return new OT::Matrix(other); }

  Matrix(PyObject * pyObj) { return new OT::Matrix( OT::convert<OT::_PySequence_,OT::Matrix>(pyObj) ); }

  OTMatrixAccessors(Matrix, NumericalScalar)

  Matrix __rmul__(NumericalScalar s) { return s * (*self); }
    
  Matrix __truediv__(NumericalScalar s) { return (*self) / s; }

} // Matrix
} // OT

