// SWIG file EventDomainImplementation.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "EventDomainImplementation.hxx"
%}

%include EventDomainImplementation.hxx
namespace OT { %extend EventDomainImplementation { EventDomainImplementation(const EventDomainImplementation & other) { return new OT::EventDomainImplementation(other); } } }
