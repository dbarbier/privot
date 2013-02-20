// SWIG file FunctionalChaosResult.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "FunctionalChaosResult.hxx"
%}

%include FunctionalChaosResult.hxx

namespace OT{ %extend FunctionalChaosResult { FunctionalChaosResult(const FunctionalChaosResult & other) { return new OT::FunctionalChaosResult(other); } } }
