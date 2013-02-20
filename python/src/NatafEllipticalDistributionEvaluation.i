// SWIG file NatafEllipticalDistributionEvaluation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NatafEllipticalDistributionEvaluation.hxx"
%}

%include NatafEllipticalDistributionEvaluation.hxx
namespace OT { %extend NatafEllipticalDistributionEvaluation { NatafEllipticalDistributionEvaluation(const NatafEllipticalDistributionEvaluation & other) { return new OT::NatafEllipticalDistributionEvaluation(other); } } }
