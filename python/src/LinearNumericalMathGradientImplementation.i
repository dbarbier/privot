// SWIG file LinearNumericalMathGradientImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LinearNumericalMathGradientImplementation.hxx"
%}

%include LinearNumericalMathGradientImplementation.hxx
namespace OT { %extend LinearNumericalMathGradientImplementation { LinearNumericalMathGradientImplementation(const LinearNumericalMathGradientImplementation & other) { return new OT::LinearNumericalMathGradientImplementation(other); } } }
