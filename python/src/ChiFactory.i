// SWIG file ChiFactory.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "ChiFactory.hxx"
%}

%include ChiFactory.hxx
namespace OT { %extend ChiFactory { ChiFactory(const ChiFactory & other) { return new OT::ChiFactory(other); } } }
