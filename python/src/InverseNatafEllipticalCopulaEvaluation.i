// SWIG file InverseNatafEllipticalCopulaEvaluation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "InverseNatafEllipticalCopulaEvaluation.hxx"
%}

%include InverseNatafEllipticalCopulaEvaluation.hxx
namespace OT { %extend InverseNatafEllipticalCopulaEvaluation { InverseNatafEllipticalCopulaEvaluation(const InverseNatafEllipticalCopulaEvaluation & other) { return new OT::InverseNatafEllipticalCopulaEvaluation(other); } } }
