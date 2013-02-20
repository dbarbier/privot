// SWIG file NatafEllipticalCopulaGradient.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NatafEllipticalCopulaGradient.hxx"
%}

%include NatafEllipticalCopulaGradient.hxx
namespace OT { %extend NatafEllipticalCopulaGradient { NatafEllipticalCopulaGradient(const NatafEllipticalCopulaGradient & other) { return new OT::NatafEllipticalCopulaGradient(other); } } }
