// SWIG file CorrelationMatrix.i
// @author schueller
// @date   2012-02-15 18:09:50 +0100 (Wed, 15 Feb 2012)

%{
#include "CorrelationMatrix.hxx"
%}

%apply const NumericalScalarCollection & { const OT::CorrelationMatrix::NumericalScalarCollection & };

%typemap(in) const CorrelationMatrix & ($1_basetype temp) {
  if (! SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0))) {
    temp = OT::convert<OT::_PySequence_,OT::CorrelationMatrix>( $input );
    $1 = &temp;
  }
}

%typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) const CorrelationMatrix & {
  $1 = SWIG_IsOK(SWIG_ConvertPtr($input, NULL, $1_descriptor, 0)) ||
       OT::isAPythonSequenceOf<OT::_PySequence_>( $input );
}

%include CorrelationMatrix.hxx

namespace OT {  

%extend CorrelationMatrix {

  CorrelationMatrix(const CorrelationMatrix & other) { return new OT::CorrelationMatrix(other); }

  CorrelationMatrix(PyObject * pyObj) { return new OT::CorrelationMatrix( OT::convert<OT::_PySequence_,OT::CorrelationMatrix>(pyObj) ); }

  OTMatrixAccessors(CorrelationMatrix, NumericalScalar)
  
  // These should be inherited from CovarianceMatrix
  NumericalPoint __mul__(NumericalPoint p) { return (*self) * p; }
  Matrix __mul__(NumericalScalar s) { return (*self) * s; }

} // CorrelationMatrix
} // OT
