// SWIG file NumericalMathEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NumericalMathEvaluationImplementation.hxx"
%}

%include NumericalMathEvaluationImplementation.hxx
namespace OT { %extend NumericalMathEvaluationImplementation { NumericalMathEvaluationImplementation(const NumericalMathEvaluationImplementation & other) { return new OT::NumericalMathEvaluationImplementation(other); } } }
