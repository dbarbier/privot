// SWIG file JacobiFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "JacobiFactory.hxx"
%}

%include JacobiFactory.hxx
namespace OT { %extend JacobiFactory { JacobiFactory(const JacobiFactory & other) { return new OT::JacobiFactory(other); } } }
