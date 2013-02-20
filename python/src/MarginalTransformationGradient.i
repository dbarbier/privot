// SWIG file MarginalTransformationGradient.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "MarginalTransformationGradient.hxx"
%}

%include MarginalTransformationGradient.hxx
namespace OT { %extend MarginalTransformationGradient { MarginalTransformationGradient(const MarginalTransformationGradient & other) { return new OT::MarginalTransformationGradient(other); } } }
