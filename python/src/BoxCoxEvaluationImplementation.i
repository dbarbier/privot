// SWIG file BoxCoxEvaluationImplementation.i
// @author schueller
// @date   2012-01-04 12:26:21 +0100 (Wed, 04 Jan 2012)

%{
#include "BoxCoxEvaluationImplementation.hxx"
%}

%include BoxCoxEvaluationImplementation.hxx
namespace OT { %extend BoxCoxEvaluationImplementation { BoxCoxEvaluationImplementation(const BoxCoxEvaluationImplementation & other) { return new OT::BoxCoxEvaluationImplementation(other); } } }
