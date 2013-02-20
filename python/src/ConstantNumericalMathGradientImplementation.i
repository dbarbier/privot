// SWIG file ConstantNumericalMathGradientImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ConstantNumericalMathGradientImplementation.hxx"
%}

%include ConstantNumericalMathGradientImplementation.hxx
namespace OT { %extend ConstantNumericalMathGradientImplementation { ConstantNumericalMathGradientImplementation(const ConstantNumericalMathGradientImplementation & other) { return new OT::ConstantNumericalMathGradientImplementation(other); } } }
