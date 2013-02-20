// SWIG file QuadraticNumericalMathEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "QuadraticNumericalMathEvaluationImplementation.hxx"
%}

%include QuadraticNumericalMathEvaluationImplementation.hxx
namespace OT { %extend QuadraticNumericalMathEvaluationImplementation { QuadraticNumericalMathEvaluationImplementation(const QuadraticNumericalMathEvaluationImplementation & other) { return new OT::QuadraticNumericalMathEvaluationImplementation(other); } } }
