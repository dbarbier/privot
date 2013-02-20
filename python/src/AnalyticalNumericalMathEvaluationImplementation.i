// SWIG file AnalyticalNumericalMathEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "AnalyticalNumericalMathEvaluationImplementation.hxx"
%}

%include AnalyticalNumericalMathEvaluationImplementation.hxx
namespace OT { %extend AnalyticalNumericalMathEvaluationImplementation { AnalyticalNumericalMathEvaluationImplementation(const AnalyticalNumericalMathEvaluationImplementation & other) { return new OT::AnalyticalNumericalMathEvaluationImplementation(other); } } }
