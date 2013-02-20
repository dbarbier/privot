// SWIG file Indices.i
// @author schueller
// @date   2012-02-15 18:09:50 +0100 (Wed, 15 Feb 2012)

%{
#include "Indices.hxx"
%}

%typemap(in) const Indices & ($1_basetype temp) {
  if (! SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0))) {
    temp = OT::convert<OT::_PySequence_,OT::Indices>( $input );
    $1 = &temp;
  }
}

%typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) const Indices & {
  $1 = SWIG_IsOK(SWIG_ConvertPtr($input, NULL, $1_descriptor, 0)) ||
       OT::isAPythonSequenceOf<OT::_PyInt_>( $input );
}

%apply const Indices & { const OT::Indices & };

%include Indices.hxx
namespace OT {  

%extend Indices {

  Indices(const Indices & other) { return new OT::Indices(other); }  

  Indices(PyObject * pyObj) { return new OT::Indices( OT::convert<OT::_PySequence_,OT::Indices>(pyObj) ); }
  
  OT_COLLECTION_GETITEM(OT::Indices, OT::UnsignedLong)
  OT_COLLECTION_SETITEM(OT::Indices, OT::UnsignedLong)

  Bool __eq__(const Indices & other) { return (*self) == other; }

} // Indices

} // OT
