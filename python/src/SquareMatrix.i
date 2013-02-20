// SWIG file SquareMatrix.i
// @author schueller
// @date   2012-02-15 18:09:50 +0100 (Wed, 15 Feb 2012)

%{
#include "SquareMatrix.hxx"
%}

%apply const NumericalScalarCollection & { const OT::SquareMatrix::NumericalScalarCollection & };

%include SquareMatrix.hxx

namespace OT {  

%extend SquareMatrix {

  SquareMatrix(const SquareMatrix & other) { return new OT::SquareMatrix(other); }

  SquareMatrix(PyObject * pyObj) { return new OT::SquareMatrix( OT::convert<OT::_PySequence_,OT::SquareMatrix>(pyObj) ); }

  OTMatrixAccessors(SquareMatrix, NumericalScalar)
  
  SquareMatrix __rmul__(NumericalScalar s) { return s * (*self); }
    
  SquareMatrix __truediv__(NumericalScalar s) { return (*self) / s; }

} // SquareMatrix
} // OT
