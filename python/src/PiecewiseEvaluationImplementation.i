// SWIG file PiecewiseEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "PiecewiseEvaluationImplementation.hxx"
%}

%include PiecewiseEvaluationImplementation.hxx

namespace OT { %extend PiecewiseEvaluationImplementation { PiecewiseEvaluationImplementation(const PiecewiseEvaluationImplementation & other) { return new OT::PiecewiseEvaluationImplementation(other); } } }
