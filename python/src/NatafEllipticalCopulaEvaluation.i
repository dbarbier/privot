// SWIG file NatafEllipticalCopulaEvaluation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NatafEllipticalCopulaEvaluation.hxx"
%}

%include NatafEllipticalCopulaEvaluation.hxx
namespace OT { %extend NatafEllipticalCopulaEvaluation { NatafEllipticalCopulaEvaluation(const NatafEllipticalCopulaEvaluation & other) { return new OT::NatafEllipticalCopulaEvaluation(other); } } }
