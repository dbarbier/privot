// SWIG file ProcessImplementation.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "ProcessImplementation.hxx"
%}

%include ProcessImplementation.hxx
namespace OT { %extend ProcessImplementation { ProcessImplementation(const ProcessImplementation & other) { return new OT::ProcessImplementation(other); } } }
