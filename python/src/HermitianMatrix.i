// SWIG file HermitianMatrix.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "HermitianMatrix.hxx"
%}

%include HermitianMatrix.hxx

namespace OT {

%extend HermitianMatrix {

  HermitianMatrix(const HermitianMatrix & other) { return new OT::HermitianMatrix(other); }

  HermitianMatrix(PyObject * pyObj) { return new OT::HermitianMatrix( OT::convert<OT::_PySequence_,OT::HermitianMatrix>(pyObj) ); }

  OTMatrixAccessors(HermitianMatrix, NumericalComplex)

  HermitianMatrix __rmul__(NumericalComplex s) { return s * (*self); }
  
  HermitianMatrix __truediv__(NumericalComplex s) { return (*self) / s; }
  
} // HermitianMatrix
} // OT
