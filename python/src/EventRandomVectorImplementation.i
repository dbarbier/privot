// SWIG file EventRandomVectorImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "EventRandomVectorImplementation.hxx"
%}

%include EventRandomVectorImplementation.hxx
namespace OT { %extend EventRandomVectorImplementation { EventRandomVectorImplementation(const EventRandomVectorImplementation & other) { return new OT::EventRandomVectorImplementation(other); } } }
