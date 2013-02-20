// SWIG file QuadraticLeastSquares.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "QuadraticLeastSquares.hxx"
%}

%include QuadraticLeastSquares.hxx
namespace OT { %extend QuadraticLeastSquares { QuadraticLeastSquares(const QuadraticLeastSquares & other) { return new OT::QuadraticLeastSquares(other); } } }
