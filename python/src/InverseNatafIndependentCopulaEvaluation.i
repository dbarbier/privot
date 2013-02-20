// SWIG file InverseNatafIndependentCopulaEvaluation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "InverseNatafIndependentCopulaEvaluation.hxx"
%}

%include InverseNatafIndependentCopulaEvaluation.hxx
namespace OT { %extend InverseNatafIndependentCopulaEvaluation { InverseNatafIndependentCopulaEvaluation(const InverseNatafIndependentCopulaEvaluation & other) { return new OT::InverseNatafIndependentCopulaEvaluation(other); } } }
