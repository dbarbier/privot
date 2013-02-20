// SWIG file RayleighFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "RayleighFactory.hxx"
%}

%include RayleighFactory.hxx
namespace OT { %extend RayleighFactory { RayleighFactory(const RayleighFactory & other) { return new OT::RayleighFactory(other); } } }
