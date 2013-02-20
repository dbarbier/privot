// SWIG file MarginalTransformationHessian.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "MarginalTransformationHessian.hxx"
%}

%include MarginalTransformationHessian.hxx
namespace OT { %extend MarginalTransformationHessian { MarginalTransformationHessian(const MarginalTransformationHessian & other) { return new OT::MarginalTransformationHessian(other); } } }
