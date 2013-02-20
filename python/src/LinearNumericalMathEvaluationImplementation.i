// SWIG file LinearNumericalMathEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LinearNumericalMathEvaluationImplementation.hxx"
%}

%include LinearNumericalMathEvaluationImplementation.hxx
namespace OT { %extend LinearNumericalMathEvaluationImplementation { LinearNumericalMathEvaluationImplementation(const LinearNumericalMathEvaluationImplementation & other) { return new OT::LinearNumericalMathEvaluationImplementation(other); } } }
