// SWIG file Interval.i
// @author schueller
// @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)

%{
#include "PythonWrappingFunctions.hxx"
#include "Interval.hxx"
%}

// class UnsignedLongCollection is defined in wrapper_module.i (module wrapper)
%pythoncode %{
# We have to make sure the submodule is loaded with absolute path
import openturns.wrapper

class BoolCollection(openturns.wrapper.UnsignedLongCollection): pass
%}

// we check for boolean type but they are stored as integers
%typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) const BoolCollection & {
  $1 = SWIG_IsOK(SWIG_ConvertPtr($input, NULL, $1_descriptor, 0)) || OT::isAPythonSequenceOf<OT::_PyBool_>( $input );
}

%typemap(in) const BoolCollection & ($1_basetype temp) {
  if (! SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0))) {
    temp = OT::convert<OT::_PySequence_,OT::Collection<OT::UnsignedLong> >( $input );
    $1 = &temp;
  }
}

%apply const BoolCollection & { OT::Interval::BoolCollection & };

%rename(__eq__) OT::Interval::operator==(const Interval & rhs) const;

%include Interval.hxx

namespace OT { %extend Interval { Interval(const Interval & other) { return new OT::Interval(other); } } }
