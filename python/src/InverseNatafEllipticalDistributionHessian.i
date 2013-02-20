// SWIG file InverseNatafEllipticalDistributionHessian.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "InverseNatafEllipticalDistributionHessian.hxx"
%}

%include InverseNatafEllipticalDistributionHessian.hxx
namespace OT { %extend InverseNatafEllipticalDistributionHessian { InverseNatafEllipticalDistributionHessian(const InverseNatafEllipticalDistributionHessian & other) { return new OT::InverseNatafEllipticalDistributionHessian(other); } } }
