// SWIG file NumericalMathGradientImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NumericalMathGradientImplementation.hxx"
%}

%include NumericalMathGradientImplementation.hxx
namespace OT { %extend NumericalMathGradientImplementation { NumericalMathGradientImplementation(const NumericalMathGradientImplementation & other) { return new OT::NumericalMathGradientImplementation(other); } } }
