// SWIG file BetaFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "BetaFactory.hxx"
%}

%include BetaFactory.hxx
namespace OT { %extend BetaFactory { BetaFactory(const BetaFactory & other) { return new OT::BetaFactory(other); } } }
