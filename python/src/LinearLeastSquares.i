// SWIG file LinearLeastSquares.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LinearLeastSquares.hxx"
%}

%include LinearLeastSquares.hxx
namespace OT { %extend LinearLeastSquares { LinearLeastSquares(const LinearLeastSquares & other) { return new OT::LinearLeastSquares(other); } } }
