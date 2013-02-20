// SWIG file RestrictedEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "RestrictedEvaluationImplementation.hxx"
%}

%include RestrictedEvaluationImplementation.hxx
namespace OT { %extend RestrictedEvaluationImplementation { RestrictedEvaluationImplementation(const RestrictedEvaluationImplementation & other) { return new OT::RestrictedEvaluationImplementation(other); } } }
