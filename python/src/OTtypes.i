// SWIG file OTtypes.i
// @author dutka
// @date   2011-11-17 13:46:13 +0100 (Thu, 17 Nov 2011)

%module types

%include std_string.i
%include std_map.i
%include std_complex.i

%include OT.hxx
%include OTtypes.hxx
%include OTdebug.h

%{
#include "OT.hxx"
#include "PythonWrappingFunctions.hxx"
%}

%typemap(in) UnsignedLong {
  $1 = OT::convert< OT::_PyInt_, OT::UnsignedLong >($input);
}

%typemap(in) NumericalScalar {
  $1 = OT::convert< OT::_PyFloat_, OT::NumericalScalar >($input);
}

%typemap(in) NumericalComplex {
  $1 = OT::convert< OT::_PyComplex_, OT::NumericalComplex >($input);
}

%typemap(out) NumericalComplex {
  $result = OT::convert< OT::NumericalComplex, OT::_PyComplex_ >($1);
}



%ignore *::clone() const;
%ignore *::load(Advocate & adv);
%ignore *::save(Advocate & adv) const;
%ignore *::save(StorageManager & mgr, const String & label, bool fromStudy) const;
%ignore *::save(StorageManager & mgr, const String & label) const;
%ignore *::save(StorageManager & mgr, bool fromStudy) const;
%ignore *::save(StorageManager & mgr) const;
