// SWIG file NoNumericalMathHessianImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NoNumericalMathHessianImplementation.hxx"
%}

%include NoNumericalMathHessianImplementation.hxx
namespace OT { %extend NoNumericalMathHessianImplementation { NoNumericalMathHessianImplementation(const NoNumericalMathHessianImplementation & other) { return new OT::NoNumericalMathHessianImplementation(other); } } }
