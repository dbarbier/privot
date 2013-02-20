// SWIG file IdentityMatrix.i
// @author schueller
// @date   2012-02-15 18:09:50 +0100 (Wed, 15 Feb 2012)

%{
#include "IdentityMatrix.hxx"
%}

%include IdentityMatrix.hxx

namespace OT {  

%extend IdentityMatrix {
  IdentityMatrix(const IdentityMatrix & other) { return new OT::IdentityMatrix(other); }

  OTMatrixGetAccessor(IdentityMatrix, NumericalScalar)


  void __setitem__(PyObject * args, NumericalScalar val) {

    SWIG_exception_fail(SWIG_RuntimeError, "method IdentityMatrix___setitem__ is not available because these matrices are read-only");
fail:
    return;
  }

} // IdentityMatrix
} // OT
