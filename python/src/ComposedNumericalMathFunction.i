// SWIG file ComposedNumericalMathFunction.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ComposedNumericalMathFunction.hxx"
%}

%include ComposedNumericalMathFunction.hxx
namespace OT { %extend ComposedNumericalMathFunction { ComposedNumericalMathFunction(const ComposedNumericalMathFunction & other) { return new OT::ComposedNumericalMathFunction(other); } } }
