// SWIG file ComplexMatrixImplementation.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "ComplexMatrixImplementation.hxx"
%}

%rename(ComplexMatrixImplementation_operator___eq__) OT::operator ==(const ComplexMatrixImplementation & lhs, const ComplexMatrixImplementation & rhs);

%template(NumericalComplexCollection)                   OT::Collection<OT::NumericalComplex>;
%template(NumericalComplexPersistenCollection)          OT::PersistentCollection<OT::NumericalComplex>;

%typemap(in) const NumericalComplexCollection & ($1_basetype temp) {
  if (! SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0))) {
    temp = OT::convert<OT::_PySequence_,OT::Collection<OT::NumericalComplex> >( $input );
    $1 = &temp;
  }
}

%typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) const NumericalComplexCollection & {
  $1 = SWIG_IsOK(SWIG_ConvertPtr($input, NULL, $1_descriptor, 0)) ||
       OT::isAPythonSequenceOf<OT::_PyComplex_>( $input );
}

%include ComplexMatrixImplementation.hxx
namespace OT{ %extend ComplexMatrixImplementation { ComplexMatrixImplementation(const ComplexMatrixImplementation & other) { return new OT::ComplexMatrixImplementation(other); } } }
