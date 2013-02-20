// SWIG file Event.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Event.hxx"
%}

%include Event.hxx
namespace OT { %extend Event { Event(const Event & other) { return new OT::Event(other); } } }
