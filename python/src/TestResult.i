// SWIG file TestResult.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "TestResult.hxx"

namespace OT { 

  template <>
  struct traitsPythonType<OT::TestResult>
  {
    typedef _PySequence_ Type;
  };

  template <>
  inline
  OT::TestResult
  convert<_PySequence_,OT::TestResult>(PyObject * pyObj)
  {
    check<_PySequence_>( pyObj );
    PyObject * newPyObj = PySequence_Fast( pyObj, "" );

    if (PySequence_Fast_GET_SIZE( newPyObj ) != 4) 
      throw InvalidArgumentException(HERE) << "Sequence passed as argument is not convertible to a TestResult. Expected sequence (String, Bool, NumericalScalar, NumericalScalar)";

    PyObject * item_0 = PySequence_Fast_GET_ITEM( newPyObj, 0 ); check<_PyString_>( item_0 );
    PyObject * item_1 = PySequence_Fast_GET_ITEM( newPyObj, 1 ); check<_PyBool_>( item_1 );
    PyObject * item_2 = PySequence_Fast_GET_ITEM( newPyObj, 2 ); check<_PyFloat_>( item_2 );
    PyObject * item_3 = PySequence_Fast_GET_ITEM( newPyObj, 3 ); check<_PyFloat_>( item_3 );

    Py_XDECREF( newPyObj );
    return OT::TestResult( convert<_PyString_,String>( item_0 ),
                                              convert<_PyBool_,Bool>( item_1 ),
                                              convert<_PyFloat_,NumericalScalar>( item_2 ),
                                              convert<_PyFloat_,NumericalScalar>( item_3 ) );
  }


} /* namespace OT */

%}

%include TestResult.hxx
namespace OT{  

%extend TestResult {

TestResult(const TestResult & other) { return new OT::TestResult(other); }

TestResult(PyObject * pyObj)
{
 return new OT::TestResult( OT::convert<OT::_PySequence_,OT::TestResult>(pyObj) );
}

}
}
