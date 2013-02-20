// SWIG file IndicatorNumericalMathEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "IndicatorNumericalMathEvaluationImplementation.hxx"
%}

%include IndicatorNumericalMathEvaluationImplementation.hxx
namespace OT { %extend IndicatorNumericalMathEvaluationImplementation { IndicatorNumericalMathEvaluationImplementation(const IndicatorNumericalMathEvaluationImplementation & other) { return new OT::IndicatorNumericalMathEvaluationImplementation(other); } } }
