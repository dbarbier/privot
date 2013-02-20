// SWIG file DomainImplementation.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "DomainImplementation.hxx"
%}

%include DomainImplementation.hxx
namespace OT { %extend DomainImplementation { DomainImplementation(const DomainImplementation & other) { return new OT::DomainImplementation(other); } } }
