// SWIG file NumericalPointWithDescription.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NumericalPointWithDescription.hxx"

namespace OT { 

  template <>
  struct traitsPythonType<OT::NumericalPointWithDescription>
  {
    typedef _PySequence_ Type;
  };

  template <>
  inline
  OT::NumericalPointWithDescription
  convert<_PySequence_,OT::NumericalPointWithDescription>(PyObject * pyObj)
  {
    check<_PySequence_>( pyObj );
    PyObject * newPyObj = PySequence_Fast( pyObj, "" );

    const OT::UnsignedLong size = PySequence_Fast_GET_SIZE( newPyObj );
    OT::NumericalPointWithDescription * p_np = new OT::NumericalPointWithDescription( size );
    OT::Description desc = OT::Description( size );

    for(OT::UnsignedLong i=0; i<size; ++i) {
      PyObject * elt = PySequence_Fast_GET_ITEM( newPyObj, i );
      check<_PySequence_>( elt );
      if (PySequence_Fast_GET_SIZE( elt ) == 2) {
        PyObject * item_0 = PySequence_Fast_GET_ITEM( elt, 0 );
        check<_PyString_>( item_0 );
        desc[i] = convert<_PyString_,OT::String>( item_0 );
        PyObject * item_1 = PySequence_Fast_GET_ITEM( elt, 1 );
        check<_PyFloat_>( item_1 );
        (*p_np)[i] = convert<_PyFloat_,OT::NumericalScalar>( item_1 );
      } else throw OT::InvalidArgumentException(HERE) << "Sequence item " << i << " passed as argument is not a pair (String, NumericalScalar)";
    }

    Py_XDECREF( newPyObj );
    p_np->setDescription( desc );
    return *p_np;
  }


} /* namespace OT */

%}

%rename(NumericalPointWithDescription_operator___eq__) OT::operator ==(const NumericalPointWithDescription & lhs, const NumericalPointWithDescription & rhs);

%include NumericalPointWithDescription.hxx
namespace OT{  

%extend NumericalPointWithDescription {

NumericalPointWithDescription(const NumericalPointWithDescription & other)
{
  return new OT::NumericalPointWithDescription(other);
}


NumericalPointWithDescription(PyObject * pyObj)
{
  return new OT::NumericalPointWithDescription( OT::convert<OT::_PySequence_,OT::NumericalPointWithDescription>(pyObj) );
}

}
}

%template(NumericalPointWithDescriptionCollection) OT::Collection<OT::NumericalPointWithDescription>;
%template(NumericalPointWithDescriptionPersistentCollection) OT::PersistentCollection<OT::NumericalPointWithDescription>;
