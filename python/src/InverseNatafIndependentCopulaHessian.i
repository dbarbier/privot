// SWIG file InverseNatafIndependentCopulaHessian.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "InverseNatafIndependentCopulaHessian.hxx"
%}

%include InverseNatafIndependentCopulaHessian.hxx
namespace OT { %extend InverseNatafIndependentCopulaHessian { InverseNatafIndependentCopulaHessian(const InverseNatafIndependentCopulaHessian & other) { return new OT::InverseNatafIndependentCopulaHessian(other); } } }
