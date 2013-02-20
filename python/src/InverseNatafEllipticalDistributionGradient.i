// SWIG file InverseNatafEllipticalDistributionGradient.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "InverseNatafEllipticalDistributionGradient.hxx"
%}

%include InverseNatafEllipticalDistributionGradient.hxx
namespace OT { %extend InverseNatafEllipticalDistributionGradient { InverseNatafEllipticalDistributionGradient(const InverseNatafEllipticalDistributionGradient & other) { return new OT::InverseNatafEllipticalDistributionGradient(other); } } }
