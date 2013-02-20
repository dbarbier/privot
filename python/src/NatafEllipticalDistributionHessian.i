// SWIG file NatafEllipticalDistributionHessian.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NatafEllipticalDistributionHessian.hxx"
%}

%include NatafEllipticalDistributionHessian.hxx
namespace OT { %extend NatafEllipticalDistributionHessian { NatafEllipticalDistributionHessian(const NatafEllipticalDistributionHessian & other) { return new OT::NatafEllipticalDistributionHessian(other); } } }
