// SWIG file HermiteFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "HermiteFactory.hxx"
%}

%include HermiteFactory.hxx
namespace OT { %extend HermiteFactory { HermiteFactory(const HermiteFactory & other) { return new OT::HermiteFactory(other); } } }
