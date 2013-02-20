// SWIG file NatafIndependentCopulaHessian.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NatafIndependentCopulaHessian.hxx"
%}

%include NatafIndependentCopulaHessian.hxx
namespace OT { %extend NatafIndependentCopulaHessian { NatafIndependentCopulaHessian(const NatafIndependentCopulaHessian & other) { return new OT::NatafIndependentCopulaHessian(other); } } }
