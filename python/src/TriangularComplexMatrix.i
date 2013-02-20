// SWIG file TriangularComplexMatrix.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "TriangularComplexMatrix.hxx"
%}

%include TriangularComplexMatrix.hxx

namespace OT { 

%extend TriangularComplexMatrix {

  TriangularComplexMatrix(const TriangularComplexMatrix & other) { return new OT::TriangularComplexMatrix(other); }

  TriangularComplexMatrix(PyObject * pyObj) { return new OT::TriangularComplexMatrix( OT::convert<OT::_PySequence_,OT::TriangularComplexMatrix>(pyObj) ); }

  OTMatrixAccessors(TriangularComplexMatrix, NumericalComplex)

  TriangularComplexMatrix __rmul__(NumericalComplex s) { return s * (*self); }
  
  TriangularComplexMatrix __truediv__(NumericalComplex s) { return (*self) / s; }
  
  } // TriangularComplexMatrix
} // OT
