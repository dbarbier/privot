// SWIG file LeastSquaresStrategy.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LeastSquaresStrategy.hxx"
%}

%include LeastSquaresStrategy.hxx
namespace OT{ %extend LeastSquaresStrategy { LeastSquaresStrategy(const LeastSquaresStrategy & other) { return new OT::LeastSquaresStrategy(other); } } }
