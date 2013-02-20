// SWIG file GramSchmidtAlgorithm.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "GramSchmidtAlgorithm.hxx"
%}

%include GramSchmidtAlgorithm.hxx
namespace OT{ %extend GramSchmidtAlgorithm { GramSchmidtAlgorithm(const GramSchmidtAlgorithm & other) { return new OT::GramSchmidtAlgorithm(other); } } }
