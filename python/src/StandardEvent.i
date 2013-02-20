// SWIG file StandardEvent.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "StandardEvent.hxx"
%}

%include StandardEvent.hxx
namespace OT { %extend StandardEvent { StandardEvent(const StandardEvent & other) { return new OT::StandardEvent(other); } } }
