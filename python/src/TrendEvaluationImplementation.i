// SWIG file TrendEvaluationImplementation.i
// @author schueller
// @date   2012-01-04 12:26:21 +0100 (mer. 04 janv. 2012)

%{
#include "TrendEvaluationImplementation.hxx"
%}

%include TrendEvaluationImplementation.hxx
namespace OT { %extend TrendEvaluationImplementation { TrendEvaluationImplementation(const TrendEvaluationImplementation & other) { return new OT::TrendEvaluationImplementation(other); } } }
