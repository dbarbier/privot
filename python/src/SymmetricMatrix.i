// SWIG file SymmetricMatrix.i
// @author schueller
// @date   2012-02-15 18:09:50 +0100 (Wed, 15 Feb 2012)

%{
#include "SymmetricMatrix.hxx"
%}
%apply const NumericalScalarCollection & { const OT::SymmetricMatrix::NumericalScalarCollection & };

%include SymmetricMatrix.hxx

namespace OT {  

%extend SymmetricMatrix {

  SymmetricMatrix(const SymmetricMatrix & other) { return new OT::SymmetricMatrix(other); }

  SymmetricMatrix(PyObject * pyObj) { return new OT::SymmetricMatrix( OT::convert<OT::_PySequence_,OT::SymmetricMatrix>(pyObj) ); }

  OTMatrixAccessors(SymmetricMatrix, NumericalScalar)
  
  SymmetricMatrix __rmul__(NumericalScalar s) { return s * (*self); }
    
  SymmetricMatrix __truediv__(NumericalScalar s) { return (*self) / s; }

} // SymmetricMatrix
} // OT
