// SWIG file FunctionalChaosAlgorithm.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "FunctionalChaosAlgorithm.hxx"
%}

%include FunctionalChaosAlgorithm.hxx

namespace OT{ %extend FunctionalChaosAlgorithm { FunctionalChaosAlgorithm(const FunctionalChaosAlgorithm & other) { return new OT::FunctionalChaosAlgorithm(other); } } }
