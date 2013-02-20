// SWIG file NatafIndependentCopulaEvaluation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NatafIndependentCopulaEvaluation.hxx"
%}

%include NatafIndependentCopulaEvaluation.hxx
namespace OT { %extend NatafIndependentCopulaEvaluation { NatafIndependentCopulaEvaluation(const NatafIndependentCopulaEvaluation & other) { return new OT::NatafIndependentCopulaEvaluation(other); } } }
