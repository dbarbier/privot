// SWIG file SymmetricTensor.i
// @author schueller
// @date   2012-02-15 18:09:50 +0100 (Wed, 15 Feb 2012)

%{
#include "SymmetricTensor.hxx"
%}

%apply const NumericalScalarCollection & { const OT::SymmetricTensor::NumericalScalarCollection & };

%include SymmetricTensor.hxx

namespace OT {  

%extend SymmetricTensor {

  SymmetricTensor(const SymmetricTensor & other) { return new OT::SymmetricTensor(other); }

  SymmetricTensor(PyObject * pyObj) { return new OT::SymmetricTensor( OT::convert<OT::_PySequence_,OT::SymmetricTensor>(pyObj) ); }

  OT_TENSOR_ACCESSORS(SymmetricTensor)

} // SymmetricTensor
} // OT
