// SWIG file ComposedNumericalMathHessianImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ComposedNumericalMathHessianImplementation.hxx"
%}

%include ComposedNumericalMathHessianImplementation.hxx
namespace OT { %extend ComposedNumericalMathHessianImplementation { ComposedNumericalMathHessianImplementation(const ComposedNumericalMathHessianImplementation & other) { return new OT::ComposedNumericalMathHessianImplementation(other); } } }
