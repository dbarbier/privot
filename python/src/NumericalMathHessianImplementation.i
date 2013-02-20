// SWIG file NumericalMathHessianImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NumericalMathHessianImplementation.hxx"
%}

%include NumericalMathHessianImplementation.hxx
namespace OT { %extend NumericalMathHessianImplementation { NumericalMathHessianImplementation(const NumericalMathHessianImplementation & other) { return new OT::NumericalMathHessianImplementation(other); } } }
