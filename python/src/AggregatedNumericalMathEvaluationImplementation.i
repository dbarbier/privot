// SWIG file AggregatedNumericalMathEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "AggregatedNumericalMathEvaluationImplementation.hxx"
%}

%include AggregatedNumericalMathEvaluationImplementation.hxx

namespace OT { %extend AggregatedNumericalMathEvaluationImplementation { AggregatedNumericalMathEvaluationImplementation(const AggregatedNumericalMathEvaluationImplementation & other) { return new OT::AggregatedNumericalMathEvaluationImplementation(other); } } }
