// SWIG file ConstantNumericalMathHessianImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ConstantNumericalMathHessianImplementation.hxx"
%}

%include ConstantNumericalMathHessianImplementation.hxx
namespace OT { %extend ConstantNumericalMathHessianImplementation { ConstantNumericalMathHessianImplementation(const ConstantNumericalMathHessianImplementation & other) { return new OT::ConstantNumericalMathHessianImplementation(other); } } }
