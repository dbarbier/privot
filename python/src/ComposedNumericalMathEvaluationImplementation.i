// SWIG file ComposedNumericalMathEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ComposedNumericalMathEvaluationImplementation.hxx"
%}

%include ComposedNumericalMathEvaluationImplementation.hxx
namespace OT { %extend ComposedNumericalMathEvaluationImplementation { ComposedNumericalMathEvaluationImplementation(const ComposedNumericalMathEvaluationImplementation & other) { return new OT::ComposedNumericalMathEvaluationImplementation(other); } } }
