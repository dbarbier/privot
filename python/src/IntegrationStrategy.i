// SWIG file IntegrationStrategy.i
// @author lebrun
// @date   2009-11-11 12:10:13 +0100 (mer. 11 nov. 2009)

%{
#include "IntegrationStrategy.hxx"
%}

%include IntegrationStrategy.hxx
namespace OT{ %extend IntegrationStrategy { IntegrationStrategy(const IntegrationStrategy & other) { return new OT::IntegrationStrategy(other); } } }
