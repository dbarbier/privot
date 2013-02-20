// SWIG file InverseNatafIndependentCopulaGradient.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "InverseNatafIndependentCopulaGradient.hxx"
%}

%include InverseNatafIndependentCopulaGradient.hxx
namespace OT { %extend InverseNatafIndependentCopulaGradient { InverseNatafIndependentCopulaGradient(const InverseNatafIndependentCopulaGradient & other) { return new OT::InverseNatafIndependentCopulaGradient(other); } } }
