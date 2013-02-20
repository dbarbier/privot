// SWIG file InverseNatafEllipticalDistributionEvaluation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "InverseNatafEllipticalDistributionEvaluation.hxx"
%}

%include InverseNatafEllipticalDistributionEvaluation.hxx
namespace OT { %extend InverseNatafEllipticalDistributionEvaluation { InverseNatafEllipticalDistributionEvaluation(const InverseNatafEllipticalDistributionEvaluation & other) { return new OT::InverseNatafEllipticalDistributionEvaluation(other); } } }
