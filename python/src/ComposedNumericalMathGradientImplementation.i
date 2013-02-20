// SWIG file ComposedNumericalMathGradientImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ComposedNumericalMathGradientImplementation.hxx"
%}

%include ComposedNumericalMathGradientImplementation.hxx
namespace OT { %extend ComposedNumericalMathGradientImplementation { ComposedNumericalMathGradientImplementation(const ComposedNumericalMathGradientImplementation & other) { return new OT::ComposedNumericalMathGradientImplementation(other); } } }
