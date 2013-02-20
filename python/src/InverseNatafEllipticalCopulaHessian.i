// SWIG file InverseNatafEllipticalCopulaHessian.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "InverseNatafEllipticalCopulaHessian.hxx"
%}

%include InverseNatafEllipticalCopulaHessian.hxx
namespace OT { %extend InverseNatafEllipticalCopulaHessian { InverseNatafEllipticalCopulaHessian(const InverseNatafEllipticalCopulaHessian & other) { return new OT::InverseNatafEllipticalCopulaHessian(other); } } }
