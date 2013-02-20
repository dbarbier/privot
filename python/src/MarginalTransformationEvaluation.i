// SWIG file MarginalTransformationEvaluation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "MarginalTransformationEvaluation.hxx"
%}

%include MarginalTransformationEvaluation.hxx
namespace OT { %extend MarginalTransformationEvaluation { MarginalTransformationEvaluation(const MarginalTransformationEvaluation & other) { return new OT::MarginalTransformationEvaluation(other); } } }
