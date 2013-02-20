// SWIG file ConfidenceInterval.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ConfidenceInterval.hxx"

namespace OT { 

  template <>
  struct traitsPythonType<OT::ConfidenceInterval>
  {
    typedef _PySequence_ Type;
  };

  template <>
  inline
  OT::ConfidenceInterval
  convert<_PySequence_,OT::ConfidenceInterval>(PyObject * pyObj)
  {
    check<_PySequence_>( pyObj );
    PyObject * newPyObj = PySequence_Fast( pyObj, "" );

    if (PySequence_Fast_GET_SIZE( newPyObj ) != 2) 
      throw InvalidArgumentException(HERE) << "Sequence passed as argument is not a pair (NumericalScalar, NumericalScalar)";
    PyObject * item_0 = PySequence_Fast_GET_ITEM( newPyObj, 0 );
    check<_PyFloat_>( item_0 );
    PyObject * item_1 = PySequence_Fast_GET_ITEM( newPyObj, 1 );
    check<_PyFloat_>( item_1 );

    Py_XDECREF( newPyObj );
    return ConfidenceInterval( convert<_PyFloat_,NumericalScalar>( item_0 ),
			                   convert<_PyFloat_,NumericalScalar>( item_1 ) );
  }


} /* namespace OT */

%}

%include ConfidenceInterval.hxx
namespace OT{  

%extend ConfidenceInterval {

ConfidenceInterval(const ConfidenceInterval & other) { return new OT::ConfidenceInterval(other); }

ConfidenceInterval(PyObject * pyObj)
{
 return new OT::ConfidenceInterval( OT::convert<OT::_PySequence_,OT::ConfidenceInterval>(pyObj) );
}

}

}
