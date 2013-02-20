// SWIG file GumbelFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "GumbelFactory.hxx"
%}

%include GumbelFactory.hxx
namespace OT { %extend GumbelFactory { GumbelFactory(const GumbelFactory & other) { return new OT::GumbelFactory(other); } } }
