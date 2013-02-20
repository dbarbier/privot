// SWIG file CharlierFactory.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "CharlierFactory.hxx"
%}

%include CharlierFactory.hxx
namespace OT { %extend CharlierFactory { CharlierFactory(const CharlierFactory & other) { return new OT::CharlierFactory(other); } } }
