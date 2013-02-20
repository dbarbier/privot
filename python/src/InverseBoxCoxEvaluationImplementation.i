// SWIG file InverseBoxCoxEvaluationImplementation.i
// @author schueller
// @date   2012-01-04 12:26:21 +0100 (Wed, 04 Jan 2012)

%{
#include "InverseBoxCoxEvaluationImplementation.hxx"
%}

%include InverseBoxCoxEvaluationImplementation.hxx
namespace OT { %extend InverseBoxCoxEvaluationImplementation { InverseBoxCoxEvaluationImplementation(const InverseBoxCoxEvaluationImplementation & other) { return new OT::InverseBoxCoxEvaluationImplementation(other); } } }
