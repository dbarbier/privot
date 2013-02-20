// SWIG file NatafIndependentCopulaGradient.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NatafIndependentCopulaGradient.hxx"
%}

%include NatafIndependentCopulaGradient.hxx
namespace OT { %extend NatafIndependentCopulaGradient { NatafIndependentCopulaGradient(const NatafIndependentCopulaGradient & other) { return new OT::NatafIndependentCopulaGradient(other); } } }
