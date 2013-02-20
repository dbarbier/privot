// SWIG file InverseTrendEvaluationImplementation.i
// @author schueller
// @date   2012-01-04 12:26:21 +0100 (mer. 04 janv. 2012)

%{
#include "InverseTrendEvaluationImplementation.hxx"
%}

%include InverseTrendEvaluationImplementation.hxx
namespace OT { %extend InverseTrendEvaluationImplementation { InverseTrendEvaluationImplementation(const InverseTrendEvaluationImplementation & other) { return new OT::InverseTrendEvaluationImplementation(other); } } }
