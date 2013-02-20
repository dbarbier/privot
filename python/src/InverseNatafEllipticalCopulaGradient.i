// SWIG file InverseNatafEllipticalCopulaGradient.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "InverseNatafEllipticalCopulaGradient.hxx"
%}

%include InverseNatafEllipticalCopulaGradient.hxx
namespace OT { %extend InverseNatafEllipticalCopulaGradient { InverseNatafEllipticalCopulaGradient(const InverseNatafEllipticalCopulaGradient & other) { return new OT::InverseNatafEllipticalCopulaGradient(other); } } }
