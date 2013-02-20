// SWIG file NoNumericalMathEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NoNumericalMathEvaluationImplementation.hxx"
%}

%include NoNumericalMathEvaluationImplementation.hxx
namespace OT { %extend NoNumericalMathEvaluationImplementation { NoNumericalMathEvaluationImplementation(const NoNumericalMathEvaluationImplementation & other) { return new OT::NoNumericalMathEvaluationImplementation(other); } } }
