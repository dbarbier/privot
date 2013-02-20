// SWIG file LaguerreFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LaguerreFactory.hxx"
%}

%include LaguerreFactory.hxx
namespace OT { %extend LaguerreFactory { LaguerreFactory(const LaguerreFactory & other) { return new OT::LaguerreFactory(other); } } }
