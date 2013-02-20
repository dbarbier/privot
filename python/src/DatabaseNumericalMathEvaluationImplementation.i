// SWIG file DatabaseNumericalMathEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "DatabaseNumericalMathEvaluationImplementation.hxx"
%}

%include DatabaseNumericalMathEvaluationImplementation.hxx
namespace OT { %extend DatabaseNumericalMathEvaluationImplementation { DatabaseNumericalMathEvaluationImplementation(const DatabaseNumericalMathEvaluationImplementation & other) { return new OT::DatabaseNumericalMathEvaluationImplementation(other); } } }
