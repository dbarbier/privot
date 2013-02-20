// SWIG file NatafEllipticalCopulaHessian.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NatafEllipticalCopulaHessian.hxx"
%}

%include NatafEllipticalCopulaHessian.hxx
namespace OT { %extend NatafEllipticalCopulaHessian { NatafEllipticalCopulaHessian(const NatafEllipticalCopulaHessian & other) { return new OT::NatafEllipticalCopulaHessian(other); } } }
