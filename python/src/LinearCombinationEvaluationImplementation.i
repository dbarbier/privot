// SWIG file LinearCombinationEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LinearCombinationEvaluationImplementation.hxx"
%}

%include LinearCombinationEvaluationImplementation.hxx

namespace OT { %extend LinearCombinationEvaluationImplementation { LinearCombinationEvaluationImplementation(const LinearCombinationEvaluationImplementation & other) { return new OT::LinearCombinationEvaluationImplementation(other); } } }
