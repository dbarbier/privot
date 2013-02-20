// SWIG file NumericalMathFunctionImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NumericalMathFunctionImplementation.hxx"
%}

%template(EvaluationImplementation) OT::Pointer<OT::NumericalMathEvaluationImplementation>;
%template(GradientImplementation)   OT::Pointer<OT::NumericalMathGradientImplementation>;
%template(HessianImplementation)    OT::Pointer<OT::NumericalMathHessianImplementation>;

%include NumericalMathFunctionImplementation.hxx
namespace OT { %extend NumericalMathFunctionImplementation { NumericalMathFunctionImplementation(const NumericalMathFunctionImplementation & other) { return new OT::NumericalMathFunctionImplementation(other); } } }
