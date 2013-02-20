// SWIG file NoNumericalMathGradientImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NoNumericalMathGradientImplementation.hxx"
%}

%include NoNumericalMathGradientImplementation.hxx
namespace OT { %extend NoNumericalMathGradientImplementation { NoNumericalMathGradientImplementation(const NoNumericalMathGradientImplementation & other) { return new OT::NoNumericalMathGradientImplementation(other); } } }
