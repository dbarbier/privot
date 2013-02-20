// SWIG file LinearNumericalMathFunction.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LinearNumericalMathFunction.hxx"
%}

%include LinearNumericalMathFunction.hxx
namespace OT { %extend LinearNumericalMathFunction { LinearNumericalMathFunction(const LinearNumericalMathFunction & other) { return new OT::LinearNumericalMathFunction(other); } } }
