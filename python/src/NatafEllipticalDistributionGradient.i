// SWIG file NatafEllipticalDistributionGradient.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NatafEllipticalDistributionGradient.hxx"
%}

%include NatafEllipticalDistributionGradient.hxx
namespace OT { %extend NatafEllipticalDistributionGradient { NatafEllipticalDistributionGradient(const NatafEllipticalDistributionGradient & other) { return new OT::NatafEllipticalDistributionGradient(other); } } }
