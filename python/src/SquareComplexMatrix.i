// SWIG file SquareComplexMatrix.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "SquareComplexMatrix.hxx"
%}

%include SquareComplexMatrix.hxx

namespace OT {

%extend SquareComplexMatrix {

  SquareComplexMatrix(const SquareComplexMatrix & other) { return new OT::SquareComplexMatrix(other); }

  SquareComplexMatrix(PyObject * pyObj) { return new OT::SquareComplexMatrix( OT::convert<OT::_PySequence_,OT::SquareComplexMatrix>(pyObj) ); }

  OTMatrixAccessors(SquareComplexMatrix, NumericalComplex)

  SquareComplexMatrix __rmul__(NumericalComplex s) { return s * (*self); }
  
  SquareComplexMatrix __truediv__(NumericalComplex s) { return (*self) / s; }
  
} // SquareComplexMatrix
} // OT
